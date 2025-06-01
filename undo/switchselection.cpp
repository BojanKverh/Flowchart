#include "switchselection.h"

namespace undo {

SwitchSelection::SwitchSelection(data::Diagram& diagram)
  : QUndoCommand()
  , m_diagram(diagram)
{
  m_oldShapes = m_diagram.selectedShapes();
  m_oldCons   = m_diagram.selectedConnections();
}

void SwitchSelection::recordSelections()
{
  m_newShapes = m_diagram.selectedShapes();
  m_newCons   = m_diagram.selectedConnections();

  if (equal(m_oldShapes, m_newShapes) && equal(m_oldCons, m_newCons))
    setObsolete(true);
}

void SwitchSelection::recordSelections(const std::unordered_set<int>& shapes,
                                       const std::unordered_set<int>& cons)
{
  m_newShapes = shapes;
  m_newCons   = cons;

  if (equal(m_oldShapes, m_newShapes) && equal(m_oldCons, m_newCons))
    setObsolete(true);
}

void SwitchSelection::undo()
{
  auto& shapes = m_diagram.shapes();
  for (size_t i = 0; i < shapes.size(); ++i) {
    shapes[i]->setSelected(m_oldShapes.find(i) != m_oldShapes.end());
  }

  auto& cons = m_diagram.connections();
  for (size_t i = 0; i < cons.size(); ++i) {
    cons[i].setSelected(m_oldCons.find(i) != m_oldCons.end());
  }
}

void SwitchSelection::redo()
{
  auto& shapes = m_diagram.shapes();
  for (size_t i = 0; i < shapes.size(); ++i) {
    shapes[i]->setSelected(m_newShapes.find(i) != m_newShapes.end());
  }

  auto& cons = m_diagram.connections();
  for (size_t i = 0; i < cons.size(); ++i) {
    cons[i].setSelected(m_newCons.find(i) != m_newCons.end());
  }
}

bool SwitchSelection::equal(const std::unordered_set<int>& set1,
                            const std::unordered_set<int>& set2) const
{
  return (set1.size() == set2.size())
      && (std::all_of(set1.begin(), set1.end(),
                      [set2](const auto& elem) { return set2.find(elem) != set2.end(); }));
}

} // namespace undo
