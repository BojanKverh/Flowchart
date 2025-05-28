#include "resizeshape.h"

namespace undo {

ResizeShape::ResizeShape(data::Diagram& diagram, int index, QSizeF oldSize, QSizeF diffSize) :
    QUndoCommand(), m_diagram(diagram), m_index(index), m_oldSize(oldSize), m_diffSize(diffSize)
{}

void ResizeShape::undo()
{

}

void ResizeShape::redo()
{

}

bool ResizeShape::mergeWith(const QUndoCommand* com)
{

}

} // namespace
