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

void IGraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::black, 0.5, Qt::DashLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());
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

/// BUG: 组合图形再撤销后，图形的zvalue会改变

void GroupCommand::undo() {
    m_scene->removeItem(m_group);
    m_scene->clearSelection();
    for (auto &item : m_items) {
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        m_group->removeFromGroup(item);
        m_scene->addItem(item);
        item->setSelected(true);
    }
    qDebug() << m_group->childItems();
    // delete m_group;
    // m_group = nullptr;
    emit m_scene->selectionChanged();
}

void GroupCommand::redo() {
    Q_ASSERT(m_items.size() >= 2);
    m_scene->clearSelection();
    if (m_group == nullptr) {
        m_group = new IGraphicsItemGroup();
    }
    for (auto &item : m_items) {
        item->setSelected(false);
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        m_scene->removeItem(item);
        m_group->addToGroup(item);
    }
    m_group->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    m_group->QGraphicsItem::setSelected(true);
    m_scene->addItem(m_group);
    qDebug() << m_group->childItems();
}