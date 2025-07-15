#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void EllipseTool::onMousePress(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto rect = QRectF(startPos, startPos);
        previewItem = scene()->addEllipse(rect, QPen(Qt::blue, 1, Qt::DashLine));
        previewItem->setTransformOriginPoint(startPos);
        previewItem->setRotation(-view()->getRotateAngle());
    }
}

void EllipseTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing && previewItem) {
        auto localPos = previewItem->sceneTransform().inverted().map(pos);
        auto rect = makeNormalizedRect(startPos, localPos);
        // 按 Shift 画圆形
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

void EllipseTool::onMouseRelease(QMouseEvent *event) {
    if (isDrawing && previewItem) {
        QRectF finalElli = previewItem->rect();
        scene()->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        auto *finalItem = new QGraphicsEllipseItem(finalElli);
        finalItem->setPen(QPen(color(), 2));
        finalItem->setBrush(Qt::transparent);
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        finalItem->setTransformOriginPoint(startPos);
        finalItem->setRotation(-view()->getRotateAngle());
        window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
    }
}