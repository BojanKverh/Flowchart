#include "abstractshape.h"

void AbstractShape::moveCenter(QPointF pt)
{
    m_pt = QPointF(pt.x() - m_size.width() / 2, pt.y() - m_size.height() / 2);
}

bool AbstractShape::contains(QPointF pt, double f) const
{
    return scale(f).contains(pt);
}

bool AbstractShape::isOnInput(QPointF pt, double f) const
{
    if (m_connectIn.has_value() == false)
        return false;

    auto shapeRect = scale(f);
    auto center = global(shapeRect, m_connectIn.value().x(), m_connectIn.value().y());
    auto rect = area(center, m_cConnectorSize);
    return rect.contains(pt);
}

int AbstractShape::findOutput(QPointF pt, double f) const
{
    auto shapeRect = scale(f);
    for (size_t i = 0; i < m_vConnectsOut.size(); ++i) {
        auto center = global(shapeRect, m_vConnectsOut[i].x(), m_vConnectsOut[i].y());
        auto rect = area(center, m_cConnectorSize);
        if (rect.contains(pt) == true)
            return i;
    }

    return -1;
}

QRectF AbstractShape::scale(double f) const
{
    return QRectF(m_pt*f, m_size*f);
}

QPointF AbstractShape::global(const QRectF& rect, double x, double y) const
{
    return QPointF(rect.left() + rect.width() * x, rect.top() + rect.height() * y);
}

void AbstractShape::drawDecorations(QPainter& P, double f)
{
    drawIn(P, f);
    drawOut(P, f);

    if (isSelected() == true)
        drawSelection(P, f);
}

void AbstractShape::drawIn(QPainter& P, double f)
{
    if (m_connectIn.has_value() == false)
        return;

    P.save();
    P.setPen(Qt::blue);
    P.setBrush(Qt::blue);

    auto rect = scale(f);
    auto pt = global(rect, m_connectIn.value().x(), m_connectIn.value().y());

    P.drawRect(pt.x() - m_cConnectorSize / 2, pt.y() - m_cConnectorSize / 2, m_cConnectorSize, m_cConnectorSize);
    P.restore();
}

void AbstractShape::drawOut(QPainter& P, double f)
{
    P.save();
    P.setPen(Qt::red);
    P.setBrush(Qt::red);

    auto rect = scale(f);

    for (const auto& relPt : m_vConnectsOut) {
        auto pt = global(rect, relPt.x(), relPt.y());
        P.drawRect(pt.x() - m_cConnectorSize / 2, pt.y() - m_cConnectorSize / 2, m_cConnectorSize, m_cConnectorSize);
    }

    P.restore();
}

void AbstractShape::drawSelection(QPainter& P, double f)
{
    P.save();
    P.setPen(Qt::black);
    P.setBrush(Qt::white);

    auto rect = scale(f);

    double wStep = 20.0;
    double hStep = 20.0;
    if (rect.width() < 2 * wStep)
        wStep = rect.width() / 2;
    if (rect.height() < 2 * hStep)
        hStep = (rect.height() - m_cSelectionSize) / 2;

    for (double x = wStep; x <= rect.width() - wStep; x += wStep) {
        P.drawRect(rect.left() + x, rect.top() - m_cSelectionSize, m_cSelectionSize, m_cSelectionSize);
        P.drawRect(rect.left() + x, rect.bottom(), m_cSelectionSize, m_cSelectionSize);
    }
    for (double y = hStep; y <= rect.height() - hStep; y += hStep) {
        P.drawRect(rect.left() - m_cSelectionSize, rect.top() + y, m_cSelectionSize, m_cSelectionSize);
        P.drawRect(rect.right(), rect.top() + y, m_cSelectionSize, m_cSelectionSize);
    }

    P.drawRect(rect.left() - m_cSelectionSize, rect.top() - m_cSelectionSize, m_cSelectionSize, m_cSelectionSize);
    P.drawRect(rect.right(), rect.top() - m_cSelectionSize, m_cSelectionSize, m_cSelectionSize);
    P.drawRect(rect.left() - m_cSelectionSize, rect.bottom(), m_cSelectionSize, m_cSelectionSize);
    P.drawRect(rect.right(), rect.bottom(), m_cSelectionSize, m_cSelectionSize);

    P.restore();
}

QRectF AbstractShape::area(QPointF pt, int size) const
{
    return QRectF(pt.x() - size / 2, pt.y() - size / 2, size, size);
}
