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
    void keyPressEvent(QKeyEvent* pKE) override;
    void paintEvent(QPaintEvent* pPE) override;
    void dragEnterEvent(QDragEnterEvent* pDEE) override;
    void dropEvent(QDropEvent* pDE) override;

private:
    Diagram m_diagram;
    std::optional<QPointF> m_drag;
    Connection m_conStart;
};
