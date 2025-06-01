#include "diagram.h"

#include "connection.h"
#include "shapefactory.h"

#include <QJsonArray>
#include <QJsonObject>

namespace json {

Diagram::Diagram(data::Diagram& diagram, int w, int h)
  : m_diagram(diagram)
  , m_w(w)
  , m_h(h)
{}

QJsonDocument Diagram::toJson() const
{
  QJsonDocument doc;

  QJsonObject obj;
  QJsonArray arrShapes;
  const auto& shapes = m_diagram.shapes();

  ShapeFactory factory;

  obj[m_cqsWidth]  = m_w;
  obj[m_cqsHeight] = m_h;

  for (const auto& shape : shapes) {
    auto* io = factory.io(shape.get());
    arrShapes << io->toJson(shape.get());
  }
  obj[m_cqsShapes] = arrShapes;

  QJsonArray arrCon;
  Connection conIO;
  const auto& connections = m_diagram.connections();

  for (const auto& con : connections) {
    arrCon << conIO.toJson(con, m_diagram);
  }
  obj[m_cqsConnections] = arrCon;

  doc.setObject(obj);
  return doc;
}

void Diagram::fromJson(const QJsonDocument& doc)
{
  m_diagram.clear();

  auto obj    = doc.object();
  m_w         = obj[m_cqsWidth].toInt();
  m_h         = obj[m_cqsHeight].toInt();
  auto shapes = obj[m_cqsShapes].toArray();
  ShapeFactory factory;

  for (const auto& oneShape : shapes) {
    auto objShape = oneShape.toObject();
    auto* io      = factory.io(static_cast<data::ShapeType>(objShape["type"].toInt()));
    auto* shape   = io->fromJson(objShape);
    m_diagram.addShape(std::unique_ptr<data::AbstractShape>(shape));
  }

  Connection conIO;
  auto cons = obj[m_cqsConnections].toArray();
  for (const auto& oneCon : cons) {
    auto objCon = oneCon.toObject();
    auto con    = conIO.fromJson(objCon, m_diagram);
    con.update();
    m_diagram.addConnection(con);
  }
}

} // namespace json
