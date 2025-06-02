#include "addshape.h"

#include "../shapes/shapefactory.h"

namespace undo {

void AddShape::undo()
{
  m_diagram.removeShape(m_index);
  SwitchSelection::undo();
}

void AddShape::redo()
{
  data::ShapeFactory factory;
  auto shape = factory.shape(static_cast<int>(m_type));
  shape->moveCenter(m_pt);
  auto err = m_diagram.addShape(std::move(shape));
  if (err != data::Diagram::Error::eNone) {
    emit m_emitter->diagramError(err);
    setObsolete(true);
  } else {
    m_diagram.selectConnection(-1);
    m_diagram.selectShape(m_index);
    recordSelections();
    SwitchSelection::redo();
  }
}

} // namespace undo
