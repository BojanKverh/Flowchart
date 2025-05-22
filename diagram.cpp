#include "diagram.h"

#include <algorithm>
#include <unordered_set>

namespace data {

Diagram::Diagram() {}

Diagram::Error Diagram::addShape(std::unique_ptr<AbstractShape> shape)
{
    if ((shape->type() == ShapeType::esStart) && (hasStart() == true))
        return Error::eStartExists;
    if ((shape->type() == ShapeType::esEnd) && (hasEnd() == true))
        return Error::eEndExists;

    m_vShapes.push_back(std::move(shape));
    return Error::eNone;
}

int Diagram::indexOf(AbstractShape* shape) const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [shape](const std::unique_ptr<AbstractShape>& it) {
        return it.get() == shape;
    });

    if (it != m_vShapes.end())
        return std::distance(m_vShapes.begin(), it);
    else
        return -1;
}

Diagram::Error Diagram::addConnection(const Connection& con)
{
    m_vConnections.push_back(con);
    return Error::eNone;
}

void Diagram::selectShape(int n)
{
    for (size_t i = 0; i < m_vShapes.size(); ++i)
        m_vShapes[i]->setSelected(static_cast<int>(i) == n);
}

void Diagram::selectConnection(int n)
{
    for (size_t i = 0; i < m_vConnections.size(); ++i)
        m_vConnections[i].setSelected(static_cast<int>(i) == n);
}

int Diagram::findShape(QPointF pt) const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [pt](const auto& shape) {
        return shape->contains(pt);
    });
    return (it == m_vShapes.end()? -1 : std::distance(m_vShapes.begin(), it));
}

int Diagram::findConnection(QPointF pt) const
{
    auto it = std::find_if(m_vConnections.begin(), m_vConnections.end(), [pt](const auto& con) {
        return con.contains(pt);
    });

    return (it == m_vConnections.end()? -1 : std::distance(m_vConnections.begin(), it));
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

void Diagram::deleteSelected()
{
    std::unordered_set<AbstractShape*> set;
    auto itRemoveShapes = std::remove_if(m_vShapes.begin(), m_vShapes.end(), [&](const std::unique_ptr<AbstractShape>& it) {
        if (it->isSelected() == true) {
            set.insert(it.get());
            return true;
        }
        return false;
    });

    auto itRemoveConnections = std::remove_if(m_vConnections.begin(), m_vConnections.end(), [&](const Connection& con) {
        return (set.find(con.out()) != set.end()) || (set.find(con.in()) != set.end());
    });

    m_vConnections.erase(itRemoveConnections, m_vConnections.end());
    m_vShapes.erase(itRemoveShapes, m_vShapes.end());

    itRemoveConnections = std::remove_if(m_vConnections.begin(), m_vConnections.end(), [&](const Connection& con) {
        return con.isSelected();
    });

    m_vConnections.erase(itRemoveConnections, m_vConnections.end());
}

Connection Diagram::findConnector(QPointF pt) const
{
    for (const auto& shape : m_vShapes) {
        int outIndex = shape->findOutput(pt);
        if (outIndex >= 0)
            return Connection(shape.get(), outIndex, nullptr);
        if (shape->isOnInput(pt) == true)
            return Connection(nullptr, -1, shape.get());
    }

    return Connection(nullptr, -1, nullptr);
}

bool Diagram::hasStart() const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [](const auto& shape) {
        return shape->type() == ShapeType::esStart;
    });
    return it != m_vShapes.end();
}

bool Diagram::hasEnd() const
{
    auto it = std::find_if(m_vShapes.begin(), m_vShapes.end(), [](const auto& shape) {
        return shape->type() == ShapeType::esEnd;
    });
    return it != m_vShapes.end();
}

} // namespace
