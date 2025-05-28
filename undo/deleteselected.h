#pragma once

#include <QUndoCommand>

#include "../diagram.h"
#include "undoid.h"

namespace undo {
/**
 * @brief The DeleteSelected class This class represents the delete selected elements command
 */
class DeleteSelected : public QUndoCommand
{
public:
    /**
     * @brief DeleteSelected Constructor
     * @param diagram Reference to the diagram
     */
    DeleteSelected(data::Diagram& diagram);

    /**
     * @brief id Returns the command id
     * @return command id
     */
    int id() const override { return undo::ciDeleteSelected; }

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
    std::vector<std::unique_ptr<data::AbstractShape>> m_shapes;
    std::vector<data::Connection> m_cons;
};

}   // namespace
