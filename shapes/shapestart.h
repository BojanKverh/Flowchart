#pragma once

#include "abstractshape.h"

namespace data {

/**
 * @brief The ShapeStart class This is a shape for start element
 */
class ShapeStart : public AbstractShape
{
public:
  /**
   * @brief ShapeStart Default constructor
   */
  ShapeStart();
  /**
   * @brief draw Draws the shape
   * @param P Reference to the pointer object
   * @param f scale parameter
   */
  void draw(QPainter& P, double f = 1.0) override;
};

} // namespace data
