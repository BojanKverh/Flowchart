#include "addconnection.h"

namespace undo {

void AddConnection::undo()
{
    m_diagram.removeConnection(m_index);
    SwitchSelection::undo();
}

void AddConnection::redo()
{
    auto* out = m_diagram.shapes()[m_out].get();
    auto* in = m_diagram.shapes()[m_in].get();
    data::Connection con(out, m_outIndex, in);
    con.update();
    con.setSelected(true);
    m_diagram.selectConnection(-1);
    m_diagram.selectShape(-1);
    m_diagram.addConnection(con);

    recordSelections();
    SwitchSelection::redo();
}

} // namespace
