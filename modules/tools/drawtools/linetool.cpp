#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void LineTool::onMousePress(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto line = QLineF(startPos, startPos);
        previewItem = scene()->addLine(line, QPen(Qt::blue, 1, Qt::DashLine));
    }
}

void LineTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing && previewItem) {
        auto line = QLineF(startPos, pos);
        // TODO: 按 Shift 画八个方向的直线
        previewItem->setLine(line);
    }
}

void LineTool::onMouseRelease(QMouseEvent *event) {
    if (isDrawing && previewItem) {
        QLineF finalLine = previewItem->line();
        scene()->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        auto finalItem = new QGraphicsLineItem(finalLine);
        finalItem->setPen(QPen(color(), 2));
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable);
        window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
    }
}