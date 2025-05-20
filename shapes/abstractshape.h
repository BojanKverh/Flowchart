#pragma once

#include <QPointF>
#include <QColor>
#include <QSizeF>
#include <QPainter>

#include <optional>

#include "shapetype.h"

/**
 * @brief The AbstractShape class This class is a base class for all shapes
 */
class AbstractShape
{
public:
    /**
     * @brief AbstractShape Default constructor
     */
    AbstractShape() {}
    /**
     * @brief type Returns the shape type.
     * @return shape type. Do not forget to set it in the derived class constructor
     */
    ShapeType type() const { return m_type; }
    /**
     * @brief isSelected Returns true, if the shape is selected and false otherwise
     * @return true, if the shape is selected and false otherwise
     */
    bool isSelected() const { return m_selected; }
    /**
     * @brief position Returns the coordinates of the top left corner
     * @return coordinates of the top left corner
     */
    QPointF position() const { return m_pt; }
    /**
     * @brief size Returns the shape size
     * @return the shape size (bounding rectangle)
     */
    QSizeF size() const { return m_size; }
    /**
     * @brief backgroundColor Returns the background color
     * @return background color
     */
    QColor backgroundColor() const { return m_bg; }
    /**
     * @brief textColor Returns the text color
     * @return text and outline color
     */
    QColor textColor() const { return m_fg; }
    /**
     * @brief text Returns the text
     * @return displayed text
     */
    QString text() const { return m_text; }
    /**
     * @brief setSelected Sets the selected flag
     * @param selected New value for selected flag
     */
    void setSelected(bool selected) { m_selected = selected; }
    /**
     * @brief move Moves the top left corner of the shape to pt
     * @param pt New top left corner position
     */
    void move(QPointF pt) { m_pt = pt; }
    /**
     * @brief moveCenter Moves the center of the shape to pt
     * @param pt New center position of pt
     */
    void moveCenter(QPointF pt);
    /**
     * @brief resize Resizes the shape
     * @param size New shape size
     */
    void resize(QSizeF size) { m_size = size; }
    /**
     * @brief setBackgroundColor Sets the background color
     * @param color New background color
     */
    void setBackgroundColor(QColor color) { m_bg = color; }
    /**
     * @brief setTextColor Sets the text color
     * @param color New text color
     */
    void setTextColor(QColor color) { m_fg = color; }
    /**
     * @brief setText Sets the displayed text
     * @param text New displayed text
     */
    void setText(const QString& text) { m_text = text; }
    /**
     * @brief draw Reimplement this method to draw the shape onto canvas
     * @param P Reference to the painter object
     * @param f Resize scale
     */
    virtual void draw(QPainter& P, double f = 1.0) = 0;
    /**
     * @brief connectInPoint Returns the point, where a connection goes into the shape
     * @return Point, where a connection goes into the shape. The point coordinates are defined
     * as percentage of the shape size
     */
    std::optional<QPointF> connectInPoint() const { return m_connectIn; }
    /**
     * @brief connectOutPoints Returns the vector of points, where connection goes out of the shape
     * @return vector of points, where connection goes out of the shape. The points coordinates
     * are defined as percentage of the shape size
     */
    const QVector<QPointF> connectOutPoints() const { return m_vConnectsOut; }
    /**
     * @brief contains Checks if this shape contains point pt
     * @param pt Point to check for
     * @param f Scale factor
     * @return true, if the shape contains pt and false otherwise
     */
    bool contains(QPointF pt, double f = 1.0) const;
    /**
     * @brief isOnInput Returns true, if the point is on input connector
     * @param pt Point to check
     * @param f scale factor
     * @return true, if the point is on input connector and false otherwise
     */
    bool isOnInput(QPointF pt, double f = 1.0) const;
    /**
     * @brief findOutput Returns the index of output connector on which the point is located.
     * @param pt Point to check
     * @param f scale factor
     * @return index of output connector on which the point is located. If no such output connector can be found, -1 is returned
     */
    int findOutput(QPointF pt, double f = 1.0) const;

protected:
    /**
     * @brief scale Scales the shape by factor f
     * @param f Scale factor
     * @return Scaled rectangle for the shape
     */
    QRectF scale(double f) const;
    /**
     * @brief global Converts the point from relative in-shape coordinates into global coordinates
     * @param rect Bounding rectangle in global coordinates
     * @param x relative X coordinate of the position to convert. Should be between 0 and 1
     * @param y relative Y coordinate of the position to convert. Should be between 0 and 1
     * @return Point in global coordinates
     */
    QPointF global(const QRectF& rect, double x, double y) const;
    /**
     * @brief drawDecorations Draws the decorations (in/out connectors, selection)
     * @param P Reference to the painter object
     * @param f Resize scale
     */
    void drawDecorations(QPainter& P, double f = 1.0);
    /**
     * @brief drawIn Draws the in connector for the shape
     * @param P Reference to the painter object
     * @param f Resize scale
     */
    virtual void drawIn(QPainter& P, double f = 1.0);
    /**
     * @brief drawOut Draws the out connectors for the shape
     * @param P Reference to the painter object
     * @param f Resize scale
     */
    virtual void drawOut(QPainter& P, double f = 1.0);
    /**
     * @brief drawSelection Draws the selection around the shape
     * @param P Reference to the pointer
     * @param f Resize scale
     */
    virtual void drawSelection(QPainter& P, double f = 1.0);
    /**
     * @brief area Returns the rectnagle with center at pt and given size
     * @param pt Central point
     * @param size Rectangle size
     * @return Calculated rectangle
     */
    QRectF area(QPointF pt, int size) const;

protected:
    bool m_selected = false;
    QPointF m_pt = QPointF(0, 0);
    QSizeF m_size = QSizeF(100, 30);
    QColor m_fg = Qt::black;
    QColor m_bg = Qt::white;
    QString m_text;
    ShapeType m_type;

    std::optional<QPointF> m_connectIn;
    QVector<QPointF> m_vConnectsOut;

    const int m_cConnectorSize = 7;
    const int m_cSelectionSize = 5;
};
