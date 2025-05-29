#pragma once

#include <QMainWindow>

#include "shapes/shapetype.h"
#include "lastfileshandler.h"

class QToolBar;
class QTabWidget;

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
    /**
     * @brief current Returns the current draw area
     * @return current area
     */
    DrawArea* current() const;

private:
    /**
     * @brief buildUI Builds the entire UI
     */
    void buildUI();
    /**
     * @brief buildControl Builds the shortcuts
     */
    void buildControl();

    /**
     * @brief addButton Adds the toolbutton to the toolbar
     * @param icon Filename containing the button icon
     * @param text Button text
     * @param shape Shape to be associated with the button
     */
    void addButton(QString icon, QString text, data::ShapeType shape);
    /**
     * @brief newDiagram Creates a new diagram
     */
    void newDiagram();
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
    /**
     * @brief updateLastFiles Updates the last files
     */
    void updateLastFiles();
    /**
     * @brief load Loads the file with given name
     * @param file File name to load
     */
    void load(const QString& file);
    /**
     * @brief undo Undoes the last command on the current draw area
     */
    void undo();
    /**
     * @brief redo Redoes the last command on the current draw area
     */
    void redo();
    /**
     * @brief newTab Opens a new tab
     */
    void newTab();
    /**
     * @brief updateName Updates the current tab name
     */
    void updateName();

private:
    QToolBar* m_ptb;
    QTabWidget* m_pCentral;
    LastFilesHandler* m_lastFiles;

    QMenu* m_fileMenu;
    QAction* m_lfStart;
    QAction* m_lfEnd;
};
