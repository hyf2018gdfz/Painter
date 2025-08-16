#include "tools/edittools/edittool.h"
#include "vision/canvasview.h"
#include "vision/mainwindow.h"
#include "commands/movecommand.h"
#include "commands/groupcommand.h"

void SelectTool::onMousePress(QMouseEvent *event) {
    // setBlocked(true);
    m_lastPos = view()->mapToScene(event->pos());

    // 如果点击的位置没有选中的图形，清除现有选择
    QGraphicsItem *clickedItem = scene()->itemAt(m_lastPos, view()->transform());
    if (!clickedItem || !clickedItem->isSelected()) {
        scene()->clearSelection();
        if (clickedItem) {
            clickedItem->setSelected(true);
        }
    }

    // 创建预览图形
    if (!scene()->selectedItems().isEmpty()) {
        m_moving = true;
        m_startPos = m_lastPos;
        createPreviewItems();
    }
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
    // setBlocked(false);
    emit view() -> selectionCountChanged(scene()->selectedItems().size());
}

void SelectTool::createPreviewItems() {
    for (auto *item : scene()->selectedItems()) {
        // 创建克隆图形
        QGraphicsItem *preview = nullptr;
        if (auto *group = dynamic_cast<IGraphicsItemGroup *>(item)) {
            preview = new IGraphicsItemGroup();
            // 复制组内所有图形
            for (auto *child : group->childItems()) {
                auto *childPreview = scene()->addRect(child->boundingRect());
                childPreview->setParentItem(preview);
            }
        } else {
            preview = scene()->addRect(item->boundingRect());
        }

        // 设置预览图形的属性
        preview->setPos(item->scenePos());
        preview->setTransform(item->transform());
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