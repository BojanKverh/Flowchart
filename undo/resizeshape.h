#pragma once

#include "undoid.h"

#include "../diagram.h"

namespace undo {

/**
 * @brief The ResizeShape class This class represents the shape resize command
 */
class ResizeShape : public QUndoCommand
{
public:
    ResizeShape(data::Diagram& diagram, int index, QSizeF oldSize, QSizeF diffSize);
    /**
     * @brief id Returns the command id
     * @return command id
     */
    int id() const override { return ciResizeShape; }
    /**
     * @brief undo Undoes the command
     */
    void undo() override;
    /**
     * @brief redo Redoes the command
     */
    void redo() override;
    /**
     * @brief mergeWith Checks if this can be merged with given command
     * @param com Command to check
     * @return true, if commands can be merged and false otherwise
     */
    bool mergeWith(const QUndoCommand* com) override;

private:
    data::Diagram& m_diagram;
    int m_index;
    QSizeF m_oldSize;
    QSizeF m_diffSize;
};

}

