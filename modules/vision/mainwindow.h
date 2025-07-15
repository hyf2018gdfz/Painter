#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QUndoStack>

#include "commands/icommand.h"
#include "common/common.h"

class CanvasView;

// 主窗口类：管理工具、动作、撤销栈
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ToolType getCurrentTool() const { return currentTool; }
    void pushCommand(QUndoCommand *command);

public slots:
    void setCurrentTool(ToolType tool); // 切换当前工具
    void savePic();
    void undo(); // 撤销操作
    void redo(); // 重做操作

private:
    void initMenus();
    void initLeftBar();

    CanvasView *view;
    QUndoStack *undoStack;
    ToolType currentTool = ToolType::SELECT;

    QDockWidget *dockWidget;

    QWidget *leftBar;
};
#endif