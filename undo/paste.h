#pragma once

#pragma once

#include "switchselection.h"
#include "undoid.h"

#include "../diagram.h"
#include "../erroremitter.h"

namespace undo {

/**
 * @brief The Paste class This class represents the paste command
 */
class Paste : public SwitchSelection
{
public:
  /**
   * @brief Paste Constructor
   * @param diagram Reference to the diagram
   */
  Paste(data::Diagram& diagram, data::Diagram& copy)
    : SwitchSelection(diagram)
    , m_diagram(diagram)
    , m_copy(copy)
  {
    m_shapeIndex = m_diagram.shapes().size();
    m_conIndex   = m_diagram.connections().size();
  }
  /**
   * @brief id Returns the command id
   * @return command id
   */
  int id() const override { return ciPaste; }
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
  size_t m_shapeIndex;
  size_t m_conIndex;
  data::Diagram m_copy;
};

} // namespace undo
