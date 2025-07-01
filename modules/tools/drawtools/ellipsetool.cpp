#include "tools/drawtools/drawtool.h"

#include "vision/canvasview.h"

void EllipseTool::onMousePress(CanvasView *view, const QPointF &pos) {
    startPos = pos;
    auto rect = QRectF(startPos, startPos);
    previewItem = getScene(view)->addEllipse(rect, QPen(Qt::blue, 1, Qt::DashLine));
}

void EllipseTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (!previewItem) return;
    previewItem->setRect(makeNormalizedRect(startPos, pos));
}

void EllipseTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (!previewItem) return;
    QRectF finalElli = previewItem->rect();
    getScene(view)->removeItem(previewItem);
    delete previewItem;
    previewItem = nullptr;
    auto finalItem = new QGraphicsEllipseItem(finalElli);
    finalItem->setPen(QPen(Qt::black, 2));
    finalItem->setBrush(Qt::transparent);
    finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    getScene(view)->addItem(finalItem);
}