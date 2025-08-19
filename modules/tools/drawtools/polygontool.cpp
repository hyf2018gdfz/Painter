#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void PolygonTool::onMousePress(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (!isUpdating) {
        isUpdating = true;
        points.clear();
        points.push_back(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(QPen(Qt::blue, pen().widthF() / 1.5, Qt::SolidLine));
        previewDashLine = new QGraphicsPathItem(previewItem);
        snapIndicator = new QGraphicsEllipseItem(previewItem);
        scene()->addItem(previewItem);
        updateDrawing(pos);
    } else {
        updateDrawing(pos);

        if (points.size() >= 3 && QLineF(pos, points.first()).length() <= 10.0) {
            points.push_back(points.first());
            finishDrawing();
            cancelDrawing();
            return;
        }
        points.push_back(pos);
    }
}

void PolygonTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isUpdating) {
        updateDrawing(pos);
    }
}

void PolygonTool::onMouseRelease(QMouseEvent *event) {
}

void PolygonTool::onMouseDoubleClick(QMouseEvent *event) {
    if (isUpdating) {
        if (points.size() >= 3) {
            finishDrawing();
        }
        cancelDrawing();
    }
}

void PolygonTool::finishDrawing() {
    QPolygonF finalPolygon(points);
    finalPolygon.push_back(points.first());
    auto finalItem = new QGraphicsPolygonItem(finalPolygon);
    finalItem->setPen(pen());
    finalItem->setBrush(Qt::transparent);
    finalItem->setFlags(QGraphicsItem::ItemIsSelectable);
    window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
}

void PolygonTool::cancelDrawing() {
    if (previewItem) {
        scene()->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        previewDashLine = nullptr;
        snapIndicator = nullptr;
    }
    points.clear();
    isUpdating = false;
}

void PolygonTool::updateDrawing(const QPointF &pos) {
    if (!isUpdating || points.isEmpty()) return;
    QPainterPath path;
    if (points.size() >= 2) {
        path.moveTo(points.first());
        for (int i = 1; i < points.size(); ++i) {
            path.lineTo(points[i]);
        }
        previewItem->setPath(path);
    }
    if (previewDashLine) {
        delete previewDashLine;
        previewDashLine = new QGraphicsPathItem(previewItem);
        previewDashLine->setPen(QPen(Qt::blue, pen().widthF() / 1.5, Qt::DashLine));
        previewDashLine->setZValue(1);
    }

    QPainterPath dashPath;
    dashPath.moveTo(points.last());
    dashPath.lineTo(pos);
    if (points.size() >= 2) {
        dashPath.moveTo(pos);
        dashPath.lineTo(points.first());
    }
    previewDashLine->setPath(dashPath);

    if (points.size() >= 3 && QLineF(pos, points.first()).length() <= 10.0) {
        snapIndicator = new QGraphicsEllipseItem(previewItem);
        snapIndicator->setRect(points.first().x() - 5, points.first().y() - 5, 10, 10);
        snapIndicator->setPen(QPen(Qt::red, 1, Qt::DashLine));
        snapIndicator->setZValue(1);
    } else if (snapIndicator) {
        delete snapIndicator;
        snapIndicator = nullptr;
    }
}

void PolygonTool::activate() {
    isDrawing = true;
}

void PolygonTool::deactivate() {
    isDrawing = false;
}