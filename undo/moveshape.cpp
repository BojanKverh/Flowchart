#include "moveshape.h"

namespace undo {

MoveShape::MoveShape(data::Diagram& diagram, int index, QPointF oldPos, QPointF diffPos) :
    ManipulateShape(diagram, index), m_oldPt(oldPos), m_diffPt(diffPos)
{}

void MoveShape::undo()
{
    diagram().shapes().at(index())->move(m_oldPt);
    updateConnections();
}

void MoveShape::redo()
{
    diagram().shapes().at(index())->move(m_oldPt + m_diffPt);
    updateConnections();
}

bool MoveShape::mergeWith(const QUndoCommand* com)
{
    const auto* moveCom = dynamic_cast<const MoveShape*>(com);
    if ((moveCom == nullptr) || (index() != moveCom->index()))
        return false;

    m_diffPt += moveCom->diff();
    return true;
}

} // namespace
