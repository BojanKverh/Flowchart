#include "windowmain.h"

#include <QToolBar>
#include <QToolButton>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>

#include "scrollarea.h"
#include "drawarea.h"
#include "json/diagram.h"

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
    pMenu->addAction(tr("Load"), this, &WindowMain::load);
    pMenu->addAction(tr("Save"), this, &WindowMain::save);
    pMenu->addSeparator();
    pMenu->addAction(tr("Quit"), this, &WindowMain::quit);
    menuBar->addMenu(pMenu);

    setMenuBar(menuBar);
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
