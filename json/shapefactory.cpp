#include "shapefactory.h"

namespace json {

ShapeFactory::ShapeFactory()
{
    m_abstract = std::make_unique<AbstractShape>();
    m_process = std::make_unique<ShapeProcess>();
}

AbstractShape* ShapeFactory::io(data::AbstractShape* shape) const
{
    return io(shape->type());
}

AbstractShape* ShapeFactory::io(data::ShapeType type) const
{
    switch (type) {
    case data::ShapeType::esStart:
    case data::ShapeType::esEnd:
        return m_abstract.get();

    default:
        return m_process.get();
    }
}

} // namespace
