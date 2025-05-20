#include "drawarea.h"

#include "shapes/shapefactory.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMimeData>
#include <QMessageBox>

DrawArea::DrawArea(QWidget* parent) : QWidget(parent), m_conStart(nullptr, -1, nullptr)
{
    resize(800, 600);
    setAcceptDrops(true);
    setMouseTracking(true);
}

void DrawArea::mousePressEvent(QMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton) {
        auto i = m_diagram.findShape(pME->position());
        m_drag = pME->position();
        m_diagram.select(i);
        m_conStart = m_diagram.findConnector(m_drag.value());
        if (m_conStart.isEnd() == true)
            m_conStart.appendPoint(m_drag.value());
        update();
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent* pME)
{
    auto pt = pME->position();

    if (m_drag.has_value() == true) {

        if (m_connectionEnd.has_value() == true) {

        } else {
            m_diagram.moveSelected(pt - m_drag.value());
            update();
        }

        m_drag = pt;
        return;
    }

    auto con = m_diagram.findConnector(pt);
    if (con.isEnd() == true)
        setCursor(Qt::CursorShape::PointingHandCursor);
    else
        setCursor(Qt::CursorShape::ArrowCursor);
}

void DrawArea::mouseReleaseEvent(QMouseEvent* pME)
{
    auto pt = pME->position();
    auto con = m_diagram.findConnector(pt);
    if ((m_conStart.isEnd() == true) && (con.isEnd() == true)) {
        m_conStart.appendPoint(pt);
        m_diagram.addConnection(m_conStart);
        update();
    }

    m_drag.reset();
    m_connectionEnd.reset();
    m_conStart = Connection(nullptr, -1, nullptr);
}

void DrawArea::paintEvent(QPaintEvent* pPE)
{
    QPainter P(this);
    P.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    int iW = width();
    int iH = height();

    P.fillRect(0, 0, iW, iH, Qt::white);

    QColor clr(224, 224, 224);
    P.setPen(clr);
    P.setBrush(clr);

    for (int iY = 0; iY < iH; iY += 10)
        for (int iX = 0; iX < iW; iX += 10)
            P.drawEllipse(QPoint(iX, iY), 1, 1);

    const auto& vCons = m_diagram.connections();

    P.setPen(Qt::black);
    for (const auto& con : vCons) {
        auto pts = con.points();
        P.drawPolyline(pts);
    }

    const auto& vShapes = m_diagram.shapes();

    for (const auto& shape : vShapes) {
        shape->draw(P);
    }
}

void DrawArea::dragEnterEvent(QDragEnterEvent* pDEE)
{
    auto* data = pDEE->mimeData();
    if (data->property("type").isValid() == true)
        pDEE->acceptProposedAction();
}

void DrawArea::dropEvent(QDropEvent* pDE)
{
    auto data = pDE->mimeData();
    bool ok;
    auto shape = ShapeFactory::shape(data->property("type").toInt(&ok));
    if (ok == true && shape.get() != nullptr) {
        shape->moveCenter(pDE->position());
        auto err = m_diagram.addShape(std::move(shape));
        QString errMsg;
        switch (err) {
          case Diagram::Error::eStartExists:
              errMsg = tr("The diagram can only contain one START element!");
              break;
          case Diagram::Error::eEndExists:
              errMsg = tr("The diagram can only contain one END element!");
              break;
          default:
              break;
        }

        if (errMsg.isEmpty() == true)
            update();
        else
            QMessageBox::critical(this, tr("Shape adding failed"), errMsg);
    }
}

