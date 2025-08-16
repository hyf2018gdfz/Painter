#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void FreeHandTool::onMousePress(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (!isDrawing) {
        isDrawing = true;
        tempPath = QPainterPath();
        tempPath.moveTo(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(QPen(color(), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        scene()->addItem(previewItem);
    }
}

void FreeHandTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing && previewItem) {
        tempPath.lineTo(pos);
        previewItem->setPath(tempPath);
    }
}

void FreeHandTool::onMouseRelease(QMouseEvent *event) {
    if (isDrawing && previewItem) {
        auto finalItem = new QGraphicsPathItem(tempPath);
        finalItem->setPen(QPen(color(), 2));
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable);
        window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
        scene()->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
    }
}