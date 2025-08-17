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
        previewItem = scene()->addLine(line, QPen(Qt::blue, pen().widthF() / 1.5, Qt::DashLine));
    }
}

void LineTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing && previewItem) {
        QLineF line(startPos, pos);
        if (event->modifiers() & Qt::ShiftModifier) {
            qreal angle = fmod(line.angle() + 360.0, 360.0);
            qreal targetAngle = qRound(angle / 45.0) * 45.0;
            qreal length = line.length();
            QPointF delta(length * qCos(targetAngle * M_PI / 180.0),
                          -length * qSin(targetAngle * M_PI / 180.0));
            line.setP2(startPos + delta);
        }
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
        finalItem->setPen(pen());
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable);
        window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
    }
}