#include "vision/mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QToolButton>

#include "vision/canvasview.h"
#include "color/colorfield.h"
#include "commands/groupcommand.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), undoStack(new QUndoStack(this)) {
    view = new CanvasView(this);
    setCentralWidget(view);
    setMouseTracking(true);

    setWindowTitle(tr("Painter"));
    resize(1920, 1080);
    showMaximized();

    undoStack->setUndoLimit(50);

    initMenus();
    initLeftBar();
}

void MainWindow::initMenus() {
    QMenuBar *menuBar = this->menuBar();

    /// TODO: 实现文件新建、打开和保存
    QMenu *fileMenu = menuBar->addMenu("文件");
    QAction *newAction = fileMenu->addAction("新建");
    newAction->setShortcut(QKeySequence::New);
    QAction *openAction = fileMenu->addAction("打开");
    openAction->setShortcut(QKeySequence::Open);
    QAction *saveAction = fileMenu->addAction("保存");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::savePic);
    QAction *exitAction = fileMenu->addAction("退出");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    /// TODO: 实现复制粘贴
    QMenu *editMenu = menuBar->addMenu("编辑");
    QAction *copyAction = editMenu->addAction("复制");
    copyAction->setShortcut(QKeySequence::Copy);
    QAction *pasteAction = editMenu->addAction("粘贴");
    pasteAction->setShortcut(QKeySequence::Paste);
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
    connect(deleteAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::DELETE_SELECTED); });
    deleteAction->setEnabled(false);
    connect(view, &CanvasView::selectionCountChanged,
            [deleteAction](int count) { deleteAction->setEnabled(count >= 1); });
    QAction *combineAction = editMenu->addAction("组合");
    combineAction->setShortcut(QKeySequence("Ctrl+G"));
    connect(combineAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::COMBINE_SELECTED); });
    combineAction->setEnabled(false);
    connect(view, &CanvasView::selectionCountChanged,
            [combineAction](int count) { combineAction->setEnabled(count >= 2); });

    // 添加解组菜单项
    QAction *decombineAction = editMenu->addAction("解组");
    decombineAction->setShortcut(QKeySequence("Ctrl+Shift+G"));
    decombineAction->setEnabled(false);
    connect(decombineAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::DECOMBINE_SELECTED); });
    connect(view, &CanvasView::selectionCountChanged, [this, decombineAction](int count) {
        // 仅当选中项为组合图形时启用
        if (count == 1) {
            auto items = view->get_scene()->selectedItems();
            bool isGroup = dynamic_cast<IGraphicsItemGroup *>(items.value(0)) != nullptr;
            decombineAction->setEnabled(isGroup);
        } else {
            decombineAction->setEnabled(false);
        }
    });

    QMenu *viewMenu = menuBar->addMenu("视图");
    QAction *zoomInAction = viewMenu->addAction("放大");
    zoomInAction->setShortcut(QKeySequence("Ctrl++"));
    connect(zoomInAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::ZOOMIN); });
    QAction *zoomOutAction = viewMenu->addAction("缩小");
    zoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    connect(zoomOutAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::ZOOMOUT); });
    QAction *rotateCCWAction = viewMenu->addAction("逆时针旋转");
    rotateCCWAction->setShortcut(QKeySequence("Shift++"));
    connect(rotateCCWAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::ROTATEVIEWCCW); });
    QAction *rotateCWAction = viewMenu->addAction("顺时针旋转");
    rotateCWAction->setShortcut(QKeySequence("Shift+-"));
    connect(rotateCWAction, &QAction::triggered,
            [this]() { view->executeCommand(ToolType::ROTATEVIEWCW); });

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

/// TODO: 左侧画笔选择框
void MainWindow::initLeftBar() {
    leftBar = new QWidget(this);
    leftBar->setFixedWidth(250);
    leftBar->setStyleSheet("background: #f0f0f0;");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(leftBar);
    sidebarLayout->setContentsMargins(5, 5, 5, 5);
    sidebarLayout->setSpacing(10);

    auto *colorField = new ColorField(leftBar);
    sidebarLayout->addWidget(colorField);

    connect(colorField, &ColorField::colorChanged, view, &CanvasView::changeColor);

    dockWidget = new QDockWidget(tr("Color Panel"), this);

    dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dockWidget->setWidget(leftBar);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

void MainWindow::pushCommand(QUndoCommand *command) {
    undoStack->push(command);
}

void MainWindow::setCurrentTool(ToolType tool) {
    currentTool = tool;
    view->executeCommand(currentTool);
}

void MainWindow::savePic() {
    view->savePic();
}

void MainWindow::undo() {
    undoStack->undo();
}

void MainWindow::redo() {
    undoStack->redo();
}