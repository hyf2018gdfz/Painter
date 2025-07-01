#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H
#include <QGraphicsView>
#include <memory>

#include "mainwindow.h"

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    CanvasView(MainWindow *mw, QWidget *parent = nullptr);
    void setTool(ToolType tool) {
        currentTool = tool;
    }
    void deleteSelectedItems(); // 从场景中移除图形
    void zoomIn();
    void zoomOut();               // 视图缩放
    void rotateView(qreal angle); // 视图旋转

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    MainWindow *window;
    QGraphicsScene *scene;
    ToolType currentTool = ToolType::SELECT;

    bool drawing = false;
    QGraphicsItem *previewItem = nullptr;
    QPointF startPoint;

    QRectF makeNormalizedRect(const QPointF &p1, const QPointF &p2);
};
#endif