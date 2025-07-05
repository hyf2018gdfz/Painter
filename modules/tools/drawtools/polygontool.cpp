#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void PolygonTool::onMousePress(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (!isDrawing) {
        isDrawing = true;
        points.clear();
        points.push_back(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
        previewDashLine = new QGraphicsPathItem(previewItem);
        snapIndicator = new QGraphicsEllipseItem(previewItem);
        scene()->addItem(previewItem);
        updateDrawing(pos);
    } else {
        points.push_back(pos);
        updateDrawing(pos);

        if (points.size() >= 3 && QLineF(pos, points.first()).length() <= 10.0) {
            finishDrawing();
            cancelDrawing();
        }
    }
}

void PolygonTool::onMouseMove(QMouseEvent *event) {
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing) {
        updateDrawing(pos);
    }
}

void PolygonTool::onMouseRelease(QMouseEvent *event) {
}

void PolygonTool::onMouseDoubleClick(QMouseEvent *event) {
    if (isDrawing) {
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
    finalItem->setPen(QPen(Qt::black, 2));
    finalItem->setBrush(Qt::transparent);
    finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    window()->pushCommand(new AddItemCommand(scene(), finalItem));
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
    isDrawing = false;
}

void PolygonTool::updateDrawing(const QPointF &pos) {
    if (!isDrawing || points.isEmpty()) return;
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
        previewDashLine->setPen(QPen(Qt::blue, 1, Qt::DashLine));
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
    view()->setMouseTracking(true);
}

void PolygonTool::deactivate() {
    view()->setMouseTracking(false);
}