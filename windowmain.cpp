#include "windowmain.h"

#include <QToolBar>
#include <QToolButton>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>
#include <QShortcut>
#include <QLabel>
#include <QWidgetAction>

#include "scrollarea.h"
#include "drawarea.h"
#include "json/diagram.h"

WindowMain::WindowMain(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 600);

    buildUI();
    buildControl();
}

WindowMain::~WindowMain() {}

void WindowMain::buildUI()
{
    m_lastFiles = new LastFilesHandler(10);
    connect(m_lastFiles, &LastFilesHandler::filesChanged, this, &WindowMain::updateLastFiles);

    m_ptb = new QToolBar;
    m_ptb->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addButton(":/res/icons/start.png", tr("Start"), data::ShapeType::esStart);
    addButton(":/res/icons/end.png", tr("End"), data::ShapeType::esEnd);
    addButton(":/res/icons/io.png", tr("Input/Output"), data::ShapeType::esIO);
    addButton(":/res/icons/process.png", tr("Process"), data::ShapeType::esProcess);
    addButton(":/res/icons/decision.png", tr("Decision"), data::ShapeType::esDecision);

    addToolBar(m_ptb);

    m_pArea = new ScrollArea;

    m_pCanvas = new DrawArea;
    m_pArea->setWidget(m_pCanvas);

    setCentralWidget(m_pArea);

    auto* menuBar = new QMenuBar;
    m_fileMenu = new QMenu(tr("File"));
    m_fileMenu->addAction(tr("New (Ctrl+N"), this, &WindowMain::newDiagram);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(tr("Load (Ctrl+O)"), this, qOverload<>(&WindowMain::load));
    m_fileMenu->addAction(tr("Save (Ctrl+S)"), this, &WindowMain::save);
    m_lfStart = m_fileMenu->addSeparator();
    m_lfEnd = m_fileMenu->addSeparator();
    m_fileMenu->addAction(tr("Quit (Ctrl+Q)"), this, &WindowMain::quit);
    menuBar->addMenu(m_fileMenu);

    auto* pMenu = new QMenu(tr("Edit"));
    pMenu->addAction(tr("Undo (Ctrl+Z)"), m_pCanvas, &DrawArea::undo);
    pMenu->addAction(tr("Redo (Ctrl+Y)"), m_pCanvas, &DrawArea::redo);
    menuBar->addMenu(pMenu);

    setMenuBar(menuBar);

    updateLastFiles();
}

void WindowMain::buildControl()
{
    auto* sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
    sc->setContext(Qt::ApplicationShortcut);
    connect(sc, &QShortcut::activated, m_pCanvas, &DrawArea::undo);

    sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
    sc->setContext(Qt::ApplicationShortcut);
    connect(sc, &QShortcut::activated, m_pCanvas, &DrawArea::redo);

    sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);
    sc->setContext(Qt::ApplicationShortcut);
    connect(sc, &QShortcut::activated, this, qOverload<>(&WindowMain::load));

    sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    sc->setContext(Qt::ApplicationShortcut);
    connect(sc, &QShortcut::activated, this, &WindowMain::save);

    sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
    sc->setContext(Qt::ApplicationShortcut);
    connect(sc, &QShortcut::activated, this, &WindowMain::quit);
}

void WindowMain::addButton(QString icon, QString text, data::ShapeType shape)
{
    auto* ptb = new QToolButton;
    ptb->setIcon(QIcon(icon));
    ptb->setText(text);
    ptb->setAcceptDrops(false);
    ptb->setMouseTracking(true);
    ptb->setCheckable(false);

    connect(ptb, &QToolButton::pressed, [ptb, text, shape]() {
        ptb->setDown(false);
        auto* data = new QMimeData;
        data->setText(text);
        data->setProperty("type", static_cast<int>(shape));
        auto* drag = new QDrag(ptb);
        drag->setMimeData(data);
        drag->setPixmap(ptb->icon().pixmap(32, 32));
        drag->exec(Qt::CopyAction);
    });

    m_ptb->addWidget(ptb);
}

void WindowMain::newDiagram()
{
    m_pCanvas->diagram().clear();
    m_pCanvas->update();
}

void WindowMain::load()
{
    auto file = QFileDialog::getOpenFileName(this, tr("Select file to load from"), ".", "*.fcd");
    if (file.isEmpty() == true)
        return;

    load(file);
}

void WindowMain::save()
{
    auto file = QFileDialog::getSaveFileName(this, tr("Select file to save into"), ".", "*.fcd");
    if (file.isEmpty() == true)
        return;

    json::Diagram diagram(m_pCanvas->diagram(), m_pCanvas->width(), m_pCanvas->height());
    QFile f(file);
    if (f.open(QFile::WriteOnly) == false)
        return;

    QTextStream ts(&f);
    f.write(diagram.toJson().toJson());
    f.close();
    m_lastFiles->recordFile(file);
}

void WindowMain::quit()
{
    QApplication::instance()->quit();
}

void WindowMain::updateLastFiles()
{
    auto list = m_fileMenu->actions();
    bool toDelete = false;
    for (int i = 0; i < list.count(); ++i) {
        if (list[i] == m_lfStart) {
            toDelete = true;
        }   else if (list[i] == m_lfEnd) {
            toDelete = false;
        }   else if (toDelete == true) {
            m_fileMenu->removeAction(list[i]);
            delete list[i];
        }
    }

    QWidgetAction* sectionAction = new QWidgetAction(this);
    QLabel* sectionLabel = new QLabel(tr("Latest files"));
    sectionLabel->setStyleSheet("padding-left: 8px; font-weight: bold;");

    sectionAction->setDefaultWidget(sectionLabel);
    m_fileMenu->insertAction(m_lfEnd, sectionAction);

    auto files = m_lastFiles->files();
    for (int i = 0; i < files.count(); ++i) {
        auto* act = new QAction(files[i]);
        connect(act, &QAction::triggered, [this, files, i]() { load(files[i]); });
        m_fileMenu->insertAction(m_lfEnd, act);
    }
}

void WindowMain::load(const QString& file)
{
    QFile f(file);
    f.open(QFile::ReadOnly);
    json::Diagram diagram(m_pCanvas->diagram(), m_pCanvas->width(), m_pCanvas->height());
    auto doc = QJsonDocument::fromJson(f.readAll());
    diagram.fromJson(doc);
    f.close();

    m_pCanvas->resize(diagram.width(), diagram.height());
    m_pCanvas->update();
    m_lastFiles->recordFile(file);
}
