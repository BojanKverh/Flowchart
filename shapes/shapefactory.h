#pragma once

#include <memory>

#include "abstractshape.h"

namespace data {
/**
 * @brief The ShapeFactory class This factory is used to generate a
 * proper shape depending on type
 */
class ShapeFactory
{
public:
    /**
     * @brief shape Returns the shape of the given type
     * @param type type of shape
     * @return unique pointer to the newly created shape
     */
    static std::unique_ptr<data::AbstractShape> shape(int type);
    /**
     * @brief copy Makes a hard copy of the shape
     * @param shape Pointer to the shape to copy
     * @return Hard copy of the given shape
     */
    static std::unique_ptr<data::AbstractShape> copy(AbstractShape* shape);
};

}   // namespace
