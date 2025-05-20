#pragma once

#include "abstractshape.h"

/**
 * @brief The ShapeEnd class This is a shape for end element
 */
class ShapeEnd : public AbstractShape
{
public:
    /**
     * @brief ShapeEnd Default constructor
     */
    ShapeEnd();
    /**
     * @brief draw Draws the shape
     * @param P Reference to the pointer object
     * @param f scale parameter
     */
    void draw(QPainter& P, double f = 1.0) override;
};
