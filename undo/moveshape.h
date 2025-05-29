#pragma once

#include "manipulateshape.h"

#include "undoid.h"

#include "../diagram.h"

namespace undo {

/**
 * @brief The MoveShape class This class represents the move shape command
 */
class MoveShape : public ManipulateShape
{
public:
    /**
     * @brief MoveShape Constructor
     * @param diagram Reference to the diagram
     * @param index Shape index
     * @param oldPos Old shape position
     * @param diffPos Difference to the new position
     */
    MoveShape(data::Diagram& diagram, int index, QPointF oldPos, QPointF diffPos);

    /**
     * @brief id Returns the command id
     * @return command id
     */
    int id() const override { return ciMoveShape; }

    /**
     * @brief pos Returns the difference between old and new point
     * @return difference between old and new point
     */
    QPointF diff() const { return m_diffPt; }

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
    QPointF m_oldPt;
    QPointF m_diffPt;
};

} // namespace
