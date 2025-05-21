#pragma once

#include "abstractshape.h"

namespace data {

/**
 * @brief The ShapeDecision class This is a shape for decision element
 */
class ShapeDecision : public AbstractShape
{
public:
    /**
     * @brief ShapeDecision Default constructor
     */
    ShapeDecision();
    /**
     * @brief draw Draws the shape
     * @param P Reference to the pointer object
     * @param f scale parameter
     */
    void draw(QPainter& P, double f = 1.0) override;
};

} // namespace
