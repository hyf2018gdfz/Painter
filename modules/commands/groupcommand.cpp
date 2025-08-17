#include "commands/groupcommand.h"
#include <QPainter>
#include <QDebug>

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
        update(); // 强制重绘选中边框
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

// ----------------- GroupCommand -----------------

GroupCommand::GroupCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                           QUndoCommand *parent) : ICommand(scene, parent) {
    setText("组合图形");

    for (auto *it : items) {
        ItemState st;
        st.item = it;
        st.z = it->zValue();
        st.posInGroup = it->pos(); // 初始时在 group 下的局部坐标
        m_items.append(st);
    }
}

void GroupCommand::redo() {
    if (!m_group) m_group = new IGraphicsItemGroup();
    m_scene->clearSelection();

    for (auto &st : m_items) {
        m_scene->removeItem(st.item);
        st.item->setParentItem(m_group);
        st.item->setPos(st.posInGroup);
        st.item->setSelected(false);
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
        QPointF scenePos = m_group->mapToScene(st.posInGroup);
        st.item->setParentItem(nullptr);
        st.item->setZValue(st.z);
        st.item->setPos(scenePos);
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        m_scene->addItem(st.item);
    }
    m_scene->clearSelection();
    for (auto &st : m_items) st.item->setSelected(true);
}

// ----------------- UngroupCommand -----------------

UngroupCommand::UngroupCommand(QGraphicsScene *scene, IGraphicsItemGroup *group,
                               QUndoCommand *parent) : ICommand(scene, parent), m_group(group) {
    setText("解组图形");

    for (auto *it : group->childItems()) {
        ItemState st;
        st.item = it;
        st.z = it->zValue();
        st.posInGroup = it->pos(); // 在 group 内部的局部坐标
        m_items.append(st);
    }
}

void UngroupCommand::redo() {
    m_scene->removeItem(m_group);
    for (auto &st : m_items) {
        QPointF scenePos = m_group->mapToScene(st.posInGroup);
        st.item->setParentItem(nullptr);
        st.item->setZValue(st.z);
        st.item->setPos(scenePos);
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, true);
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
        st.item->setPos(st.posInGroup); // 恢复 group-local 坐标
        st.item->setSelected(false);
        st.item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        st.item->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    if (!m_scene->items().contains(m_group)) {
        m_scene->addItem(m_group);
    }
    m_group->setSelected(true);
}
