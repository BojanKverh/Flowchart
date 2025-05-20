#include <QObject>

#include "shapestart.h"

ShapeStart::ShapeStart() : AbstractShape()
{
    m_type = ShapeType::esStart;
    m_text = QObject::tr("START");
    m_vConnectsOut << QPointF(0.5, 1);
}

void ShapeStart::draw(QPainter& P, double f)
{
    auto rect = scale(f);
    auto center = global(rect, 0.5, 0.5);

    P.setPen(QPen(m_fg, 3));
    P.setBrush(m_bg);

    P.drawEllipse(center, rect.width() * 0.5, rect.height() * 0.5);
    P.drawText(rect, Qt::AlignCenter, m_text);

    drawDecorations(P, f);
}

