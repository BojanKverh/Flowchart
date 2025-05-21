#include <QObject>
#include <QPolygonF>

#include "shapeio.h"

namespace data {

ShapeIO::ShapeIO() : AbstractShape()
{
    m_type = ShapeType::esIO;
    m_connectIn = QPointF(0.5, 0);
    m_vConnectsOut << QPointF(0.5, 1);
}

void ShapeIO::draw(QPainter& P, double f)
{
    auto rect = scale(f);

    P.setPen(QPen(m_fg, 3));
    P.setBrush(m_bg);

    QPolygonF poly;
    poly << global(rect, 0.15, 0) << global(rect, 0, 1)
         << global(rect, 0.85, 1) << global(rect, 1, 0);
    P.drawPolygon(poly);
    P.drawText(rect, Qt::AlignCenter, m_text);

    drawDecorations(P, f);
}

} // namespace
