#include "tools/edittools/edittool.h"
#include "vision/canvasview.h"
#include "vision/mainwindow.h"
#include "commands/movecommand.h"
#include "commands/groupcommand.h"

/// INFO: 检查的选中状态看的是图形的碰撞框shape
void SelectTool::onMousePress(QMouseEvent *event) {
    m_lastPos = view()->mapToScene(event->pos());

    QGraphicsItem *clickedItem = scene()->itemAt(m_lastPos, view()->transform());

    bool clickInSelected = false;
    for (auto *item : scene()->selectedItems()) {
        auto itemPos = item->mapFromScene(m_lastPos);
        // qDebug() << item->shape() << " " << itemPos;
        if (item->shape().contains(itemPos)) {
            clickInSelected = true;
            break;
        }
    }
    if (!clickInSelected) {
        scene()->clearSelection();
        if (clickedItem) {
            clickedItem->setSelected(true);
        }
    }

    if (!scene()->selectedItems().isEmpty()) {
        m_moving = true;
        m_startPos = m_lastPos;
        createPreviewItems();
    }
    // qDebug() << m_lastPos << " " << clickedItem << " " << clickInSelected;
}

void SelectTool::onMouseMove(QMouseEvent *event) {
    if (m_moving) {
        QPointF newPos = view()->mapToScene(event->pos());
        QPointF delta = newPos - m_lastPos;

        // 更新预览图形的位置
        for (auto item : m_previewItems) {
            item->moveBy(delta.x(), delta.y());
        }

        m_lastPos = newPos;
    }
}

void SelectTool::onMouseRelease(QMouseEvent *event) {
    if (m_moving) {
        QPointF endPos = view()->mapToScene(event->pos());
        QPointF delta = endPos - m_startPos;

        // 删除预览图形
        qDeleteAll(m_previewItems);
        m_previewItems.clear();

        // 执行实际的移动命令
        if (delta.manhattanLength() > 0) {
            window()->pushCommand(new MoveCommand(scene(), scene()->selectedItems(), delta));
        }

        m_moving = false;
    }
    emit view() -> selectionCountChanged(scene()->selectedItems().size());
}

void SelectTool::createPreviewItems() {
    for (auto *item : scene()->selectedItems()) {
        // 创建克隆图形
        QGraphicsItem *preview = nullptr;
        if (auto *group = dynamic_cast<IGraphicsItemGroup *>(item)) {
            // 对于组合图形，只显示其边界矩形
            auto rect = group->mapRectToScene(group->boundingRect());
            preview = scene()->addRect(rect);
            preview->setPos(QPointF(0, 0));
        } else {
            preview = scene()->addRect(item->boundingRect());
            preview->setPos(item->scenePos());
            preview->setTransform(item->transform());
            preview->setRotation(item->rotation());
            // qDebug() << item << " " << item->transform();
        }

        preview->setOpacity(0.5);
        m_previewItems.append(preview);
    }
}

void SelectTool::activate() {
    view()->setDragMode(QGraphicsView::RubberBandDrag);
}

void SelectTool::deactivate() {
    qDeleteAll(m_previewItems);
    m_previewItems.clear();
    m_moving = false;
    view()->setDragMode(QGraphicsView::NoDrag);
    scene()->clearSelection();
}