#include "drawarea.h"

#include "shapes/shapefactory.h"
#include "dialogshape.h"

#include "undo/addshape.h"
#include "undo/addconnection.h"
#include "undo/moveshape.h"
#include "undo/deleteselected.h"
#include "undo/editproperties.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMimeData>
#include <QMessageBox>
#include <QMenu>

DrawArea::DrawArea(QWidget* parent) : QWidget(parent), m_conStart(nullptr, -1, nullptr)
{
    resize(800, 600);
    setAcceptDrops(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DrawArea::customContextMenuRequested, this, &DrawArea::showContextMenu);
    connect(&m_emitter, &data::ErrorEmitter::diagramError, this, &DrawArea::handleError);
}

void DrawArea::undo()
{
    m_diagram.undo();
    update();
}

void DrawArea::redo()
{
    m_diagram.redo();
    update();
}

void DrawArea::mousePressEvent(QMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton) {
        auto pt = pME->position();
        auto iS = m_diagram.findShape(pt);
        auto iC = (iS < 0? m_diagram.findConnection(pt) : -1);

        m_drag = pt;
        m_conStart = m_diagram.findConnector(m_drag.value());

        auto* com = new undo::SwitchSelection(m_diagram);
        std::unordered_set<int> shapes;
        std::unordered_set<int> cons;

        if (iS >= 0)
            shapes.insert(iS);
        if (iC >= 0)
            cons.insert(iC);
        com->recordSelections(shapes, cons);

        m_diagram.addOperation(com);
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
            //m_diagram.moveSelected(pt - m_drag.value());
            auto set = m_diagram.selectedShapes();
            for (auto it = set.begin(); it != set.end(); ++it) {
                auto* com = new undo::MoveShape(m_diagram, *it, m_diagram.shapes()[*it]->position(), pt - m_drag.value());
                m_diagram.addOperation(com);
            }
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
            auto* com = new undo::AddConnection(
                &m_emitter, m_diagram, m_diagram.findShape(m_conStart.out()), m_conStart.outIndex(), m_diagram.findShape(m_conStart.in()));
            m_diagram.addOperation(com);
            update();
        }

        m_drag.reset();
        m_conStart = data::Connection(nullptr, -1, nullptr);
    }

    QWidget::mouseReleaseEvent(pME);
}

void DrawArea::keyPressEvent(QKeyEvent* pKE)
{
    if ((pKE->key() == Qt::Key_Delete) || (pKE->key() == Qt::Key_Backspace)) {
        m_diagram.addOperation(new undo::DeleteSelected(m_diagram));
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
    auto shape = data::ShapeFactory::shape(data->property(m_cType.toLatin1()).toInt(&ok));
    if (ok == true && shape.get() != nullptr) {
        auto* com = new undo::AddShape(&m_emitter, m_diagram, shape->type(), pDE->position());
        m_diagram.addOperation(com);
        update();
    }
}

void DrawArea::editProperties(const QPointF& pt)
{
    auto index = m_diagram.findShape(pt);
    if (index >= 0) {
        auto* com = new undo::EditProperties(m_diagram, index);
        m_diagram.selectShape(index);
        com->recordSelections();
        auto* shape = m_diagram.shapes()[index].get();
        DialogShape dlg(shape, this);
        if (dlg.exec() == true) {
            shape->setBackgroundColor(dlg.backgroundColor());
            shape->setTextColor(dlg.textColor());
            shape->setText(dlg.text());
            com->recordProperties();
            m_diagram.addOperation(com);
        }   else {
            delete com;
        }

        update();
        return;
    }
}

void DrawArea::showContextMenu(const QPoint& pt)
{
    auto index = m_diagram.findShape(pt);
    if (index >= 0) {
        m_diagram.selectShape(index);
        QMenu menu;
        menu.move(mapToGlobal(pt));
        menu.addAction(tr("Delete"), [index, this]() {
            m_diagram.addOperation(new undo::DeleteSelected(m_diagram));
        });
        menu.addAction(tr("Edit properties"), [pt, this]() {
            editProperties(pt);
        });
        menu.exec();
        return;
    }

    index = m_diagram.findConnection(pt);
    if (index >= 0) {
        m_diagram.selectConnection(index);
        QMenu menu;
        menu.move(mapToGlobal(pt));
        menu.addAction(tr("Delete"), [index, this]() {
            m_diagram.addOperation(new undo::DeleteSelected(m_diagram));
        });

        menu.exec();
    }
}

void DrawArea::handleError(data::Diagram::Error error)
{
    QString errMsg;

    switch (error) {
    case data::Diagram::Error::eStartExists:
        errMsg = tr("The diagram can only contain one START element!");
        break;
    case data::Diagram::Error::eEndExists:
        errMsg = tr("The diagram can only contain one END element!");
        break;
    default:
        break;
    }

    if (errMsg.isEmpty() == false)
        QMessageBox::critical(this, tr("Shape adding failed"), errMsg);
}
