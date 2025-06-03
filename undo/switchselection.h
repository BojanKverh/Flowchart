#pragma once

#include <QUndoCommand>

#include "undoid.h"

#include <unordered_set>

#include "../diagram.h"

namespace undo {

/**
 * @brief The SwitchSelection class This is base class for all commands that switch selections
 */
class SwitchSelection : public QUndoCommand
{
public:
  /**
   * @brief SwitchSelection Constructor
   * @param diagram Reference to the diagram object
   */
  SwitchSelection(data::Diagram& diagram);
  /**
   * @brief id Returns the command id
   * @return command id
   */
  int id() const override { return ciSwitchSelection; }

  /**
   * @brief recordSelections Records the selected shapes and connections into m_newShapes and
   * m_newCons set
   */
  void recordSelections();
  /**
   * @brief recordSelections Records the selected shapes and connections
   * @param shapes Shapes to select
   * @param cons Connections to select
   */
  void recordSelections(const std::unordered_set<int>& shapes, const std::unordered_set<int>& cons);
  /**
   * @brief recordAll Records all shapes and connections as selected
   */
  void recordAll();

  /**
   * @brief undo Undoes the command
   */
  void undo() override;
  /**
   * @brief redo Redoes the command
   */
  void redo() override;

private:
  data::Diagram& m_diagram;
  std::unordered_set<int> m_oldShapes;
  std::unordered_set<int> m_oldCons;
  std::unordered_set<int> m_newShapes;
  std::unordered_set<int> m_newCons;
};

} // namespace undo
