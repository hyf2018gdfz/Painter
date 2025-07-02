#include "tools/drawtools/drawtool.h"

#include "vision/canvasview.h"

void SelectTool::onMousePress(CanvasView *view, const QPointF &pos) {
}

void SelectTool::onMouseMove(CanvasView *view, const QPointF &pos) {
}

void SelectTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
}

void SelectTool::activate(CanvasView *view) {
    view->setDragMode(QGraphicsView::RubberBandDrag);
}

void SelectTool::deactivate(CanvasView *view) {
    view->setDragMode(QGraphicsView::NoDrag);
    getScene(view)->clearSelection();
}