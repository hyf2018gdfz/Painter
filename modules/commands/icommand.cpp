#include "commands/icommand.h"

void AddItemsCommand::undo() {
    for (auto item : m_items) {
        m_scene->removeItem(item);
    }
}

void AddItemsCommand::redo() {
    for (auto item : m_items) {
        m_scene->addItem(item);
    }
}

void DeleteItemsCommand::undo() {
    for (auto *item : m_items) {
        qDebug() << "undo deleting" << item;
        m_scene->addItem(item);
    }
}

void DeleteItemsCommand::redo() {
    for (auto *item : m_items) {
        qDebug() << "deleting" << item;
        m_scene->removeItem(item);
    }
}

void CompositeCommand::undo() {
    for (auto it = m_commands.rbegin(); it != m_commands.rend(); ++it) {
        (*it)->undo();
    }
}

void CompositeCommand::redo() {
    for (const auto &command : m_commands) {
        command->redo();
    }
}