#pragma once

#include <QUndoCommand>

#include "../diagram.h"

namespace undo {

/**
 * @brief The ManipulateShape class This is base class for all shape manipulation operations like move and resize
 */
class ManipulateShape : public QUndoCommand
{
public:
    /**
     * @brief ManipulateShape Constructor
     * @param diagram Reference to the diagram
     * @param index shape index
     */
    ManipulateShape(data::Diagram& diagram, int index) : QUndoCommand(), m_diagram(diagram), m_index(index)
    {}

    /**
     * @brief diagram Returns reference to the diagram
     * @return reference to the diagram
     */
    data::Diagram& diagram() const { return m_diagram; }
    /**
     * @brief index Returns the index
     * @return shape index
     */
    int index() const { return m_index; }

protected:
    /**
     * @brief updateConnections Updates all the connections connected to indicated shape
     */
    void updateConnections();

private:
    data::Diagram& m_diagram;
    int m_index;
};

} // namespace
