#include "moveshape.h"

namespace undo {

MoveShape::MoveShape(data::Diagram& diagram, int index, QPointF oldPos, QPointF diffPos) :
    QUndoCommand(), m_diagram(diagram), m_index(index), m_oldPt(oldPos), m_diffPt(diffPos)
{}

void MoveShape::undo()
{
    m_diagram.shapes()[m_index]->move(m_oldPt);
}

void MoveShape::redo()
{
    m_diagram.shapes()[m_index]->move(m_oldPt + m_diffPt);
}

bool MoveShape::mergeWith(const QUndoCommand* com)
{
    const auto* moveCom = dynamic_cast<const MoveShape*>(com);
    if ((moveCom == nullptr) || (m_index != moveCom->index()))
        return false;

    m_diffPt += moveCom->diff();
    return true;
}

} // namespace
