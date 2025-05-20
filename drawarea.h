#pragma once

#include "diagram.h"

#include <QWidget>
#include <optional>

class DrawArea : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief DrawArea Constructor Default constructor
     * @param parent Pointer to the parent widget
     */
    DrawArea(QWidget* parent = nullptr);


protected:
    void mousePressEvent(QMouseEvent* pME) override;
    void mouseMoveEvent(QMouseEvent* pME) override;
    void mouseReleaseEvent(QMouseEvent* pME) override;
    void paintEvent(QPaintEvent* pPE) override;
    void dragEnterEvent(QDragEnterEvent* pDEE) override;
    void dropEvent(QDropEvent* pDE) override;

private:
    /**
     * @brief findShape Finds the index of the shape, which contains the point pt
     * @param pt Point to look for
     * @return Index of the shape, which contains the point pt. If no such shape can be
     * found, -1 is returned
     */
    int findShape(QPointF pt) const;

private:
    Diagram m_diagram;
    std::optional<QPointF> m_drag;
};
