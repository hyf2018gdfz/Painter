#include "vision/canvasview.h"

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QPainter>

#include "tools/drawtools/drawtool.h"

CanvasView::CanvasView(MainWindow *mw, QWidget *parent) : window(mw), QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 720, 480);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    border = scene->addRect(scene->sceneRect());
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

// 在此注册工具
void CanvasView::initTools() {
    tools[ToolType::RECTANGLE] = new RectangleTool();
    tools[ToolType::ELLIPSE] = new EllipseTool();
    tools[ToolType::POLYGON] = new PolygonTool();
    tools[ToolType::SELECT] = new SelectTool();
    tools[ToolType::LINE] = new LineTool();
    tools[ToolType::FREEHAND] = new FreeHandTool();
}

void CanvasView::deleteSelectedItems() {
    auto selectedItems = scene->selectedItems();
    if (selectedItems.isEmpty()) return;
    window->pushCommand(new DeleteItemsCommand(scene, selectedItems));
    emit window->hasSelectionChanged(false);
}

void CanvasView::savePic() {
    scene->removeItem(border);
    QImage pic(720, 480, QImage::Format_RGB32);
    QPainter pt(&pic);
    auto *background = scene->addRect(scene->sceneRect());
    background->setBrush(QBrush(QColor(255, 255, 255, 255)));
    background->setZValue(-1);
    scene->render(&pt);
    pic.save("try.png", "PNG", 100);
    scene->removeItem(background);
    scene->addItem(border);
}

void CanvasView::zoomIn() {
    scale(1.2, 1.2);
}

void CanvasView::zoomOut() {
    scale(1 / 1.2, 1 / 1.2);
}

void CanvasView::rotateView(qreal angle) {
    rotate(angle);
    rotateAngle += angle;
}

qreal CanvasView::getRotateAngle() const {
    return rotateAngle;
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

void CanvasView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (currentTool && event->button() == Qt::LeftButton) {
        currentTool->onMouseDoubleClick(this, mapToScene(event->pos()));
        if (currentTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mouseDoubleClickEvent(event);
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