#pragma once

#include <vector>
#include <memory>

#include "shapes/abstractshape.h"
#include "connection.h"

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
     * @brief addConnection Tries to append a new connection
     * @param con Reference to the connection
     * @return true, if the connection was added and false otherwise
     */
    Error addConnection(const Connection& con);
    /**
     * @brief select Selects the n-th shape and deselects all the others
     * @param n Shape index
     */
    void select(int n);
    /**
     * @brief findShape Finds the index of the shape, which contains the point pt
     * @param pt Point to look for
     * @return Index of the shape, which contains the point pt. If no such shape can be
     * found, -1 is returned
     */
    int findShape(QPointF pt) const;
    /**
     * @brief moveSelected Moves the selected shapes by given point
     * @param pt Vector by which the selected shapes will be moved
     */
    void moveSelected(QPointF pt);
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
