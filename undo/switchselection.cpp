#include "switchselection.h"

#include <QDebug>

namespace undo {

SwitchSelection::SwitchSelection(data::Diagram& diagram) : QUndoCommand(), m_diagram(diagram)
{
    m_oldShapes = m_diagram.selectedShapes();
    m_oldCons = m_diagram.selectedConnections();
}

void SwitchSelection::recordSelections()
{
    m_newShapes = m_diagram.selectedShapes();
    m_newCons = m_diagram.selectedConnections();
}

void SwitchSelection::recordSelections(const std::unordered_set<int>& shapes, const std::unordered_set<int>& cons)
{
    m_newShapes = shapes;
    m_newCons = cons;
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

}
