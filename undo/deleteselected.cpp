#include "deleteselected.h"

#include <QDebug>

namespace undo {

DeleteSelected::DeleteSelected(data::Diagram& diagram) : QUndoCommand(), m_diagram(diagram)
{
}

void DeleteSelected::undo()
{
    m_diagram.restoreShapes(m_shapes);
    m_diagram.restoreConnections(m_cons);

    m_shapes.clear();
    m_cons.clear();
}

void DeleteSelected::redo()
{
    std::tie(m_shapes, m_cons) = m_diagram.deleteSelected();
    qDebug() << "REDO" << m_shapes[0].get();
    setObsolete((m_shapes.empty() == true) && (m_cons.empty() == true));
}

} // namespace
