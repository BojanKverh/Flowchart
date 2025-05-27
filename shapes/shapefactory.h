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
    static std::unique_ptr<data::AbstractShape> shape(int type);
};

}   // namespace
