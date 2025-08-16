#include "commands/groupcommand.h"

#include <QCoreApplication>
#include <QGraphicsItem>
#include <QPainter>

QRectF IGraphicsItemGroup::boundingRect() const {
    if (childItems().empty()) return QRectF();

    QRectF rect;
    for (const auto &child : childItems()) {
        rect |= child->mapToParent(child->boundingRect()).boundingRect();
    }
    return rect;
}

QVariant IGraphicsItemGroup::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemSelectedHasChanged) {
        update(); // 强制重绘以更新外边框
    }
    return QGraphicsItem::itemChange(change, value);
}

void IGraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isSelected()) {
        painter->setPen(QPen(Qt::black, 0.5, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}

void IGraphicsItemGroup::addToGroup(QGraphicsItem *item) {
    if (!item) return;
    item->setParentItem(this);
    prepareGeometryChange();
}

void IGraphicsItemGroup::removeFromGroup(QGraphicsItem *item) {
    if (!item || item->parentItem() != this) return;
    item->setParentItem(nullptr);
    prepareGeometryChange();
}

void GroupCommand::redo() {
    if (!m_group) m_group = new IGraphicsItemGroup();
    m_scene->clearSelection();

    for (auto &st : m_items) {
        m_scene->removeItem(st.item);
        st.item->setParentItem(m_group);
        st.item->setSelected(false);
        // 禁止子项被选中和移动
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        st.item->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    m_group->setFlags(QGraphicsItem::ItemIsSelectable);
    if (!m_scene->items().contains(m_group)) m_scene->addItem(m_group);
    m_group->setSelected(true);
}

void GroupCommand::undo() {
    m_scene->removeItem(m_group);
    for (auto &st : m_items) {
        QPointF scenePos = m_group->mapToScene(st.item->pos());
        st.item->setParentItem(nullptr);
        st.item->setZValue(st.z);
        st.item->setPos(scenePos);
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        // st.item->setFlag(QGraphicsItem::ItemIsMovable, true);
        m_scene->addItem(st.item);
    }
    m_scene->clearSelection();
    for (auto &st : m_items) st.item->setSelected(true);
}

void UngroupCommand::redo() {
    // 保存组合的位置，用于撤销时恢复
    m_groupPos = m_group->pos();
    m_scene->removeItem(m_group);
    for (auto &st : m_items) {
        QPointF scenePos = m_group->mapToScene(st.item->pos());
        st.item->setParentItem(nullptr);
        st.item->setZValue(st.z);
        st.item->setPos(scenePos);
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        // st.item->setFlag(QGraphicsItem::ItemIsMovable, true);
        m_scene->addItem(st.item);
    }
    m_scene->clearSelection();
    for (auto &st : m_items) st.item->setSelected(true);
}

void UngroupCommand::undo() {
    m_scene->clearSelection();
    for (auto &st : m_items) {
        m_scene->removeItem(st.item);
        st.item->setParentItem(m_group);
        st.item->setSelected(false);
        // 禁止子项被选中和移动
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        st.item->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    if (!m_scene->items().contains(m_group)) {
        m_scene->addItem(m_group);
        m_group->setPos(m_groupPos); // 恢复组合的位置
    }
    m_group->setSelected(true);
}
