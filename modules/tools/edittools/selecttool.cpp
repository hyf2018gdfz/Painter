#include "tools/edittools/edittool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void SelectTool::onMousePress(QMouseEvent *event) {
}

void SelectTool::onMouseMove(QMouseEvent *event) {
}

void SelectTool::onMouseRelease(QMouseEvent *event) {
    emit view() -> selectionCountChanged(scene()->selectedItems().size());
}

void SelectTool::activate() {
    view()->setDragMode(QGraphicsView::RubberBandDrag);
}

void SelectTool::deactivate() {
    view()->setDragMode(QGraphicsView::NoDrag);
    scene()->clearSelection();
}