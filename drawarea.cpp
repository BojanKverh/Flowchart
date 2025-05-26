#include "drawarea.h"

#include "shapes/shapefactory.h"
#include "dialogshape.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMimeData>
#include <QMessageBox>

DrawArea::DrawArea(QWidget* parent) : QWidget(parent), m_conStart(nullptr, -1, nullptr)
{
    resize(800, 600);
    setAcceptDrops(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void DrawArea::mousePressEvent(QMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton) {
        auto pt = pME->position();
        auto i = m_diagram.findShape(pt);
        m_drag = pt;
        m_diagram.selectShape(i);
        m_conStart = m_diagram.findConnector(m_drag.value());
        if (i >= 0) {
            m_diagram.selectConnection(-1);
        } else {
            i = m_diagram.findConnection(pt);
            m_diagram.selectConnection(i);
        }
        update();
    }
}

void DrawArea::mouseDoubleClickEvent(QMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton) {
        editProperties(pME->pos());
    }
}

void DrawArea::mouseMoveEvent(QMouseEvent* pME)
{
    auto pt = pME->position();

    if (m_drag.has_value() == true) {

        if (m_conStart.isEnd() == true) {

        } else {
            m_diagram.moveSelected(pt - m_drag.value());
        }
        update();

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
    if (pME->button() == Qt::LeftButton) {
        auto pt = pME->position();
        auto con = m_diagram.findConnector(pt);
        if ((con.isEnd() == false) && (m_conStart.isOutput() == true)) {
            int i = m_diagram.findShape(pt);
            if (i >= 0)
                con.setIn(m_diagram.shapes().at(i).get());
        }

        if ((m_conStart.isEnd() == true) && (con.isEnd() == true)) {
            m_conStart.addEnd(con.isOutput() == true? con.out() : con.in(), con.outIndex());
            m_conStart.update();
            m_conStart.setSelected(true);
            m_diagram.selectShape(-1);
            m_diagram.selectConnection(-1);
            m_diagram.addConnection(m_conStart);
            update();
        }

        m_drag.reset();
        m_conStart = data::Connection(nullptr, -1, nullptr);
    } else if (pME->button() == Qt::RightButton) {
        editProperties(pME->pos());
    }

    QWidget::mouseReleaseEvent(pME);
}

void DrawArea::keyPressEvent(QKeyEvent* pKE)
{
    if ((pKE->key() == Qt::Key_Delete) || (pKE->key() == Qt::Key_Backspace)) {
        m_diagram.deleteSelected();
        update();
    }
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
        con.draw(P);
    }

    const auto& vShapes = m_diagram.shapes();

    for (const auto& shape : vShapes) {
        shape->draw(P);
    }
}

void DrawArea::dragEnterEvent(QDragEnterEvent* pDEE)
{
    auto* data = pDEE->mimeData();
    if (data->property(m_cType.toLatin1()).isValid() == true)
        pDEE->acceptProposedAction();
}

void DrawArea::dropEvent(QDropEvent* pDE)
{
    auto data = pDE->mimeData();
    bool ok;
    auto shape = ShapeFactory::shape(data->property(m_cType.toLatin1()).toInt(&ok));
    if (ok == true && shape.get() != nullptr) {
        shape->moveCenter(pDE->position());
        auto err = m_diagram.addShape(std::move(shape));
        QString errMsg;
        switch (err) {
        case data::Diagram::Error::eStartExists:
              errMsg = tr("The diagram can only contain one START element!");
              break;
        case data::Diagram::Error::eEndExists:
              errMsg = tr("The diagram can only contain one END element!");
              break;
          default:
              break;
        }

        if (errMsg.isEmpty() == true) {
            update();
            m_diagram.selectConnection(-1);
            m_diagram.selectShape(m_diagram.shapes().size() - 1);
        } else
            QMessageBox::critical(this, tr("Shape adding failed"), errMsg);
    }
}

void DrawArea::editProperties(const QPointF& pt)
{
    auto index = m_diagram.findShape(pt);
    if (index >= 0) {
        m_diagram.selectShape(index);
        update();
        auto* shape = m_diagram.shapes()[index].get();
        DialogShape dlg(shape, this);
        if (dlg.exec() == true) {
            shape->setBackgroundColor(dlg.backgroundColor());
            shape->setTextColor(dlg.textColor());
            shape->setText(dlg.text());
        }
        return;
    }
    update();
}


