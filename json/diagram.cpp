#include "diagram.h"

#include "shapefactory.h"
#include "connection.h"

#include <QJsonObject>
#include <QJsonArray>

namespace json {

Diagram::Diagram(data::Diagram& diagram, int w, int h) : m_diagram(diagram),
    m_w(w), m_h(h)
{
}

QJsonDocument Diagram::toJson() const
{
    QJsonDocument doc;

    QJsonObject obj;
    QJsonArray arrShapes;
    const auto& shapes = m_diagram.shapes();

    ShapeFactory factory;

    obj["w"] = m_w;
    obj["h"] = m_h;

    for (const auto& shape : shapes) {
        auto* io = factory.io(shape.get());
        arrShapes << io->toJson(shape.get());
    }
    obj["shapes"] = arrShapes;

    QJsonArray arrCon;
    Connection conIO;
    const auto& connections = m_diagram.connections();

    for (const auto& con : connections) {
        arrCon << conIO.toJson(con, m_diagram);
    }
    obj["cons"] = arrCon;

    doc.setObject(obj);
    return doc;
}

} // namespace

