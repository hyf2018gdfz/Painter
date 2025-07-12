#include "commands/groupcommand.h"

/// BUG: 组合图形再撤销后，图形的zvalue会改变

void GroupCommand::undo() {
    m_scene->removeItem(m_group);
    for (auto *item : m_items) {
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        m_scene->addItem(item);
        m_group->removeFromGroup(item);
        item->setSelected(true);
    }
    delete m_group;
    m_group = nullptr;
}

void GroupCommand::redo() {
    Q_ASSERT(m_group == nullptr && m_items.size() >= 2);
    m_group = new QGraphicsItemGroup();
    for (auto *item : m_items) {
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        item->setSelected(false);
        m_scene->removeItem(item);
        m_group->addToGroup(item);
    }
    m_group->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    m_group->setSelected(true);
    m_scene->addItem(m_group);
}