#pragma once

#include <vector>
#include <memory>

#include "shapes/abstractshape.h"
#include "connection.h"

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
     * @brief shapes Returns all the shapes
     * @return Reference to the vector containing all the shapes
     */
    const std::vector<std::unique_ptr<AbstractShape>>& shapes() const { return m_vShapes; }
    /**
     * @brief connections Returns all the connections
     * @return Reference to the vector containing all the connections
     */
    const std::vector<Connection> connections() const { return m_vConnections; }
    /**
     * @brief addShape Tries to append a new shape.
     * @param shape Pointer to the shape
     * @return true, if the shape was added and false otherwise
     */
    Error addShape(std::unique_ptr<AbstractShape> shape);
    /**
     * @brief indexOf Returns the index of given shape in the vector of shapes
     * @param shape Pointer to the shape to find
     * @return Index of the given shape in the vector of shapes. If no such shape
     * can be found, this method will return -1
     */
    int indexOf(AbstractShape* shape) const;
    /**
     * @brief addConnection Tries to append a new connection
     * @param con Reference to the connection
     * @return true, if the connection was added and false otherwise
     */
    Error addConnection(const Connection& con);
    /**
     * @brief selectShape Selects the n-th shape and deselects all the others
     * @param n Shape index
     */
    void selectShape(int n);
    /**
     * @brief selectConnection Selects the n-th connection and deselects all the others
     * @param n Connection index
     */
    void selectConnection(int n);
    /**
     * @brief findShape Finds the index of the shape, which contains the point pt
     * @param pt Point to look for
     * @return Index of the shape, which contains the point pt. If no such shape can be
     * found, -1 is returned
     */
    int findShape(QPointF pt) const;
    /**
     * @brief findConnection Finds the index of the connection, which contains point pt
     * @param pt Point to look for
     * @return Index of the connection, which contains the point pt. If no such connection
     * can be found, -1 is returned
     */
    int findConnection(QPointF pt) const;
    /**
     * @brief moveSelected Moves the selected shapes by given point
     * @param pt Vector by which the selected shapes will be moved
     */
    void moveSelected(QPointF pt);
    /**
     * @brief deleteSelected Deletes the selected shapes and connections
     */
    void deleteSelected();
    /**
     * @brief isOnConnector checks if point is on any connector
     * @param pt Point to check
     * @return object containing pointer to the output shape, output index and
     * pointer to the input shape. At least on of the returned pointers is nullptr.
     * If both are nullptr, means that the point is not on any connector
     */
    Connection findConnector(QPointF pt) const;

private:
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

private:
    std::vector<std::unique_ptr<AbstractShape>> m_vShapes;
    std::vector<Connection> m_vConnections;
};

} // namespace
