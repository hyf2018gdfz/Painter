#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void FreeHandTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        tempPath = QPainterPath();
        tempPath.moveTo(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        getScene(view)->addItem(previewItem);
    }
}

void FreeHandTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        tempPath.lineTo(pos);
        previewItem->setPath(tempPath);
    }
}

void FreeHandTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        auto finalItem = new QGraphicsPathItem(tempPath);
        finalItem->setPen(QPen(Qt::black, 2));
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
        getScene(view)->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
    }
}

bool FreeHandTool::isBlocked() const {
    return isDrawing;
}