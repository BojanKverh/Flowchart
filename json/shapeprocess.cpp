#include "shapeprocess.h"

namespace json {

ShapeProcess::ShapeProcess() : AbstractShape()
{}

QJsonObject ShapeProcess::toJson(data::AbstractShape* shape) const
{
    auto obj = AbstractShape::toJson(shape);

    obj["text"] = shape->text();

    return obj;
}

} // namespace

