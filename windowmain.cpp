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
    auto* pMenu = new QMenu(tr("File"));
    pMenu->addAction(tr("Load (Ctrl+O)"), this, &WindowMain::load);
    pMenu->addAction(tr("Save (Ctrl+S)"), this, &WindowMain::save);
    pMenu->addSeparator();
    pMenu->addAction(tr("Quit (Ctrl+Q)"), this, &WindowMain::quit);
    menuBar->addMenu(pMenu);

    pMenu = new QMenu(tr("Edit"));
    pMenu->addAction(tr("Undo (Ctrl+Z)"), m_pCanvas, &DrawArea::undo);
    pMenu->addAction(tr("Redo (Ctrl+Y)"), m_pCanvas, &DrawArea::redo);
    menuBar->addMenu(pMenu);

    setMenuBar(menuBar);
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
    connect(sc, &QShortcut::activated, this, &WindowMain::load);

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

void WindowMain::load()
{
    auto file = QFileDialog::getOpenFileName(this, tr("Select file to load from"), ".", "*.fcd");
    if (file.isEmpty() == true)
        return;

    QFile f(file);
    f.open(QFile::ReadOnly);
    json::Diagram diagram(m_pCanvas->diagram(), m_pCanvas->width(), m_pCanvas->height());
    auto doc = QJsonDocument::fromJson(f.readAll());
    diagram.fromJson(doc);
    f.close();

    m_pCanvas->resize(diagram.width(), diagram.height());
    m_pCanvas->update();
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
}

void WindowMain::quit()
{
    QApplication::instance()->quit();
}
