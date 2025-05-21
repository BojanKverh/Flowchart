#pragma once

#include <QMainWindow>

#include "shapes/shapetype.h"

class QToolBar;

class ScrollArea;
class DrawArea;

/**
 * @brief The WindowMain class This is the application main window
 */
class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief WindowMain Default constructor
     * @param parent pointer to the parent widget
     */
    WindowMain(QWidget *parent = nullptr);
    /**
     * @brief ~WindowMain Destructor
     */
    virtual ~WindowMain();

private:
    /**
     * @brief buildUI Builds the entire UI
     */
    void buildUI();

    /**
     * @brief addButton Adds the toolbutton to the toolbar
     * @param icon Filename containing the button icon
     * @param text Button text
     * @param shape Shape to be associated with the button
     */
    void addButton(QString icon, QString text, data::ShapeType shape);

    /**
     * @brief load Asks the user to select a file to load and loads it
     */
    void load();
    /**
     * @brief save Saves the diagram content
     */
    void save();
    /**
     * @brief quit Quits the application
     */
    void quit();

private:
    QToolBar* m_ptb;
    ScrollArea* m_pArea;
    DrawArea* m_pCanvas;
};
