#include "tools/drawtools/drawtool.h"

#include "vision/canvasview.h"

void RectangleTool::onMousePress(CanvasView *view, const QPointF &pos) {
    startPos = pos;
    auto rect = QRectF(startPos, startPos);
    previewItem = getScene(view)->addRect(rect, QPen(Qt::blue, 1, Qt::DashLine));
}

void RectangleTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (!previewItem) return;
    previewItem->setRect(makeNormalizedRect(startPos, pos));
}

void RectangleTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (!previewItem) return;
    QRectF finalRect = previewItem->rect();
    getScene(view)->removeItem(previewItem);
    delete previewItem;
    previewItem = nullptr;
    auto finalItem = new QGraphicsRectItem(finalRect);
    finalItem->setPen(QPen(Qt::black, 2));
    finalItem->setBrush(Qt::transparent);
    finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
}