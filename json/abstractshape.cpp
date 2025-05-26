#include "abstractshape.h"

#include "../shapes/shapestart.h"
#include "../shapes/shapeend.h"

namespace json {

AbstractShape::AbstractShape()
{}

QJsonObject AbstractShape::toJson(data::AbstractShape* shape) const
{
    QJsonObject obj;
    QJsonObject pt;
    QJsonObject sz;
    obj[m_cqsType] = static_cast<int>(shape->type());
    pt[m_cqsX] = shape->position().x();
    pt[m_cqsY] = shape->position().y();
    obj[m_cqsPosition] = pt;
    sz[m_cqsWidth] = shape->size().width();
    sz[m_cqsHeight] = shape->size().height();
    obj[m_cqsSize] = sz;
    obj[m_cqsBackgroundColor] = shape->backgroundColor().name();
    obj[m_cqsTextColor] = shape->textColor().name();

    return obj;
}

data::AbstractShape* AbstractShape::fromJson(const QJsonObject& obj) const
{
    auto type = static_cast<data::ShapeType>(obj[m_cqsType].toInt());
    data::AbstractShape* shape = nullptr;
    switch(type) {
    case data::ShapeType::esStart:
        shape = new data::ShapeStart;
        break;
    case data::ShapeType::esEnd:
        shape = new data::ShapeEnd;
        break;

    default:
        return nullptr;
    }

    apply(shape, obj);
    return shape;
}

void AbstractShape::apply(data::AbstractShape* shape, const QJsonObject& obj) const
{
    auto pos = obj[m_cqsPosition].toObject();
    auto size = obj[m_cqsSize].toObject();
    shape->move(QPointF(pos[m_cqsX].toDouble(), pos[m_cqsY].toDouble()));
    shape->resize(QSize(size[m_cqsWidth].toDouble(), size[m_cqsHeight].toDouble()));
    shape->setBackgroundColor(QColor(obj[m_cqsBackgroundColor].toString()));
    shape->setTextColor(QColor(obj[m_cqsTextColor].toString()));
}

} // namespace

