#pragma once

#include <vector>
#include <unordered_set>
#include <memory>

#include "shapes/abstractshape.h"
#include "connection.h"
#include "edge.h"

#include <QUndoStack>

namespace data {
/**
 * @brief The Diagram class This class represents a diagram of shapes and connections
 */
class Diagram
{
public:
    enum class Error {
        eNone = 0,
        eStartExists = 1,
        eEndExists = 2
    };

public:
    /**
     * @brief Diagram Default constructor
     */
    Diagram();
    /**
     * @brief clear Clears all the diagram data
     */
    void clear();
    /**
     * @brief addOperation Adds an operation to the undo/redo stack
     * @param com Pointer to the operation
     */
    void addOperation(QUndoCommand* com);
    /**
     * @brief undo Undoes the last command
     */
    void undo();
    /**
     * @brief redo Redoes the last command
     */
    void redo();
    /**
     * @brief shapes Returns all the shapes
     * @return Read only reference to the vector containing all the shapes
     */
    const std::vector<std::unique_ptr<AbstractShape>>& shapes() const { return m_vShapes; }
    /**
     * @brief connections Returns all the connections
     * @return Read only reference to the vector containing all the connections
     */
    const std::vector<Connection>& connections() const { return m_vConnections; }
    /**
     * @brief shapes Returns all the shapes
     * @return Full access reference to the vector containing all the shapes
     */
    std::vector<std::unique_ptr<AbstractShape>>& shapes() { return m_vShapes; }
    /**
     * @brief connections Returns all the connections
     * @return Full access reference to the vector containing all the connections
     */
    std::vector<Connection>& connections() { return m_vConnections; }
    /**
     * @brief selectedShapes Returns the collection of selected shapes' indices
     * @return collection of selected shapes' indices
     */
    std::unordered_set<int> selectedShapes() const;
    /**
     * @brief selectedConnections Returns the collection of selected connections' indices
     * @return collection of selected connections' indices
     */
    std::unordered_set<int> selectedConnections() const;
    /**
     * @brief inside Returns indices of all the elements inside the given rectangle
     * @param rect Bounding rectangle
     * @return returns the tuple in format <shapesInsideIndices, connectionsInsideIndices>
     */
    std::tuple<std::unordered_set<int>, std::unordered_set<int>> inside(QRectF rect) const;
    /**
     * @brief indexOf Returns the index of given shape in the vector of shapes
     * @param shape Pointer to the shape to find
     * @return Index of the given shape in the vector of shapes. If no such shape
     * can be found, this method will return -1
     */
    int indexOf(AbstractShape* shape) const;
    /**
     * @brief findShape Finds the index of the shape, which contains the point pt
     * @param pt Point to look for
     * @param extended If this is true, the search rectangle will be extended by a threshold
     * @return Index of the shape, which contains the point pt. If no such shape can be
     * found, -1 is returned
     */
    int findShape(QPointF pt, bool extended = false) const;
    /**
     * @brief findEdge Finds the edge the given point is on the i-th shape
     * @param i index of the shape to check
     * @param pt Point to check
     * @return the edge found
     */
    data::Edge findEdge(int i, QPointF pt) const;
    /**
     * @brief findShape Returns index of the given shape. If no such shape can be found, it will return -1
     * @param shape Pointer to the shape
     * @return Index of the shape
     */
    int findShape(data::AbstractShape* shape) const;
    /**
     * @brief findConnection Finds the index of the connection, which contains point pt
     * @param pt Point to look for
     * @return Index of the connection, which contains the point pt. If no such connection
     * can be found, -1 is returned
     */
    int findConnection(QPointF pt) const;
    /**
     * @brief isOnConnector checks if point is on any connector
     * @param index shape to check
     * @param pt Point to check
     * @return object containing pointer to the output shape, output index and
     * pointer to the input shape. At least on of the returned pointers is nullptr.
     * If both are nullptr, means that the point is not on any connector
     */
    Connection findConnector(int index, QPointF pt) const;
    /**
     * @brief hasStart Return true, if the vector of shapes contains the Start shape
     * @return true, if the vector of shapes contains the Start shape and false otherwise
     */
    bool hasStart() const;
    /**
     * @brief hasEnd Return true, if the vector of shapes contains the End shape
     * @return true, if the vector of shapes contains the End shape and false otherwise
     */
    bool hasEnd() const;

    /**
     * @brief restoreShapes Restores the shapes from given vector
     * @param v Vector to restore shapes from
     */
    void restoreShapes(std::vector<std::unique_ptr<AbstractShape> > &v);
    /**
     * @brief addShape Tries to append a new shape. This method is supposed to be called by QUndoStack only
     * @param shape Pointer to the shape
     * @return true, if the shape was added and false otherwise
     */
    Error addShape(std::unique_ptr<AbstractShape> shape);
    /**
     * @brief removeShape Removes the i-th shape. This method is supposed to be called by QUndoStack only
     * @param i Shape index
     */
    void removeShape(int i);
    /**
     * @brief selectShape Selects the n-th shape and deselects all the others. This method is supposed to be
     * called by QUndoStack only
     * @param n Shape index
     */
    void selectShape(int n);
    /**
     * @brief restoreConnections Restores the connections
     * @param v New vector of connection
     */
    void restoreConnections(const std::vector<Connection> v);
    /**
     * @brief addConnection Tries to append a new connection. This method is supposed to be called by QUndoStack only
     * @param con Reference to the connection
     * @return true, if the connection was added and false otherwise
     */
    Error addConnection(const Connection& con);
    /**
     * @brief removeConnection Removes the i-th connection. This method is supposed to be called by QUndoStack only
     * @param i Connection index
     */
    void removeConnection(int i);
    /**
     * @brief selectConnection Selects the n-th connection and deselects all the others. This method is supposed to be
     * called by QUndoStack only
     * @param n Connection index
     */
    void selectConnection(int n);
    /**
     * @brief moveSelected Moves the selected shapes by given point. This method is supposed to be called by QUndoStack only
     * @param pt Vector by which the selected shapes will be moved
     */
    void moveSelected(QPointF pt);
    /**
     * @brief deleteSelected Deletes the selected shapes and connections. This method is supposed to be called by QUndoStack only
     */
    std::tuple<std::vector<std::unique_ptr<AbstractShape>>, std::vector<Connection>> deleteSelected();

private:
    std::vector<std::unique_ptr<AbstractShape>> m_vShapes;
    std::vector<Connection> m_vConnections;

    QUndoStack m_stack;
};

} // namespace
