#ifndef GROUP_COMMAND_H
#define GROUP_COMMAND_H

#include "commands/icommand.h"

struct ItemState {
    QGraphicsItem *item;
    QGraphicsItem *oldParent;
    qreal z;
};

class IGraphicsItemGroup : public QGraphicsItem {
public:
    explicit IGraphicsItemGroup(QGraphicsItem *parent = nullptr) : QGraphicsItem(parent) {}
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void addToGroup(QGraphicsItem *item);
    void removeFromGroup(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

class GroupCommand : public ICommand {
public:
    GroupCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                 QUndoCommand *parent = nullptr) : ICommand(scene, parent) {
        setText("组合图形");
        for (auto *st : items) {
            m_items.append({st, st->parentItem(), st->zValue()});
        }
    }

    void undo() override;
    void redo() override;

private:
    QList<ItemState> m_items;
    IGraphicsItemGroup *m_group = nullptr;
};

class UngroupCommand : public ICommand {
public:
    UngroupCommand(QGraphicsScene *scene, IGraphicsItemGroup *group,
                   QUndoCommand *parent = nullptr) : ICommand(scene, parent), m_group(group) {
        setText("解组图形");
        for (auto *st : group->childItems()) {
            m_items.append({st, group, st->zValue()});
        }
    }

    void undo() override;
    void redo() override;

private:
    IGraphicsItemGroup *m_group;
    QList<ItemState> m_items;
    QPointF m_groupPos; // 添加此行以记录组合的位置
};

#endif