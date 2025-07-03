#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void RectangleTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto rect = QRectF(startPos, startPos);
        previewItem = getScene(view)->addRect(rect, QPen(Qt::blue, 1, Qt::DashLine));
        previewItem->setTransformOriginPoint(startPos);
        previewItem->setRotation(-view->getRotateAngle());
    }
}

void RectangleTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        auto localPos = previewItem->sceneTransform().inverted().map(pos);
        auto rect = makeNormalizedRect(startPos, localPos);
        // 按 Shift 画正方形
        if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
            qreal len = qMin(rect.width(), rect.height());
            if (localPos.x() < startPos.x()) rect.setX(startPos.x() - len);
            if (localPos.y() < startPos.y()) rect.setY(startPos.y() - len);
            rect.setWidth(len);
            rect.setHeight(len);
        }
        previewItem->setRect(rect);
    }
}

void RectangleTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        QRectF finalRect = previewItem->rect();
        getScene(view)->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        auto finalItem = new QGraphicsRectItem(finalRect);
        finalItem->setPen(QPen(Qt::black, 2));
        finalItem->setBrush(Qt::transparent);
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        finalItem->setTransformOriginPoint(startPos);
        finalItem->setRotation(-view->getRotateAngle());
        getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
    }
}

bool RectangleTool::isBlocked() const {
    return isDrawing;
}