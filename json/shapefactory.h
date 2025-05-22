#pragma once

#include "abstractshape.h"
#include "shapeprocess.h"

#include "../shapes/abstractshape.h"

#include <memory.h>

namespace json {

/**
 * @brief The ShapeFactory class This class is used to obtain
 * the proper shape JSON reader/writer for given shape type
 */
class ShapeFactory
{
public:
    /**
     * @brief ShapeFactory Default constructor
     */
    ShapeFactory();

    /**
     * @brief io Returns a proper reader/writer
     * @param shape Shape to obtain the reader/writer object for
     * @return Pointer to the reader/writer. This object belongs to
     */
    AbstractShape* io(data::AbstractShape* shape) const;

private:
    std::unique_ptr<AbstractShape> m_abstract;
    std::unique_ptr<ShapeProcess> m_process;
};

} // namespace
