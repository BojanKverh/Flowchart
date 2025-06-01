#include "editproperties.h"

#include <QDebug>

namespace undo {

EditProperties::EditProperties(data::Diagram& diagram, int index)
  : SwitchSelection(diagram)
  , m_diagram(diagram)
  , m_index(index)
{
  const auto& shape = m_diagram.shapes().at(m_index);

  m_oldBg   = shape->backgroundColor();
  m_oldFg   = shape->textColor();
  m_oldText = shape->text();
}

void EditProperties::recordProperties()
{
  auto& shape = m_diagram.shapes().at(m_index);
  m_newBg     = shape->backgroundColor();
  m_newFg     = shape->textColor();
  m_newText   = shape->text();
  setObsolete((m_oldBg == m_newBg) && (m_oldFg == m_newFg) && (m_oldText == m_newText));
}

void EditProperties::undo()
{
  auto& shape = m_diagram.shapes().at(m_index);
  shape->setBackgroundColor(m_oldBg);
  shape->setTextColor(m_oldFg);
  shape->setText(m_oldText);
  SwitchSelection::undo();
}

void EditProperties::redo()
{
  auto& shape = m_diagram.shapes().at(m_index);
  shape->setBackgroundColor(m_newBg);
  shape->setTextColor(m_newFg);
  shape->setText(m_newText);
  SwitchSelection::redo();
}

} // namespace undo
