#include "resizeshape.h"

#include "../utils/setcompare.h"

namespace undo {

ResizeShape::ResizeShape(
    data::Diagram& diagram, int index, QRectF oldSize, double dX, double dY, double dW, double dH)
    : ManipulateShape(diagram, {index})
    , m_oldSize(oldSize)
    , m_dX(dX)
    , m_dY(dY)
    , m_dW(dW)
    , m_dH(dH)
{}

void ResizeShape::undo()
{
  for (auto index : shapes()) {
    auto* shape = diagram().shapes().at(index).get();
    shape->move(m_oldSize.topLeft());
    shape->resize(m_oldSize.size());
  }
  updateConnections();
}

void ResizeShape::redo()
{
  for (auto index : shapes()) {
    auto* shape = diagram().shapes().at(index).get();
    shape->move(m_oldSize.topLeft() + QPointF(m_dX, m_dY));
    shape->resize(m_oldSize.size() + QSizeF(m_dW, m_dH));
  }
  updateConnections();
}

bool ResizeShape::mergeWith(const QUndoCommand* com)
{
  const auto* resizeCom = dynamic_cast<const ResizeShape*>(com);
  if ((resizeCom == nullptr) || (utils::SetCompare::equal(shapes(), resizeCom->shapes()) == false))
    return false;

  m_dX += resizeCom->dX();
  m_dY += resizeCom->dY();
  m_dW += resizeCom->dW();
  m_dH += resizeCom->dH();
  return true;
}

} // namespace undo
