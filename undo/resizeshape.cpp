#include "resizeshape.h"

namespace undo {

ResizeShape::ResizeShape(data::Diagram& diagram, int index, QRectF oldSize, double dX, double dY, double dW, double dH) :
    ManipulateShape(diagram, index), m_oldSize(oldSize), m_dX(dX), m_dY(dY), m_dW(dW), m_dH(dH)
{}

void ResizeShape::undo()
{
    auto* shape = diagram().shapes().at(index()).get();
    shape->move(m_oldSize.topLeft());
    shape->resize(m_oldSize.size());
    updateConnections();
}

void ResizeShape::redo()
{
    auto* shape = diagram().shapes().at(index()).get();
    shape->move(m_oldSize.topLeft() + QPointF(m_dX, m_dY));
    shape->resize(m_oldSize.size() + QSizeF(m_dW, m_dH));
    updateConnections();
}

bool ResizeShape::mergeWith(const QUndoCommand* com)
{
    const auto* moveCom = dynamic_cast<const ResizeShape*>(com);
    if ((moveCom == nullptr) || (index() != moveCom->index()))
        return false;

    m_dX += moveCom->dX();
    m_dY += moveCom->dY();
    m_dW += moveCom->dW();
    m_dH += moveCom->dH();
    return true;
}

} // namespace
