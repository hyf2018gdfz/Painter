#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void LineTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        startPos = pos;
        auto line = QLineF(startPos, startPos);
        previewItem = getScene(view)->addLine(line, QPen(Qt::blue, 1, Qt::DashLine));
    }
}

void LineTool::onMouseDoubleClick(CanvasView *view, const QPointF &pos) {
}

void LineTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        auto line = QLineF(startPos, pos);
        // TODO: 按 Shift 画八个方向的直线
        previewItem->setLine(line);
    }
}

void LineTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
    if (isDrawing && previewItem) {
        QLineF finalLine = previewItem->line();
        getScene(view)->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        auto finalItem = new QGraphicsLineItem(finalLine);
        finalItem->setPen(QPen(Qt::black, 2));
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
    }
}

bool LineTool::isBlocked() const {
    return isDrawing;
}