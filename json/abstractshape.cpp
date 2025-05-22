#include "abstractshape.h"

namespace json {

AbstractShape::AbstractShape()
{}

QJsonObject AbstractShape::toJson(data::AbstractShape* shape) const
{
    QJsonObject obj;
    QJsonObject pt;
    QJsonObject sz;
    obj["type"] = static_cast<int>(shape->type());
    pt["x"] = shape->position().x();
    pt["y"] = shape->position().y();
    obj["pos"] = pt;
    sz["w"] = shape->size().width();
    sz["h"] = shape->size().height();
    obj["size"] = sz;
    obj["bg"] = shape->backgroundColor().name();
    obj["fg"] = shape->textColor().name();

    return obj;
}

} // namespace

