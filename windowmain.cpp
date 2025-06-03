#include "windowmain.h"

#include <QApplication>
#include <QDrag>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMimeData>
#include <QShortcut>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QWidgetAction>

#include "drawarea.h"
#include "scrollarea.h"
#include "json/diagram.h"

WindowMain::WindowMain(QWidget* parent)
  : QMainWindow(parent)
{
  setMinimumSize(800, 600);

  buildUI();
  buildControl();
}

WindowMain::~WindowMain() {}

DrawArea* WindowMain::current() const
{
  auto* area = dynamic_cast<ScrollArea*>(m_pCentral->currentWidget());
  if (area == nullptr)
    return nullptr;

  auto* canvas = dynamic_cast<DrawArea*>(area->widget());
  return canvas;
}

void WindowMain::buildUI()
{
  m_lastFiles = new LastFilesHandler(10);
  connect(m_lastFiles, &LastFilesHandler::filesChanged, this, &WindowMain::updateLastFiles);

  m_ptb = new QToolBar;
  m_ptb->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea | Qt::TopToolBarArea
                         | Qt::BottomToolBarArea);

  addButton(":/res/icons/start.png", tr("Start"), data::ShapeType::esStart);
  addButton(":/res/icons/end.png", tr("End"), data::ShapeType::esEnd);
  addButton(":/res/icons/io.png", tr("Input/Output"), data::ShapeType::esIO);
  addButton(":/res/icons/process.png", tr("Process"), data::ShapeType::esProcess);
  addButton(":/res/icons/decision.png", tr("Decision"), data::ShapeType::esDecision);

  addToolBar(m_ptb);

  m_pCentral = new QTabWidget(this);
  newTab();

  setCentralWidget(m_pCentral);

  auto* menuBar = new QMenuBar;
  m_fileMenu    = new QMenu(tr("File"));
  m_fileMenu->addAction(tr("New (Ctrl+N"), this, &WindowMain::newDiagram);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(tr("Load (Ctrl+O)"), this, qOverload<>(&WindowMain::load));
  m_fileMenu->addAction(tr("Save (Ctrl+S)"), this, &WindowMain::save);
  m_fileMenu->addAction(tr("Save As (Ctrl+Shirt+S)"), this, &WindowMain::saveAs);
  m_lfStart = m_fileMenu->addSeparator();
  m_lfEnd   = m_fileMenu->addSeparator();
  m_fileMenu->addAction(tr("Quit (Ctrl+Q)"), this, &WindowMain::quit);
  menuBar->addMenu(m_fileMenu);

  auto* pMenu = new QMenu(tr("Edit"));
  pMenu->addAction(tr("Undo (Ctrl+Z)"), this, &WindowMain::undo);
  pMenu->addAction(tr("Redo (Ctrl+Y)"), this, &WindowMain::redo);
  pMenu->addSeparator();
  pMenu->addAction(tr("Copy (Ctrl+C)"), this, &WindowMain::copy);
  pMenu->addAction(tr("Paste (Ctrl+V)"), this, &WindowMain::paste);
  pMenu->addSeparator();
  pMenu->addAction(tr("Select All (Ctrl+A)"), this, &WindowMain::selectAll);
  menuBar->addMenu(pMenu);

  setMenuBar(menuBar);

  updateLastFiles();
}

void WindowMain::buildControl()
{
  auto* sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::newDiagram);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, qOverload<>(&WindowMain::load));

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::save);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::saveAs);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::quit);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::undo);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::redo);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_C), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::copy);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_V), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::paste);

  sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_A), this);
  sc->setContext(Qt::ApplicationShortcut);
  connect(sc, &QShortcut::activated, this, &WindowMain::selectAll);
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
  newTab();
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
  auto* canvas = current();
  auto file    = QString::fromStdString(canvas->diagram().name());
  if (file.isEmpty() == true) {
    saveAs();
    return;
  }

  json::Diagram diagram(canvas->diagram(), canvas->width(), canvas->height());
  QFile f(file);
  if (f.open(QFile::WriteOnly) == false)
    return;

  QTextStream ts(&f);
  f.write(diagram.toJson().toJson());
  f.close();
  m_lastFiles->recordFile(file);
}

void WindowMain::saveAs()
{
  auto file = QFileDialog::getSaveFileName(this, tr("Select file to save into"), ".", "*.fcd");
  if (file.isEmpty() == true)
    return;

  auto* canvas = current();
  canvas->diagram().setName(file.toStdString());
  updateName();
  save();
}

void WindowMain::quit()
{
  QApplication::instance()->quit();
}

void WindowMain::updateLastFiles()
{
  auto list     = m_fileMenu->actions();
  bool toDelete = false;
  for (int i = 0; i < list.count(); ++i) {
    if (list[i] == m_lfStart) {
      toDelete = true;
    } else if (list[i] == m_lfEnd) {
      toDelete = false;
    } else if (toDelete == true) {
      m_fileMenu->removeAction(list[i]);
      delete list[i];
    }
  }

  QWidgetAction* sectionAction = new QWidgetAction(this);
  QLabel* sectionLabel         = new QLabel(tr("Latest files"));
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
  if (f.open(QFile::ReadOnly) == false)
    return;

  m_lastFiles->recordFile(file);

  for (int i = 0; i < m_pCentral->count(); ++i) {
    auto* area = dynamic_cast<ScrollArea*>(m_pCentral->widget(i));
    if (area == nullptr)
      break;
    auto* canvas = dynamic_cast<DrawArea*>(area->widget());
    if (canvas->diagram().name() == file.toStdString()) {
      m_pCentral->setCurrentIndex(i);
      return;
    }
  }

  newTab();
  auto* canvas = current();
  json::Diagram diagram(canvas->diagram(), canvas->width(), canvas->height());
  auto doc = QJsonDocument::fromJson(f.readAll());
  diagram.fromJson(doc);
  f.close();

  canvas->resize(diagram.width(), diagram.height());
  canvas->update();
  canvas->diagram().setName(file.toStdString());
  updateName();
}

void WindowMain::undo()
{
  current()->undo();
}

void WindowMain::redo()
{
  current()->redo();
}

void WindowMain::copy()
{
  m_copy = std::make_unique<data::Diagram>();
  m_copy->copySelected(current()->diagram());
}

void WindowMain::paste()
{
  if (m_copy == nullptr)
    return;
  current()->paste(m_copy.get());
}

void WindowMain::selectAll()
{
  current()->selectAll();
}

void WindowMain::newTab()
{
  if (current() != nullptr && current()->diagram().isEmpty() == true)
    return;

  auto* area   = new ScrollArea;
  auto* canvas = new DrawArea;
  area->setWidget(canvas);
  m_pCentral->addTab(area, "");
  m_pCentral->setCurrentIndex(m_pCentral->count() - 1);
  updateName();
}

void WindowMain::updateName()
{
  auto name = QString::fromStdString(current()->diagram().name());
  if (name.length() == 0)
    name = tr("NONAME");
  else {
    QFileInfo fi(name);
    name = fi.fileName();
  }
  m_pCentral->setTabText(m_pCentral->currentIndex(), name);
}
