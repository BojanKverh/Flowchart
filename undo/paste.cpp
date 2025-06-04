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
  auto n   = m_diagram.shapes().size();
  auto err = m_diagram.copySelected(m_copy);
  if (err != data::Diagram::Error::eNone) {
    emit m_emitter->diagramError(err);
    setObsolete(true);
  } else {
    auto& shapes = m_diagram.shapes();
    qDebug() << "OFFSET" << m_ptOffset << n << shapes.size() << m_ptOffset;
    for (size_t i = n; i < shapes.size(); ++i)
      shapes[i]->move(shapes[i]->position() + m_ptOffset);

    recordSelections();
    SwitchSelection::redo();
  }
}

} // namespace undo
