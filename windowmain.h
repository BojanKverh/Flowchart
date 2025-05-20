#pragma once

#include <QMainWindow>

#include "shapes/shapetype.h"

class QToolBar;

class ScrollArea;
class DrawArea;

class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
    WindowMain(QWidget *parent = nullptr);
    ~WindowMain();

private:
    void buildUI();

    void addButton(QString icon, QString text, ShapeType shape);

private:
    QToolBar* m_ptb;
    ScrollArea* m_pArea;
    DrawArea* m_pCanvas;
};
