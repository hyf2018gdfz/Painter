#include "vision/canvasview.h"

#include <QMouseEvent>
#include <QGraphicsItem>

#include "tools/drawtools/drawtool.h"

CanvasView::CanvasView(MainWindow *mw, QWidget *parent) : window(mw), QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 720, 480);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    QGraphicsRectItem *border = scene->addRect(scene->sceneRect());
    border->setPen(QPen(QColor(255, 0, 0, 63), 2));
    border->setBrush(Qt::NoBrush);
    border->setZValue(-1);

    initTools();
    setTool(ToolType::SELECT);
}

void CanvasView::setTool(ToolType tool) {
    if (currentTool) currentTool->deactivate(this);
    auto it = tools.find(tool);
    currentTool = (it != tools.end()) ? it->second : nullptr;
    if (currentTool) currentTool->activate(this);
}

void CanvasView::initTools() {
    tools[ToolType::RECTANGLE] = new RectangleTool();
    tools[ToolType::ELLIPSE] = new EllipseTool();
    tools[ToolType::SELECT] = new SelectTool();
}

void CanvasView::deleteSelectedItems() {
    auto selectedItems = scene->selectedItems();
    if (selectedItems.isEmpty()) return;
    window->pushCommand(new DeleteItemsCommand(scene, selectedItems));
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

void CanvasView::mousePressEvent(QMouseEvent *event) {
    if (currentTool && event->button() == Qt::LeftButton) {
        currentTool->onMousePress(this, mapToScene(event->pos()));
        if (currentTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event) {
    if (currentTool) {
        currentTool->onMouseMove(this, mapToScene(event->pos()));
        if (currentTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event) {
    if (currentTool && event->button() == Qt::LeftButton) {
        currentTool->onMouseRelease(this, mapToScene(event->pos()));
        if (currentTool->isBlocked()) {
            event->accept();
            return;
        }
    }
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