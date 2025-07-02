#include "vision/mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QActionGroup>

#include "vision/canvasview.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), undoStack(new QUndoStack(this)) {
    view = new CanvasView(this);
    setCentralWidget(view);

    initMenus();

    setWindowTitle(tr("Painter"));
    resize(800, 600);

    undoStack->setUndoLimit(50);
}

void MainWindow::initMenus() {
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("文件");
    QAction *openAction = fileMenu->addAction("打开");
    QAction *saveAction = fileMenu->addAction("保存");
    QAction *exitAction = fileMenu->addAction("退出");

    QMenu *editMenu = menuBar->addMenu("编辑");
    QAction *copyAction = editMenu->addAction("复制");
    QAction *pasteAction = editMenu->addAction("粘贴");
    QAction *undoAction = editMenu->addAction("撤销");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);
    QAction *redoAction = editMenu->addAction("重做");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);
    connect(undoStack, &QUndoStack::canUndoChanged, undoAction, &QAction::setEnabled);
    connect(undoStack, &QUndoStack::canRedoChanged, redoAction, &QAction::setEnabled);
    undoAction->setEnabled(undoStack->canUndo());
    redoAction->setEnabled(undoStack->canRedo());
    QAction *deleteAction = editMenu->addAction("删除选中");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelected);

    QMenu *viewMenu = menuBar->addMenu("视图");
    QAction *zoomInAction = viewMenu->addAction("放大");
    zoomInAction->setShortcut(QKeySequence("Ctrl++"));
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    QAction *zoomOutAction = viewMenu->addAction("缩小");
    zoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    QAction *rotateCWAction = viewMenu->addAction("顺时针旋转");
    rotateCWAction->setShortcut(QKeySequence("Shift++"));
    connect(rotateCWAction, &QAction::triggered, [this]() { rotateView(3); });
    QAction *rotateCCWAction = viewMenu->addAction("逆时针旋转");
    rotateCCWAction->setShortcut(QKeySequence("Shift+-"));
    connect(rotateCCWAction, &QAction::triggered, [this]() { rotateView(-3); });

    QMenu *toolMenu = menuBar->addMenu("工具");
    QAction *selectTool = toolMenu->addAction("选择");
    connect(selectTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::SELECT); });
    QAction *freeHandTool = toolMenu->addAction("画笔");
    connect(freeHandTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::FREEHAND); });
    QAction *lineTool = toolMenu->addAction("直线");
    connect(lineTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::LINE); });
    QAction *rectTool = toolMenu->addAction("矩形");
    connect(rectTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::RECTANGLE); });
    QAction *elliTool = toolMenu->addAction("椭圆");
    connect(elliTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::ELLIPSE); });
    QAction *polyTool = toolMenu->addAction("多边形");
    connect(polyTool, &QAction::triggered, [this]() { setCurrentTool(ToolType::POLYGON); });
}

void MainWindow::pushCommand(QUndoCommand *command) {
    undoStack->push(command);
}

void MainWindow::setCurrentTool(ToolType tool) {
    currentTool = tool;
    view->setTool(currentTool);
}

void MainWindow::undo() {
    undoStack->undo();
}

void MainWindow::redo() {
    undoStack->redo();
}

void MainWindow::deleteSelected() {
    view->deleteSelectedItems();
}

void MainWindow::zoomIn() {
    view->zoomIn();
}

void MainWindow::zoomOut() {
    view->zoomOut();
}

void MainWindow::rotateView(qreal angle) {
    view->rotateView(angle);
}