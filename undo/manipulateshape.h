#pragma once

#include <QUndoCommand>

#include "../diagram.h"

namespace undo {

/**
 * @brief The ManipulateShape class This is base class for all shape manipulation operations like
 * move and resize
 */
class ManipulateShape : public QUndoCommand
{
public:
  /**
   * @brief ManipulateShape Constructor
   * @param diagram Reference to the diagram
   * @param shapes set of shape indices
   */
  ManipulateShape(data::Diagram& diagram, const std::unordered_set<int>& shapes)
      : QUndoCommand()
      , m_diagram(diagram)
      , m_shapes(shapes)
  {}

  /**
   * @brief diagram Returns reference to the diagram
   * @return reference to the diagram
   */
  data::Diagram& diagram() const { return m_diagram; }
  /**
   * @brief shapes Returns the index
   * @return set of shape indices
   */
  const std::unordered_set<int>& shapes() const { return m_shapes; }

protected:
  /**
   * @brief updateConnections Updates all the connections connected to indicated shape
   */
  void updateConnections();

private:
  data::Diagram& m_diagram;
  std::unordered_set<int> m_shapes;
};

} // namespace undo
