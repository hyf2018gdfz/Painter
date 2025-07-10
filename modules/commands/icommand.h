#ifndef I_COMMAND_H
#define I_COMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

class ICommand : public QUndoCommand {
public:
    ICommand(QGraphicsScene *scene, QUndoCommand *parent = nullptr) :
        QUndoCommand(parent), m_scene(scene) {}
    virtual ~ICommand() = default;

    virtual void undo() override = 0;
    virtual void redo() override = 0;

protected:
    QGraphicsScene *m_scene;
};

class AddItemsCommand : public ICommand {
public:
    AddItemsCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                    QUndoCommand *parent = nullptr) : ICommand(scene, parent), m_items(items) {
        setText("添加图形");
    }

    template <typename... Args>
    AddItemsCommand(QGraphicsScene *scene, QUndoCommand *parent, Args *...args) :
        ICommand(scene, parent), m_items{static_cast<QGraphicsItem *>(args)...} {
        setText("添加图形");
    }

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> m_items;
};

class DeleteItemsCommand : public ICommand {
public:
    DeleteItemsCommand(QGraphicsScene *scene, const QList<QGraphicsItem *> &items,
                       QUndoCommand *parent = nullptr) : ICommand(scene, parent), m_items(items) {
        setText("删除图形");
    }

    template <typename... Args>
    DeleteItemsCommand(QGraphicsScene *scene, QUndoCommand *parent, Args *...args) :
        ICommand(scene, parent), m_items{static_cast<QGraphicsItem *>(args)...} {
        setText("删除图形");
    }

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> m_items;
};

/// INFO:
/// 暂时保留如下不优美的实现，这个实现需要多次地传入scene，额外创建一个上下文管理器可以解决这个问题。但是目前没有这个需要，所以暂时不去实现。

class CompositeCommand : public ICommand {
public:
    CompositeCommand(QGraphicsScene *scene, const QList<ICommand *> &commands,
                     QUndoCommand *parent = nullptr) :
        ICommand(scene, parent), m_commands(commands) {
        setText("组合命令");
    }

    template <typename... Args>
    CompositeCommand(QGraphicsScene *scene, QUndoCommand *parent, Args *...args) :
        ICommand(scene, parent), m_commands{static_cast<ICommand *>(args)...} {
        setText("组合命令");
    }

    void undo() override;
    void redo() override;

private:
    QList<ICommand *> m_commands;
};

#endif