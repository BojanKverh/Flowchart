#include "drawarea.h"

#include "dialogshape.h"
#include "shapes/shapefactory.h"

#include "undo/addconnection.h"
#include "undo/addshape.h"
#include "undo/deleteselected.h"
#include "undo/editproperties.h"
#include "undo/moveshape.h"
#include "undo/paste.h"
#include "undo/resizeshape.h"

#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPaintEvent>
#include <QPainter>

DrawArea::DrawArea(QWidget* parent)
  : QWidget(parent)
  , m_conStart(nullptr, -1, nullptr)
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

void DrawArea::paste(data::Diagram* copy)
{
  if (copy->shapes().size() == 0)
    return;

  auto* com = new undo::Paste(&m_emitter, m_diagram, *copy);
  m_diagram.addOperation(com);
  update();
}

void DrawArea::selectAll()
{
  auto* com = new undo::SwitchSelection(m_diagram);
  com->recordAll();
  m_diagram.addOperation(com);
  update();
}

void DrawArea::mousePressEvent(QMouseEvent* pME)
{
  if (pME->button() == Qt::LeftButton) {
    auto pt = pME->position();
    auto iS = m_diagram.findShape(pt, true);
    auto iC = (iS < 0 ? m_diagram.findConnection(pt) : -1);
    std::unordered_set<int> shapes;
    std::unordered_set<int> cons;
    if (iS >= 0)
      shapes.insert(iS);
    if (iC >= 0)
      cons.insert(iC);

    if ((m_diagram.isSelectionEqual(shapes, cons) == false)
        && (m_diagram.isShapeSelected(iS) == false)
        && (m_diagram.isConnectionSelected(iC) == false))
    {
      auto* com = new undo::SwitchSelection(m_diagram);
      com->recordSelections(shapes, cons);

      m_diagram.addOperation(com);
    }

    m_drag = pt;

    if (initConnect(iS, pt) == true) {
      m_mode = MoveMode::emmConnect;
    } else if (initResize(iS, pt) == true) {
      m_resize = iS;
      m_mode   = MoveMode::emmResize;
    } else if (initMove(iS) == true) {
      m_mode = MoveMode::emmMove;
    } else if (initSelect(pt) == true) {
      m_mode = MoveMode::emmSelect;
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

  switch (m_mode) {
    case MoveMode::emmMove: handleMove(pt); break;
    case MoveMode::emmResize: handleResize(pt); break;
    case MoveMode::emmSelect: handleSelect(pt); break;

    default: break;
  }

  if (m_mode != MoveMode::emmNone) {
    setCursor(Qt::CursorShape::ClosedHandCursor);
  } else {
    int index = m_diagram.findShape(pt);
    if (index < 0) {
      setCursor(Qt::CursorShape::ArrowCursor);
      return;
    }

    auto con = m_diagram.findConnector(index, pt);
    if (con.isEnd() == true) {
      setCursor(Qt::CursorShape::PointingHandCursor);
      return;
    }

    auto edge  = m_diagram.findEdge(index, pt);
    auto shape = data::EdgeHandler::shape(edge);
    setCursor(shape);
  }
}

void DrawArea::mouseReleaseEvent(QMouseEvent* pME)
{
  if (pME->button() == Qt::LeftButton) {
    auto pt = pME->position();

    switch (m_mode) {
      case DrawArea::MoveMode::emmConnect: finishConnect(pt); break;
      case DrawArea::MoveMode::emmSelect: finishSelect(pt); break;

      default: break;
    }
  }

  m_drag.reset();
  m_rectSelect.reset();
  m_mode   = MoveMode::emmNone;
  m_edge   = data::Edge::eNone;
  m_resize = -1;
  QWidget::mouseReleaseEvent(pME);
  update();
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
  P.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing
                   | QPainter::SmoothPixmapTransform);

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

  if (m_rectSelect.has_value() == true) {
    P.setPen(palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Text));
    auto color = palette().color(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight);
    color.setAlpha(64);
    P.setBrush(color);
    P.drawRect(m_rectSelect.value());
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
    } else {
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
    menu.addAction(tr("Edit properties"), [pt, this]() { editProperties(pt); });
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
    default: break;
  }

  if (errMsg.isEmpty() == false)
    QMessageBox::critical(this, tr("Shape adding failed"), errMsg);
}

bool DrawArea::initMove(int index) const
{
  return (index >= 0);
}

bool DrawArea::initConnect(int index, QPointF pt)
{
  m_conStart = m_diagram.findConnector(index, pt);
  return m_conStart.isEnd();
}

bool DrawArea::initResize(int index, QPointF pt)
{
  m_edge = m_diagram.findEdge(index, pt);
  return m_edge != data::Edge::eNone;
}

bool DrawArea::initSelect(QPointF pt)
{
  return true;
}

void DrawArea::handleMove(QPointF pt)
{
  auto set = m_diagram.selectedShapes();
  auto* com = new undo::MoveShape(m_diagram, set, pt - m_drag.value());
  m_diagram.addOperation(com);
  update();
  m_drag = pt;
}

void DrawArea::handleResize(QPointF pt)
{
  if (m_resize < 0)
    return;
  auto* shape = m_diagram.shapes().at(m_resize).get();

  double dX, dY, dW, dH;
  std::tie(dX, dY, dW, dH) = data::EdgeHandler::move(m_edge, m_drag.value(), pt);

  auto* com = new undo::ResizeShape(m_diagram, m_resize, QRectF(shape->position(), shape->size()),
                                    dX, dY, dW, dH);
  m_diagram.addOperation(com);
  update();
  m_drag = pt;
}

void DrawArea::handleSelect(QPointF pt)
{
  auto pt2 = m_drag.value();
  QRectF rect(qMin(pt.x(), pt2.x()), qMin(pt.y(), pt2.y()), qAbs(pt2.x() - pt.x()),
              qAbs(pt2.y() - pt.y()));
  m_rectSelect = rect;

  update();
}

void DrawArea::finishConnect(QPointF pt)
{
  auto index = m_diagram.findShape(pt, true);
  if (index < 0)
    return;

  if (m_conStart.isOutput() == true) {
    m_conStart.setIn(m_diagram.shapes().at(index).get());
  } else {
    auto con = m_diagram.findConnector(index, pt);
    m_conStart.setOut(con.out(), con.outIndex());
  }

  if (m_conStart.isValid() == false)
    return;

  auto* com = new undo::AddConnection(&m_emitter, m_diagram, m_diagram.findShape(m_conStart.out()),
                                      m_conStart.outIndex(), m_diagram.findShape(m_conStart.in()));
  m_diagram.addOperation(com);
  update();
  m_conStart = data::Connection();
}

void DrawArea::finishSelect(QPointF pt)
{
  if (m_rectSelect.has_value() == false)
    return;

  auto pt2 = m_drag.value();
  QRectF rect(qMin(pt.x(), pt2.x()), qMin(pt.y(), pt2.y()), qAbs(pt2.x() - pt.x()),
              qAbs(pt2.y() - pt.y()));

  std::unordered_set<int> shapes;
  std::unordered_set<int> cons;

  std::tie(shapes, cons) = m_diagram.inside(rect);
  auto* com              = new undo::SwitchSelection(m_diagram);
  com->recordSelections(shapes, cons);
  m_diagram.addOperation(com);
  update();
}
