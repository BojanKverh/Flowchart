#pragma once

#include "abstractshape.h"

/**
 * @brief The ShapeIO class This is a shape for I/O element
 */
class ShapeIO : public AbstractShape
{
public:
    /**
     * @brief ShapeIO Default constructor
     */
    ShapeIO();
    /**
     * @brief draw Draws the shape
     * @param P Reference to the pointer object
     * @param f scale parameter
     */
    void draw(QPainter& P, double f = 1.0) override;
};

