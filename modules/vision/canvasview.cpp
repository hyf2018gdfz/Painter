#include "canvasview.h"

#include <QMouseEvent>
#include <QGraphicsItem>

CanvasView::CanvasView(MainWindow *mw, QWidget *parent) : window(mw), QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 720, 480);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);

    QGraphicsRectItem *border = scene->addRect(scene->sceneRect());
    border->setPen(QPen(QColor(255, 0, 0, 63), 2));
    border->setBrush(Qt::NoBrush);
    border->setZValue(-1);
}

void CanvasView::deleteSelectedItems() {
    auto selectedItems = scene->selectedItems();
    for (auto *item : selectedItems) {
        scene->removeItem(item);
        delete item;
    }
}

void CanvasView::zoomIn() {
    scale(1.2, 1.2);
}

void CanvasView::zoomOut() {
    scale(1 / 1.2, 1 / 1.2);
}

void CanvasView::rotateView(qreal angle) {
    rotate(angle);
}

QRectF CanvasView::makeNormalizedRect(const QPointF &p1, const QPointF &p2) {
    qreal x1 = std::min(p1.x(), p2.x());
    qreal y1 = std::min(p1.y(), p2.y());
    qreal x2 = std::max(p1.x(), p2.x());
    qreal y2 = std::max(p1.y(), p2.y());
    return QRectF(QPointF(x1, y1), QPointF(x2, y2));
}

void CanvasView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPoint = mapToScene(event->pos());

        switch (currentTool) {
        case ToolType::RECTANGLE: {
            auto rect = QRectF(startPoint, startPoint);
            previewItem = scene->addRect(rect, QPen(Qt::blue, 1, Qt::DashLine));
            previewItem->setZValue(1);
            break;
        }
        case ToolType::ELLIPSE: {
            auto elli = QRectF(startPoint, startPoint);
            previewItem = scene->addEllipse(elli, QPen(Qt::blue, 1, Qt::DashLine));
            previewItem->setZValue(1);
            break;
        }
        case ToolType::TRIANGLE: {
            break;
        }
        case ToolType::FREEHAND: {
            break;
        }

        default: break;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event) {
    if (!previewItem) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    QPointF currentPoint = mapToScene(event->pos());
    QRectF updatedRect = makeNormalizedRect(startPoint, currentPoint);

    switch (currentTool) {
    case ToolType::RECTANGLE: {
        auto rectItem = qgraphicsitem_cast<QGraphicsRectItem *>(previewItem);
        if (rectItem) rectItem->setRect(updatedRect);
        break;
    }
    case ToolType::ELLIPSE: {
        auto elliItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(previewItem);
        if (elliItem) elliItem->setRect(updatedRect);
        break;
    }
    case ToolType::TRIANGLE:
        // TODO: 更新三角形
        break;
    case ToolType::FREEHAND:
        // TODO: 更新自由曲线
        break;
    default: break;
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event) {
    if (!previewItem) {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    switch (currentTool) {
    case ToolType::RECTANGLE: {
        auto rectItem = qgraphicsitem_cast<QGraphicsRectItem *>(previewItem);
        if (rectItem) {
            QRectF finalRect = rectItem->rect();
            scene->removeItem(rectItem);
            delete rectItem;
            auto finalItem = new QGraphicsRectItem(finalRect);
            finalItem->setPen(QPen(Qt::black, 2));
            finalItem->setBrush(Qt::transparent);
            finalItem->setFlag(QGraphicsItem::ItemIsSelectable);
            finalItem->setFlag(QGraphicsItem::ItemIsMovable);
            window->pushCommand(new AddItemCommand(scene, finalItem));
        }
        break;
    }
    case ToolType::ELLIPSE: {
        auto elliItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(previewItem);
        if (elliItem) {
            QRectF finalElli = elliItem->rect();
            scene->removeItem(elliItem);
            delete elliItem;
            auto finalItem = new QGraphicsEllipseItem(finalElli);
            finalItem->setPen(QPen(Qt::black, 2));
            finalItem->setBrush(Qt::transparent);
            finalItem->setFlag(QGraphicsItem::ItemIsSelectable);
            finalItem->setFlag(QGraphicsItem::ItemIsMovable);
            window->pushCommand(new AddItemCommand(scene, finalItem));
        }
        break;
    }
    case ToolType::TRIANGLE:
        // TODO: 完成三角形绘制
        break;
    case ToolType::FREEHAND:
        // TODO: 完成自由曲线
        break;
    default: break;
    }

    previewItem = nullptr;
    QGraphicsView::mouseReleaseEvent(event);
}

void CanvasView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    } else if (event->modifiers() & Qt::ShiftModifier) {
        const qreal rotationStep = 3.0;
        if (event->angleDelta().y() > 0) {
            rotateView(rotationStep);
        } else {
            rotateView(-rotationStep);
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}