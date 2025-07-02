#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void RectangleTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto rect = QRectF(startPos, startPos);
        previewItem = getScene(view)->addRect(rect, QPen(Qt::blue, 1, Qt::DashLine));
    }
}

void RectangleTool::onMouseDoubleClick(CanvasView *view, const QPointF &pos) {
}

void RectangleTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        auto rect = makeNormalizedRect(startPos, pos);
        // 按 Shift 画正方形
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
        getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
    }
}

bool RectangleTool::isBlocked() const {
    return isDrawing;
}