#include <QObject>

#include "shapeprocess.h"

namespace data {

ShapeProcess::ShapeProcess()
  : AbstractShape()
{
  m_type      = ShapeType::esProcess;
  m_connectIn = QPointF(0.5, 0);
  m_vConnectsOut << QPointF(0.5, 1);
}

void ShapeProcess::draw(QPainter& P, double f)
{
  auto rect = scale(f);

  P.setPen(QPen(m_fg, 3));
  P.setBrush(m_bg);

  P.drawRect(rect);
  P.drawText(rect, Qt::AlignCenter, m_text);

  drawDecorations(P, f);
}

} // namespace data
