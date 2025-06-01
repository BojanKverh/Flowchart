#include <QObject>
#include <QPolygonF>

#include "shapedecision.h"

namespace data {

ShapeDecision::ShapeDecision()
  : AbstractShape()
{
  m_type      = ShapeType::esDecision;
  m_size      = QSize(100, 75);
  m_text      = QObject::tr("?");
  m_connectIn = QPointF(0.5, 0);
  m_vConnectsOut << QPointF(0, 0.5) << QPointF(1, 0.5);
}

void ShapeDecision::draw(QPainter& P, double f)
{
  auto rect = scale(f);

  P.setPen(QPen(m_fg, 3));
  P.setBrush(m_bg);

  QPolygonF poly;
  poly << global(rect, 0.5, 0) << global(rect, 0, 0.5) << global(rect, 0.5, 1)
       << global(rect, 1, 0.5);
  P.drawPolygon(poly);
  P.drawText(rect, Qt::AlignCenter, m_text);

  drawDecorations(P, f);
}

} // namespace data
