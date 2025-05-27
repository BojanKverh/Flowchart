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

signals:
    /**
     * @brief signalEditProperties Emitted when there is a request to edit element properties
     * @param pt Point of click
     */
    void signalEditProperties(const QPointF& pt);

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

private:
    data::Diagram m_diagram;
    std::optional<QPointF> m_drag;
    data::Connection m_conStart;

    const QString m_cType = "type";
};
