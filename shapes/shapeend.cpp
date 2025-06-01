#include <QObject>

#include "shapeend.h"

namespace data {

ShapeEnd::ShapeEnd()
  : AbstractShape()
{
  m_type      = ShapeType::esEnd;
  m_text      = QObject::tr("END");
  m_connectIn = QPointF(0.5, 0);
}

void ShapeEnd::draw(QPainter& P, double f)
{
  P.setPen(QPen(m_fg, 3));
  P.setBrush(m_bg);

  auto rect   = scale(f);
  auto center = global(rect, 0.5, 0.5);

  P.drawEllipse(center, rect.width() * 0.5, rect.height() * 0.5);
  P.drawText(rect, Qt::AlignCenter, m_text);

  drawDecorations(P, f);
}

} // namespace data
