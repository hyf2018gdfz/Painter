#ifndef GROUP_COMMAND_H
#define GROUP_COMMAND_H

#include "commands/icommand.h"
#include <QGraphicsItem>
#include <QList>

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

/// 保存每个子项的状态（z 值 + 在 group 坐标下的位置）
struct ItemState {
    QGraphicsItem *item;
    qreal z;
    QPointF posInGroup;
};

class GroupCommand : public ICommand {
public:
    GroupCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                 QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    IGraphicsItemGroup *group() const { return m_group; }

private:
    QList<ItemState> m_items;
    IGraphicsItemGroup *m_group = nullptr;
};

class UngroupCommand : public ICommand {
public:
    UngroupCommand(QGraphicsScene *scene, IGraphicsItemGroup *group,
                   QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<ItemState> m_items;
    IGraphicsItemGroup *m_group = nullptr;
};

#endif
