#include "tools/drawtools/drawtool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

QRectF makeNormalizedRect(const QPointF &p1, const QPointF &p2) {
    qreal x1 = std::min(p1.x(), p2.x());
    qreal y1 = std::min(p1.y(), p2.y());
    qreal x2 = std::max(p1.x(), p2.x());
    qreal y2 = std::max(p1.y(), p2.y());
    return QRectF(QPointF(x1, y1), QPointF(x2, y2));
}

QGraphicsScene *DrawTool::getScene(CanvasView *view) const {
    return view->scene;
}

MainWindow *DrawTool::getWindow(CanvasView *view) const {
    return view->window;
}

void DrawTool::activate(CanvasView *view) {
}

void DrawTool::deactivate(CanvasView *view) {
}

bool DrawTool::isBlocked() const {
    return false;
}