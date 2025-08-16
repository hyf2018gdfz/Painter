#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "commands/icommand.h"

class MoveCommand : public ICommand {
public:
    MoveCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items, const QPointF &delta,
                QUndoCommand *parent = nullptr) :
        ICommand(scene, parent), m_items(items), m_delta(delta) {
        setText("移动图形");
    }

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> m_items;
    QPointF m_delta;
};

#endif
