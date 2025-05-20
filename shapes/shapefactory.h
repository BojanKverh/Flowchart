#pragma once

#include <memory>

#include "abstractshape.h"

/**
 * @brief The ShapeFactory class This factory is used to generate a
 * proper shape depending on type
 */
class ShapeFactory
{
public:
    static std::unique_ptr<AbstractShape> shape(int type);
};
