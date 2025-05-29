#pragma once

#include "manipulateshape.h"

#include "undoid.h"
#include "../diagram.h"

namespace undo {

/**
 * @brief The ResizeShape class This class represents the shape resize command
 */
class ResizeShape : public ManipulateShape
{
public:
    /**
     * @brief ResizeShape Constructor
     * @param diagram Reference to the diagram
     * @param index Shape index
     * @param oldSize Old shape size
     * @param dX difference in X coordinate
     * @param dY difference in Y coordinate
     * @param dW difference in shape width
     * @param dH difference in shape height
     */
    ResizeShape(data::Diagram& diagram, int index, QRectF oldSize, double dX, double dY, double dW, double dH);
    /**
     * @brief id Returns the command id
     * @return command id
     */
    int id() const override { return ciResizeShape; }
    /**
     * @brief dX Returns the difference between old and new X coordinate
     * @return difference between old and new X coordinate
     */
    double dX() const { return m_dX; }
    /**
     * @brief dY Returns the difference between old and new Y coordinate
     * @return difference between old and new Y coordinate
     */
    double dY() const { return m_dY; }
    /**
     * @brief dW Returns the difference between old and new width
     * @return difference between old and new width
     */
    double dW() const { return m_dW; }
    /**
     * @brief dH Returns the difference between old and new height
     * @return difference between old and new height
     */
    double dH() const { return m_dH; }

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
    QRectF m_oldSize;
    double m_dX = 0.0;
    double m_dY = 0.0;
    double m_dW = 0.0;
    double m_dH = 0.0;
};

}

