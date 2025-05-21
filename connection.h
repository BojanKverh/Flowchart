#pragma once

#include "shapes/abstractshape.h"

#include <QPainter>

/**
 * @brief The Connection class This class holds the connection between two shapes
 */
class Connection
{
public:
    /**
     * @brief Connection Constructor
     * @param out Pointer to the output shape
     * @param outIndex Index of the shape output
     * @param in Pointer to the input shape
     */
    Connection(AbstractShape* out, int outIndex, AbstractShape* in);
    /**
     * @brief isSelected Returns true, if the connection is selected and false otherwise
     * @return true, if the connection is selected and false otherwise
     */
    bool isSelected() const { return m_bSelected; }
    /**
     * @brief out Returns pointer to the shape containing output connector
     * @return pointer to the shape containing output connector
     */
    AbstractShape* out() const { return m_out; }
    /**
     * @brief outIndex Returns the output connector index
     * @return output connector index
     */
    int outIndex() const { return m_outIndex; }
    /**
     * @brief in Returns pointer to the shape containing input connector
     * @return pointer to the shape containing input connector
     */
    AbstractShape* in() const { return m_in; }
    /**
     * @brief setIn Sets the shape for input connector
     * @param in New input connector shape
     */
    void setIn(AbstractShape* in) { m_in = in; }
    /**
     * @brief contains Returns true, if the connection contains the given shape
     * @param end Shape to check
     * @return true, if the connection connects the given shape to another shape and
     * false otherwise
     */
    bool contains(AbstractShape* end) const;
    /**
     * @brief isValid Returns true, if the connection is valid. The connection is
     * valid when both connectors are defined
     * @return true, if connection is valid and false otherwise
     */
    bool isValid() const { return (m_out != nullptr) && (m_in != nullptr); }
    /**
     * @brief isEnd Returns true, if at least one connector is defined
     * @return true, if at least one connector is defined
     */
    bool isEnd() const { return (m_out != nullptr) || (m_in != nullptr); }
    /**
     * @brief isOutput Returns true, if output connector is defined and input is not
     * @return true, if output connector is defined and input is not
     */
    bool isOutput() const { return (m_out != nullptr) && (m_in == nullptr); }
    /**
     * @brief isInput Returns true, if input connector is defined and output is not
     * @return true, if input connector is defined and output is not
     */
    bool isInput() const { return (m_out == nullptr) && (m_in != nullptr); }
    /**
     * @brief points Returns the vector of points. with which the connector is drawn
     * @return vector of points. with which the connector is drawn
     */
    const QPolygonF& points() const { return m_vPoints; }
    /**
     * @brief setSelected Sets the selected flag for the connection
     * @param selected New value of the selected flag
     */
    void setSelected(bool selected) { m_bSelected = selected; }
    /**
     * @brief appendPoint Appends the point to the vector
     * @param pt Point to append
     */
    void appendPoint(QPointF pt) { m_vPoints.push_back(pt); }
    /**
     * @brief addEnd Adds the missing connector to the connection. If both ends
     * are already present, nothing will happen
     * @param end Connector to add
     * @param index Optional output connector index
     */
    void addEnd(AbstractShape* end, int index = -1);
    /**
     * @brief draw Draws the connection using the painter
     * @param P reference to the painter object
     * @param f scale factor
     */
    void draw(QPainter& P, double f = 1.0) const;
    /**
     * @brief update Updates connection points
     * @param f scale factor
     */
    void update(double f = 1.0);

    /**
     * @brief contains Returns true, if given point is contained in this connection
     * @param pt Point to check
     * @return true, if connection contains the given point and false otherwise
     */
    bool contains(QPointF pt) const;

private:
    bool m_bSelected = false;
    AbstractShape* m_out;
    int m_outIndex;
    AbstractShape* m_in;
    QPolygonF m_vPoints;
    QVector<QRectF> m_vAreas;

    static constexpr int m_cArrowSize = 10;
    static constexpr int m_cSelectionSize = 5;
    static constexpr int m_cInflateSize = 10;
};
