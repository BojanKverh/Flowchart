#pragma once

#include <QMainWindow>

#include "shapes/shapetype.h"
#include "lastfileshandler.h"

class QToolBar;
class QTabWidget;

class ScrollArea;
class DrawArea;

namespace data {
class Diagram;
}

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
     * @brief save Saves the diagram content, using current diagram name. If the name is not set,
     * it will ask for it by calling saveAs()
     */
    void save();
    /**
     * @brief saveAs Saves the diagram content, asking the user for the file name
     */
    void saveAs();
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
     * @brief copy Copies the content of the current diagram
     */
    void copy();
    /**
     * @brief paste Pastes the copied content into the current diagram
     */
    void paste();
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

    std::unique_ptr<data::Diagram> m_copy;
};
