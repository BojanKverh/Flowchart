#include "shapeprocess.h"

#include "../shapes/shapeprocess.h"
#include "../shapes/shapedecision.h"
#include "../shapes/shapeio.h"

namespace json {

ShapeProcess::ShapeProcess() : AbstractShape()
{}

QJsonObject ShapeProcess::toJson(data::AbstractShape* shape) const
{
    auto obj = AbstractShape::toJson(shape);

    obj[m_cqsText] = shape->text();

    return obj;
}

data::AbstractShape* ShapeProcess::fromJson(const QJsonObject& obj) const
{
    auto type = static_cast<data::ShapeType>(obj[m_cqsType].toInt());
    data::AbstractShape* shape = nullptr;
    switch(type) {
    case data::ShapeType::esIO:
        shape = new data::ShapeIO;
        break;
    case data::ShapeType::esProcess:
        shape = new data::ShapeProcess;
        break;

    case data::ShapeType::esDecision:
        shape = new data::ShapeDecision;
        break;

    default:
        return nullptr;
    }

    shape->setText(obj[m_cqsText].toString());
    apply(shape, obj);
    return shape;
}

} // namespace

