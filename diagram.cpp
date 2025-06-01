#include "diagram.h"

#include "shapes/shapefactory.h"

#include <algorithm>
#include <unordered_set>

#include <QDebug>

namespace data {

Diagram::Diagram()
{
  m_name = "";
}

Diagram::Diagram(const Diagram& diagram)
{
  for (const auto& shape : diagram.shapes()) {
    addShape(ShapeFactory::copy(shape.get()));
  }

  for (const auto& con : diagram.connections()) {
    auto out = diagram.findShape(con.out());
    auto in  = diagram.findShape(con.in());
    Connection copy(m_vShapes[out].get(), con.outIndex(), m_vShapes[in].get());
  }
}

void Diagram::clear()
{
  m_vConnections.clear();
  m_vShapes.clear();
  m_stack.clear();
}

bool Diagram::isEmpty() const
{
  return (m_vShapes.size() == 0) && (m_stack.count() == 0);
}

void Diagram::addOperation(QUndoCommand* com)
{
  m_stack.push(com);
}

void Diagram::undo()
{
  m_stack.undo();
}

void Diagram::redo()
{
  m_stack.redo();
}

std::unordered_set<int> Diagram::selectedShapes() const
{
  std::unordered_set<int> set;

  for (size_t i = 0; i < m_vShapes.size(); ++i)
    if (m_vShapes[i]->isSelected() == true)
      set.insert(i);

  return set;
}

std::unordered_set<int> Diagram::selectedConnections() const
{
  std::unordered_set<int> set;

  for (size_t i = 0; i < m_vConnections.size(); ++i)
    if (m_vConnections[i].isSelected() == true)
      set.insert(i);

  return set;
}

std::tuple<std::unordered_set<int>, std::unordered_set<int>> Diagram::inside(QRectF rect) const
{
  std::unordered_set<int> shapes;
  for (size_t i = 0; i < m_vShapes.size(); ++i) {
    if (rect.contains(m_vShapes[i]->rect()) == true)
      shapes.insert(i);
  }

  std::unordered_set<int> cons;
  for (size_t i = 0; i < m_vConnections.size(); ++i) {
    const auto& con = m_vConnections[i];
    if ((rect.contains(con.out()->rect()) == true) && (rect.contains(con.in()->rect()) == true)) {
      cons.insert(i);
    }
  }

  return std::make_tuple(shapes, cons);
}

int Diagram::indexOf(AbstractShape* shape) const
{
  auto it =
    std::find_if(m_vShapes.begin(), m_vShapes.end(),
                 [shape](const std::unique_ptr<AbstractShape>& it) { return it.get() == shape; });

  if (it != m_vShapes.end())
    return std::distance(m_vShapes.begin(), it);
  else
    return -1;
}

int Diagram::findShape(QPointF pt, bool extended) const
{
  auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [pt, extended](const auto& shape) {
    return shape->contains(pt, extended);
  });
  return (it == m_vShapes.end() ? -1 : std::distance(m_vShapes.begin(), it));
}

data::Edge Diagram::findEdge(int i, QPointF pt) const
{
  if (i < 0)
    return Edge::eNone;

  QVector<QRectF> v;
  const double size = 5;
  const auto& shape = m_vShapes[i];
  double x          = shape->position().x();
  double y          = shape->position().y();
  double w          = shape->size().width();
  double h          = shape->size().height();
  v << QRectF(x, y, 0, 0) << QRectF(x + w, y, 0, 0) << QRectF(x + w, y + h, 0, 0)
    << QRectF(x, y + h, 0, 0) << QRectF(x, y, w, 0) << QRectF(x + w, y, 0, h)
    << QRectF(x, y + h, w, 0) << QRectF(x, y, 0, h);

  for (int i = 0; i < v.size(); ++i) {
    if (v[i].adjusted(-size, -size, size, size).contains(pt) == true)
      return static_cast<data::Edge>(i + 1);
  }

  return data::Edge::eNone;
}

int Diagram::findShape(data::AbstractShape* shape) const
{
  auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(),
                         [shape](const auto& itShape) { return itShape.get() == shape; });
  return (it == m_vShapes.end() ? -1 : std::distance(m_vShapes.begin(), it));
}

int Diagram::findConnection(QPointF pt) const
{
  auto it = std::find_if(m_vConnections.begin(), m_vConnections.end(),
                         [pt](const auto& con) { return con.contains(pt); });

  return (it == m_vConnections.end() ? -1 : std::distance(m_vConnections.begin(), it));
}

Connection Diagram::findConnector(int index, QPointF pt) const
{
  if (index < 0)
    return Connection();

  auto* shape  = m_vShapes[index].get();
  int outIndex = shape->findOutput(pt);
  if (outIndex >= 0)
    return Connection(shape, outIndex, nullptr);
  if (shape->isOnInput(pt) == true)
    return Connection(nullptr, -1, shape);
  return Connection();
}

bool Diagram::hasStart() const
{
  auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(),
                         [](const auto& shape) { return shape->type() == ShapeType::esStart; });
  return it != m_vShapes.end();
}

bool Diagram::hasEnd() const
{
  auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(),
                         [](const auto& shape) { return shape->type() == ShapeType::esEnd; });
  return it != m_vShapes.end();
}

void Diagram::restoreShapes(std::vector<std::unique_ptr<AbstractShape>>& v)
{
  for (size_t i = 0; i < v.size(); ++i)
    if (v[i] != nullptr)
      m_vShapes.insert(m_vShapes.begin() + i, std::move(v[i]));
}

Diagram::Error Diagram::addShape(std::unique_ptr<AbstractShape> shape)
{
  if ((shape->type() == ShapeType::esStart) && (hasStart() == true))
    return Error::eStartExists;
  if ((shape->type() == ShapeType::esEnd) && (hasEnd() == true))
    return Error::eEndExists;

  m_vShapes.push_back(std::move(shape));
  return Error::eNone;
}

void Diagram::removeShape(int i)
{
  m_vShapes.erase(m_vShapes.begin() + i);
}

void Diagram::selectShape(int n)
{
  for (size_t i = 0; i < m_vShapes.size(); ++i)
    m_vShapes[i]->setSelected(static_cast<int>(i) == n);
}

void Diagram::restoreConnections(const std::vector<Connection> v)
{
  m_vConnections = v;
}

Diagram::Error Diagram::addConnection(const Connection& con)
{
  m_vConnections.push_back(con);
  return Error::eNone;
}

void Diagram::removeConnection(int i)
{
  m_vConnections.erase(m_vConnections.begin() + i);
}

void Diagram::selectConnection(int n)
{
  for (size_t i = 0; i < m_vConnections.size(); ++i)
    m_vConnections[i].setSelected(static_cast<int>(i) == n);
}

void Diagram::moveSelected(QPointF pt)
{
  std::unordered_set<size_t> set;
  for (auto& shape : m_vShapes) {
    if (shape->isSelected() == true) {
      shape->move(shape->position() + pt);
      for (size_t i = 0; i < m_vConnections.size(); ++i)
        if (m_vConnections[i].contains(shape.get()))
          set.insert(i);
    }
  }

  for (auto it = set.begin(); it != set.end(); ++it) {
    m_vConnections[*it].update();
  }
}

void Diagram::copySelected(const Diagram& diagram)
{
  size_t n = m_vShapes.size();
  std::unordered_map<size_t, size_t> map;
  const auto& others = diagram.shapes();
  for (size_t i = 0; i < diagram.shapes().size(); ++i) {
  }
}

std::tuple<std::vector<std::unique_ptr<AbstractShape>>, std::vector<Connection>>
Diagram::deleteSelected()
{
  std::unordered_set<AbstractShape*> set;
  std::vector<data::Connection> deletedCons = m_vConnections;
  std::vector<std::unique_ptr<data::AbstractShape>> deletedShapes;

  size_t i = 0;
  while (i < m_vShapes.size()) {
    if (m_vShapes[i]->isSelected() == true) {
      set.insert(m_vShapes[i].get());
      deletedShapes.push_back(std::move(m_vShapes[i]));
      m_vShapes.erase(m_vShapes.begin() + i);
    } else {
      ++i;
      deletedShapes.push_back(nullptr);
    }
  }

  auto itRemoveConnections =
    std::remove_if(m_vConnections.begin(), m_vConnections.end(), [&](const Connection& con) {
      return (set.find(con.out()) != set.end()) || (set.find(con.in()) != set.end());
    });

  m_vConnections.erase(itRemoveConnections, m_vConnections.end());

  itRemoveConnections = std::remove_if(m_vConnections.begin(), m_vConnections.end(),
                                       [&](const Connection& con) { return con.isSelected(); });

  m_vConnections.erase(itRemoveConnections, m_vConnections.end());

  return std::make_tuple(std::move(deletedShapes), std::move(deletedCons));
}

} // namespace data
