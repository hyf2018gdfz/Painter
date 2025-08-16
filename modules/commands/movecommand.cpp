#include "commands/movecommand.h"
#include <QGraphicsItem>

void MoveCommand::undo() {
    for (auto item : m_items) {
        item->moveBy(-m_delta.x(), -m_delta.y());
    }
}

void MoveCommand::redo() {
    for (auto item : m_items) {
        item->moveBy(m_delta.x(), m_delta.y());
    }
}
