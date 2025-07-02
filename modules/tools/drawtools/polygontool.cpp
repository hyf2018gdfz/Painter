#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"

void PolygonTool::onMousePress(CanvasView *view, const QPointF &pos) {
    if (!isDrawing) {
        isDrawing = true;
        points.clear();
        points.push_back(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
        previewDashLine = new QGraphicsPathItem(previewItem);
        snapIndicator = new QGraphicsEllipseItem(previewItem);
        getScene(view)->addItem(previewItem);
        updateDrawing(view, pos);
    } else {
        points.push_back(pos);
        updateDrawing(view, pos);

        if (points.size() >= 3 && QLineF(pos, points.first()).length() <= 10.0) {
            finishDrawing(view);
            cancelDrawing(view);
        }
    }
}

void PolygonTool::onMouseMove(CanvasView *view, const QPointF &pos) {
    if (isDrawing) {
        updateDrawing(view, pos);
    }
}

void PolygonTool::onMouseRelease(CanvasView *view, const QPointF &pos) {
}

void PolygonTool::onMouseDoubleClick(CanvasView *view, const QPointF &pos) {
    if (isDrawing) {
        if (points.size() >= 3) {
            finishDrawing(view);
        }
        cancelDrawing(view);
    }
}

void PolygonTool::finishDrawing(CanvasView *view) {
    QPolygonF finalPolygon(points);
    finalPolygon.push_back(points.first());
    auto finalItem = new QGraphicsPolygonItem(finalPolygon);
    finalItem->setPen(QPen(Qt::black, 2));
    finalItem->setBrush(Qt::transparent);
    finalItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    getWindow(view)->pushCommand(new AddItemCommand(getScene(view), finalItem));
}

void PolygonTool::cancelDrawing(CanvasView *view) {
    if (previewItem) {
        getScene(view)->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        previewDashLine = nullptr;
        snapIndicator = nullptr;
    }
    points.clear();
    isDrawing = false;
}

void PolygonTool::updateDrawing(CanvasView *view, const QPointF &pos) {
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

bool PolygonTool::isBlocked() const {
    return isDrawing;
}

void PolygonTool::activate(CanvasView *view) {
    view->setMouseTracking(true);
}

void PolygonTool::deactivate(CanvasView *view) {
    view->setMouseTracking(false);
}