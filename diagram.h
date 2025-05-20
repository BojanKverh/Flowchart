#pragma once

#include <vector>
#include <memory>

#include "shapes/abstractshape.h"

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
     * @brief addShape Appends the shape. This
     * @param shape Pointer to the shape
     * @return true, if the shape was added and false otherwise
     */
    Error addShape(std::unique_ptr<AbstractShape> shape);
    /**
     * @brief select Selects the n-th shape and deselects all the others
     * @param n Shape index
     */
    void select(int n);
    /**
     * @brief moveSelected Moves the selected shapes by given point
     * @param pt Vector by which the selected shapes will be moved
     */
    void moveSelected(QPointF pt);

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
};
