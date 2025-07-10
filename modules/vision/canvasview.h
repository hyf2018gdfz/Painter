#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H

#include <QGraphicsView>

#include <memory>

enum class CATEGORY;
enum class ToolType;
class ITool;
class ToolManager;
class MainWindow;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    CanvasView(MainWindow *mw, QWidget *parent = nullptr);
    void setTool(ToolType tool);
    void deleteSelectedItems(); // 从场景中移除图形
    void savePic();
    // void zoomIn();
    // void zoomOut();               // 视图缩放
    // void rotateView(qreal angle); // 视图旋转
    qreal getRotateAngle() const;

    void executeCommand(ToolType tool);

signals:
    void selectionCountChanged(int count);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // void keyPressEvent(QKeyEvent *event) override;
    // void keyReleaseEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

private:
    void initTools();
    void updateSelectionState();

    MainWindow *window;
    QGraphicsScene *scene;
    ToolManager *toolManager;

    ITool *curTool = nullptr; // 此处的tool应该是选择或者某种画图

    qreal rotateAngle;
    QGraphicsRectItem *border;

    friend class ITool;
};
#endif