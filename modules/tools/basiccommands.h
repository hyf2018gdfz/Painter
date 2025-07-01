#ifndef BASIC_COMMANDS_H
#define BASIC_COMMANDS_H
#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsScene>

class AddItemCommand : public QUndoCommand {
public:
    AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr) :
        QUndoCommand(parent), m_scene(scene), m_item(item) {
        setText("添加图形");
    }

    void undo() override {
        m_scene->removeItem(m_item);
    }

    void redo() override {
        m_scene->addItem(m_item);
    }

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *m_item;
};

class DeleteItemsCommand : public QUndoCommand {
public:
    DeleteItemsCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &item,
                       QUndoCommand *parent = nullptr) :
        QUndoCommand(parent), m_scene(scene), m_items(item) {
        setText("删除图形");
    }

    void undo() override {
        for (auto item : m_items) {
            m_scene->addItem(item);
        }
    }

    void redo() override {
        for (auto item : m_items) {
            m_scene->removeItem(item);
        }
    }

private:
    QGraphicsScene *m_scene;
    QList<QGraphicsItem *> m_items;
};

// class ModifyItemCommand : public QUndoCommand {
// public:
//     ModifyItemCommand(QGraphicsScene *scene, QGraphicsItem *item, QUndoCommand *parent = nullptr)
//     :
//         QUndoCommand(parent), m_scene(scene), m_item(item) {
//         setText("移动图形");
//     }

//     void undo() override {
//         m_scene->addItem(m_item);
//     }

//     void redo() override {
//         m_scene->removeItem(m_item);
//     }

// private:
//     QGraphicsScene *m_scene;
//     QGraphicsItem *m_item;
// };

#endif