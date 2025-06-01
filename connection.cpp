#include "connection.h"

#include <QDebug>
#include <QLineF>
#include <QtMath>

namespace data {

Connection::Connection()
  : m_out(nullptr)
  , m_outIndex(-1)
  , m_in(nullptr)
{}

Connection::Connection(AbstractShape* out, int outIndex, AbstractShape* in)
  : m_out(out)
  , m_outIndex(outIndex)
  , m_in(in)
{}

void Connection::setOut(AbstractShape* out, int index)
{
  m_out      = out;
  m_outIndex = index;
}

bool Connection::contains(AbstractShape* end) const
{
  return (m_out == end) || (m_in == end);
}

void Connection::addEnd(AbstractShape* end, int index)
{
  if (isOutput() == true) {
    m_in = end;
  } else if (isInput() == true) {
    m_out      = end;
    m_outIndex = index;
  }
}

void Connection::draw(QPainter& P, double f) const
{
  QPolygonF poly;

  poly << m_out->outputPos(m_outIndex, f);
  poly.append(m_vPoints);
  poly << m_in->inputPos(f);

  P.setPen(Qt::black);

  P.drawPolyline(poly);

  QPointF pt2 = poly.last();
  QPointF pt1 = poly.at(poly.count() - 2);

  auto ang = qAtan2(pt2.y() - pt1.y(), pt2.x() - pt1.x());

  QPolygonF polyArrow;
  polyArrow << pt2;
  pt1 = QPointF(pt2.x() + m_cArrowSize * qCos(ang + M_PI / 6),
                pt2.y() - m_cArrowSize * qSin(ang + M_PI / 6));
  polyArrow << pt1;
  pt1 = QPointF(pt2.x() + m_cArrowSize * qCos(ang - M_PI / 6),
                pt2.y() - m_cArrowSize * qSin(ang - M_PI / 6));
  polyArrow << pt1;

  P.setBrush(Qt::black);
  P.drawPolygon(polyArrow);

  if (isSelected() == false)
    return;

  P.setBrush(Qt::NoBrush);

  int sz = m_cSelectionSize;
  for (int i = 1; i < poly.count(); ++i) {
    QLineF line(poly.at(i - 1), poly.at(i));
    auto total = line.length();
    auto nv    = line.normalVector();
    nv.setLength(7.0);
    for (double d = total; d >= 0.0; d -= 20.0) {
      line.setLength(d);
      auto pt3 = line.p2() + QPointF(nv.dx(), nv.dy());
      P.drawRect(pt3.x() - sz / 2, pt3.y() - sz / 2, sz, sz);
      pt3 = line.p2() - QPointF(nv.dx(), nv.dy());
      P.drawRect(pt3.x() - sz / 2, pt3.y() - sz / 2, sz, sz);
    }
  }
}

void Connection::update(double f)
{
  m_vPoints.clear();
  auto pt1 = m_out->outputPos(m_outIndex, f);
  auto pt2 = m_in->inputPos(f);

  if (qFuzzyCompare(pt1.x(), pt2.x()) == false) {
    double y = (pt1.y() + pt2.y()) / 2.0;
    m_vPoints << QPointF(pt1.x(), y) << QPointF(pt2.x(), y);
  }

  m_vAreas.clear();
  QPolygonF poly;
  poly << pt1;
  poly.append(m_vPoints);
  poly << pt2;

  for (int i = 1; i < poly.count(); ++i) {
    pt1 = poly.at(i - 1);
    pt2 = poly.at(i);

    if (qFuzzyCompare(pt1.x(), pt2.x()) == true)
      m_vAreas.append(
        QRectF(pt1.x() - m_cInflateSize / 2, pt1.y(), m_cInflateSize, pt2.y() - pt1.y()));
    else
      m_vAreas.append(
        QRectF(pt1.x(), pt1.y() - m_cInflateSize / 2, pt2.x() - pt1.x(), m_cInflateSize));
  }
}

bool Connection::contains(QPointF pt) const
{
  for (const auto& area : m_vAreas)
    if (area.contains(pt) == true)
      return true;

  return false;
}

} // namespace data
