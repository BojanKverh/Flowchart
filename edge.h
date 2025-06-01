#pragma once

#include <QPointF>

#include <tuple>

namespace data {

enum class Edge {
  eNone,
  eTopLeft,
  eTopRight,
  eBottomRight,
  eBottomLeft,
  eTop,
  eRight,
  eBottom,
  eLeft
};

/**
 * @brief The EdgeHandler class This class is used to calculated rectangle move
 */
class EdgeHandler
{
public:
  /**
   * @brief move Calculates the move coordinates
   * @param edge Edge type
   * @param oldPt Old point
   * @param newPt New point
   * @return returns the move coordinates in format <x, y, w, h>
   */
  static std::tuple<double, double, double, double> move(Edge edge, QPointF oldPt, QPointF newPt);
  /**
   * @brief shape Returns the cursor shape for given edge type
   * @param edge Edge type
   * @return Appropriate cursor shape
   */
  static Qt::CursorShape shape(Edge edge);
};

} // namespace data
