#ifndef GROUP_COMMAND_H
#define GROUP_COMMAND_H

#include "commands/icommand.h"

class IGraphicsItemGroup : public QGraphicsItem {
public:
    explicit IGraphicsItemGroup(QGraphicsItem *parent = nullptr) : QGraphicsItem(parent) {}
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void addToGroup(QGraphicsItem *item);
    void removeFromGroup(QGraphicsItem *item);
};

class GroupCommand : public ICommand {
public:
    GroupCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                 QUndoCommand *parent = nullptr) : ICommand(scene, parent), m_items(items) {
        setText("组合图形");
    }

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> m_items;
    IGraphicsItemGroup *m_group = nullptr;
};

#endif