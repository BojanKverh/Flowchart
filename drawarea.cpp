#include "drawarea.h"

#include "shapes/shapefactory.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMimeData>
#include <QMessageBox>

DrawArea::DrawArea(QWidget* parent) : QWidget(parent)
{
    resize(800, 600);
    setAcceptDrops(true);
    setMouseTracking(true);
}

void DrawArea::mousePressEvent(QMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton) {
        auto i = findShape(pME->position());
        m_drag = pME->position();
        m_diagram.select(i);
        update();
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent* pME)
{
    if (m_drag.has_value() == true) {
        auto pt = pME->position();
        m_diagram.moveSelected(pt - m_drag.value());
        update();
        m_drag = pt;
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent* pME)
{
    m_drag.reset();
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

    const auto& v = m_diagram.shapes();

    for (const auto& shape : v) {
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

int DrawArea::findShape(QPointF pt) const
{
    const auto& v = m_diagram.shapes();
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i]->contains(pt) == true)
            return i;

    return -1;
}
