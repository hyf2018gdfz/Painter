#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QUndoStack>

#include "basiccommands.hpp"

// 工具类型枚举
enum class ToolType {
    FREEHAND,
    RECTANGLE,
    TRIANGLE,
    ELLIPSE,
    SELECT,
    DELETE,
    COPY,
    PASTE,
    UNDO,
    REDO,
    NEW,
    SAVE,
    OPEN
};

class CanvasView;

// 主窗口类：管理工具、动作、撤销栈
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ToolType getCurrentTool() const {
        return currentTool;
    }
    void pushCommand(QUndoCommand *command);

public slots:
    void setCurrentTool(ToolType tool); // 切换当前工具
    void undo();                        // 撤销操作
    void redo();                        // 重做操作
    void deleteSelected();              // 删除所有选中图形
    void zoomIn();
    void zoomOut();
    void rotateView(qreal angle);
    // ... 菜单和工具栏相关函数
private:
    void initMenus();

    CanvasView *view;
    QUndoStack *undoStack;
    ToolType currentTool = ToolType::SELECT;
};
#endif