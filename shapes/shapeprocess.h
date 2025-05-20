#pragma once

#include "abstractshape.h"

/**
 * @brief The ShapeProcess class This is a shape for start element
 */
class ShapeProcess : public AbstractShape
{
public:
    /**
     * @brief ShapeProcess Default constructor
     */
    ShapeProcess();
    /**
     * @brief draw Draws the shape
     * @param P Reference to the pointer object
     * @param f scale parameter
     */
    void draw(QPainter& P, double f = 1.0) override;
};
