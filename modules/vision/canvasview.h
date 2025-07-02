#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H
#include <QGraphicsView>

#include "vision/mainwindow.h"

class DrawTool;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    CanvasView(MainWindow *mw, QWidget *parent = nullptr);
    void setTool(ToolType tool);
    void deleteSelectedItems(); // 从场景中移除图形
    void zoomIn();
    void zoomOut();               // 视图缩放
    void rotateView(qreal angle); // 视图旋转

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void initTools();

    MainWindow *window;
    QGraphicsScene *scene;
    std::unordered_map<ToolType, DrawTool *> tools;
    DrawTool *currentTool = nullptr;

    friend class DrawTool;
};
#endif