#include "vision/canvasview.h"

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QPainter>

#include "vision/mainwindow.h"
#include "common/common.h"
#include "tools/itool.h"
#include "tools/drawtools/drawtool.h"
#include "tools/edittools/edittool.h"
#include "tools/viewtools/viewtool.h"
#include "tools/toolmanager.h"

CanvasView::CanvasView(MainWindow *mw, QWidget *parent) : window(mw), QGraphicsView(parent) {
    toolManager = new ToolManager(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 720, 480);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    border = scene->addRect(scene->sceneRect());
    border->setPen(QPen(QColor(255, 0, 0, 63), 2));
    border->setBrush(Qt::NoBrush);
    border->setZValue(-1);

    initTools();
    setTool(ToolType::SELECT);

    connect(scene, &QGraphicsScene::selectionChanged, this, &CanvasView::updateSelectionState);
}

void CanvasView::setTool(ToolType tool) {
    if (curTool) curTool->deactivate();
    curTool = toolManager->getTool(tool);
    Q_ASSERT(curTool->category() == CATEGORY::SELECT_TOOL
             || curTool->category() == CATEGORY::EXCLUSIVE_OPERATION);
    if (curTool) curTool->activate();
}

// 在此注册工具
void CanvasView::initTools() {
    toolManager->registerTool<RectangleTool>(ToolType::RECTANGLE);
    toolManager->registerTool<EllipseTool>(ToolType::ELLIPSE);
    toolManager->registerTool<PolygonTool>(ToolType::POLYGON);
    toolManager->registerTool<LineTool>(ToolType::LINE);
    toolManager->registerTool<FreeHandTool>(ToolType::FREEHAND);
    toolManager->registerTool<SelectTool>(ToolType::SELECT);
    toolManager->registerTool<ZoomTool>(ToolType::ZOOMIN, ZoomTool::MODE::IN);
    toolManager->registerTool<ZoomTool>(ToolType::ZOOMOUT, ZoomTool::MODE::OUT);
    toolManager->registerTool<RotateViewTool>(ToolType::ROTATEVIEWCW, 3);
    toolManager->registerTool<RotateViewTool>(ToolType::ROTATEVIEWCCW, -3);
    toolManager->registerTool<DeleteTool>(ToolType::DELETE_SELECTED);
    toolManager->registerTool<CombineTool>(ToolType::COMBINE_SELECTED);
}

void CanvasView::updateSelectionState() {
    int count = scene->selectedItems().size();
    emit selectionCountChanged(count);
}

void CanvasView::executeCommand(ToolType tooltype) {
    auto *tool = toolManager->getTool(tooltype);
    Q_ASSERT(tool != nullptr);
    // qDebug() << "now: " << (int)curTool->category() << " switch " << (int)tool->category();
    switch (tool->category()) {
    case CATEGORY::INDEPENDENT_OPERATION: {
        tool->activate();
        break;
    }
    case CATEGORY::DEPENDENT_SELECTION: {
        if (curTool->category() == CATEGORY::SELECT_TOOL) {
            tool->activate();
        }
        break;
    }
    case CATEGORY::EXCLUSIVE_OPERATION: {
        setTool(tooltype);
        break;
    }
    case CATEGORY::SELECT_TOOL: {
        setTool(tooltype);
        break;
    }
    default: break;
    }
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

// void CanvasView::zoomIn() {
//     scale(1.2, 1.2);
// }

// void CanvasView::zoomOut() {
//     scale(1 / 1.2, 1 / 1.2);
// }

// void CanvasView::rotateView(qreal angle) {
//     rotate(angle);
//     rotateAngle += angle;
// }

qreal CanvasView::getRotateAngle() const {
    return rotateAngle;
}

void CanvasView::mousePressEvent(QMouseEvent *event) {
    if (curTool && event->button() == Qt::LeftButton) {
        curTool->onMousePress(event);
        if (curTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void CanvasView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (curTool && event->button() == Qt::LeftButton) {
        curTool->onMouseDoubleClick(event);
        if (curTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent *event) {
    if (curTool) {
        curTool->onMouseMove(event);
        if (curTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event) {
    if (curTool && event->button() == Qt::LeftButton) {
        curTool->onMouseRelease(event);
        if (curTool->isBlocked()) {
            event->accept();
            return;
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CanvasView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            executeCommand(ToolType::ZOOMIN);
        } else {
            executeCommand(ToolType::ZOOMOUT);
        }
        event->accept();
    } else if (event->modifiers() & Qt::ShiftModifier) {
        if (event->angleDelta().y() > 0) {
            rotateAngle -= 3;
            executeCommand(ToolType::ROTATEVIEWCCW);
        } else {
            rotateAngle += 3;
            executeCommand(ToolType::ROTATEVIEWCW);
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}