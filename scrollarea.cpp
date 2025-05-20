#include "scrollarea.h"

#include <QResizeEvent>
#include <QDebug>

ScrollArea::ScrollArea(QWidget* pParent) : QScrollArea(pParent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(false);
}

void ScrollArea::resizeEvent(QResizeEvent* pRE)
{
    QScrollArea::resizeEvent(pRE);

    if (widget() == nullptr)
        return;

    int iW = qMax(pRE->size().width(), widget()->width());
    int iH = qMax(pRE->size().height(), widget()->height());

    widget()->resize(iW, iH);
}

