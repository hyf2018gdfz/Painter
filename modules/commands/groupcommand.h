#ifndef GROUP_COMMAND_H
#define GROUP_COMMAND_H

#include "commands/icommand.h"

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
    QGraphicsItemGroup *m_group = nullptr;
};

#endif