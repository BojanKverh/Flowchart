#include "moveshape.h"

namespace undo {

MoveShape::MoveShape(data::Diagram& diagram, int index, QPointF oldPos, QPointF diffPos) :
    QUndoCommand(), m_diagram(diagram), m_index(index), m_oldPt(oldPos), m_diffPt(diffPos)
{}

void MoveShape::undo()
{
    auto* shape = m_diagram.shapes().at(m_index).get();
    shape->move(m_oldPt);
    auto& cons = m_diagram.connections();
    std::for_each(cons.begin(), cons.end(), [this, shape](auto& con)
    {
        if ((con.out() == shape) || (con.in() == shape))
            con.update();
    });
}

void MoveShape::redo()
{
    auto* shape = m_diagram.shapes().at(m_index).get();
    shape->move(m_oldPt + m_diffPt);
    auto& cons = m_diagram.connections();
    std::for_each(cons.begin(), cons.end(), [this, shape](auto& con)
    {
        if ((con.out() == shape) || (con.in() == shape))
            con.update();
    });
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
