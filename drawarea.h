#pragma once

#include "diagram.h"
#include "erroremitter.h"
#include "edge.h"

#include <QWidget>
#include <optional>

class DrawArea : public QWidget
{
    Q_OBJECT

    enum class MoveMode {
        emmNone,
        emmMove,
        emmConnect,
        emmResize
    };

public:
    /**
     * @brief DrawArea Constructor Default constructor
     * @param parent Pointer to the parent widget
     */
    DrawArea(QWidget* parent = nullptr);
    /**
     * @brief diagram Returns the read only reference to the diagram
     * @return read only reference to the diagram
     */
    const data::Diagram& diagram() const { return m_diagram; }
    /**
     * @brief diagram Returns the reference to the diagram
     * @return reference to the diagram
     */
    data::Diagram& diagram() { return m_diagram; }
    /**
     * @brief undo Undoes the last operation
     */
    void undo();
    /**
     * @brief redo Redoes the last operation
     */
    void redo();

protected:
    void mousePressEvent(QMouseEvent* pME) override;
    void mouseDoubleClickEvent(QMouseEvent* pME) override;
    void mouseMoveEvent(QMouseEvent* pME) override;
    void mouseReleaseEvent(QMouseEvent* pME) override;
    void keyPressEvent(QKeyEvent* pKE) override;
    void paintEvent(QPaintEvent* pPE) override;
    void dragEnterEvent(QDragEnterEvent* pDEE) override;
    void dropEvent(QDropEvent* pDE) override;

private:
    void editProperties(const QPointF& pt);
    void showContextMenu(const QPoint& pt);

    void handleError(data::Diagram::Error error);

    bool checkMove(int index) const;
    bool checkConnect(int index, QPointF pt);
    bool checkResize(int index, QPointF pt);

    void handleMove(QPointF pt);
    void handleConnect(QPointF pt);
    void handleResize(QPointF pt);

private:
    data::Diagram m_diagram;
    std::optional<QPointF> m_drag;
    data::Connection m_conStart;
    MoveMode m_mode = MoveMode::emmNone;
    data::Edge m_edge = data::Edge::eNone;
    int m_resize = -1;

    const QString m_cType = "type";
    data::ErrorEmitter m_emitter;
};
