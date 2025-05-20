#pragma once

#include <QScrollArea>

class ScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    ScrollArea(QWidget* pParent = nullptr);

protected:
    void resizeEvent(QResizeEvent* pRE);
};
