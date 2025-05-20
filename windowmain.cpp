#include "windowmain.h"

#include <QToolBar>
#include <QToolButton>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QMenuBar>

#include "scrollarea.h"
#include "drawarea.h"

WindowMain::WindowMain(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 600);

    buildUI();
}

WindowMain::~WindowMain() {}

void WindowMain::buildUI()
{
    m_ptb = new QToolBar;
    m_ptb->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea | Qt::BottomToolBarArea);

    addButton(":/res/icons/start.png", tr("Start"), ShapeType::esStart);
    addButton(":/res/icons/end.png", tr("End"), ShapeType::esEnd);
    addButton(":/res/icons/io.png", tr("Input/Output"), ShapeType::esIO);
    addButton(":/res/icons/process.png", tr("Process"), ShapeType::esProcess);
    addButton(":/res/icons/decision.png", tr("Decision"), ShapeType::esDecision);

    addToolBar(m_ptb);

    m_pArea = new ScrollArea;

    m_pCanvas = new DrawArea;
    m_pArea->setWidget(m_pCanvas);

    setCentralWidget(m_pArea);

    auto* menuBar = new QMenuBar;
    auto* pMenu = new QMenu(tr("File"));
    pMenu->addAction(tr("Load"), []() {});
    pMenu->addAction(tr("Save"), []() {});
    pMenu->addSeparator();
    pMenu->addAction(tr("Quit"), []() {});
    menuBar->addMenu(pMenu);

    setMenuBar(menuBar);
}

void WindowMain::addButton(QString icon, QString text, ShapeType shape)
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
