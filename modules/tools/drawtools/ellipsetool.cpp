#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void EllipseTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto rect = QRectF(startPos, startPos);
        previewItem = getScene(view)->addEllipse(rect, QPen(Qt::blue, 1, Qt::DashLine));
    }
}

void EllipseTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        auto rect = makeNormalizedRect(startPos, pos);
        // 按 Shift 画圆形
        if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
            qreal len = qMin(rect.width(), rect.height());
            if (pos.x() < startPos.x()) rect.setX(startPos.x() - len);
            if (pos.y() < startPos.y()) rect.setY(startPos.y() - len);
            rect.setWidth(len);
            rect.setHeight(len);
        }
        previewItem->setRect(rect);
    }
}

void EllipseTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        QRectF finalElli = previewItem->rect();
        getScene(view)->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        auto finalItem = new QGraphicsEllipseItem(finalElli);
        finalItem->setPen(QPen(Qt::black, 2));
        finalItem->setBrush(Qt::transparent);
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
    }
}

bool EllipseTool::isBlocked() const {
    return isDrawing;
}