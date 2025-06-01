#include "paste.h"

#include <QDebug>

namespace undo {

void Paste::undo()
{
  auto& shapes = m_diagram.shapes();
  shapes.erase(shapes.begin() + m_shapeIndex, shapes.end());
  auto& cons = m_diagram.connections();
  cons.erase(cons.begin() + m_conIndex, cons.end());
  SwitchSelection::undo();
}

void Paste::redo()
{
  m_diagram.selectShape(-1);
  m_diagram.selectConnection(-1);
  m_diagram.copySelected(m_copy);
  recordSelections();
  SwitchSelection::redo();
}

} // namespace undo
