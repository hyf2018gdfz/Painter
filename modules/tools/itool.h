#ifndef I_TOOL_H
#define I_TOOL_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "common/common.h"

class MainWindow;
class CanvasView;

class ITool {
public:
    explicit ITool(CanvasView *view) : m_view(view) {}
    virtual ~ITool() = default;
    ITool(const ITool &) = delete;
    ITool &operator=(const ITool &) = delete;

    virtual CATEGORY category() const = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual bool isBlocked() const { return false; }

    virtual void onMousePress(QMouseEvent *) = 0;
    virtual void onMouseMove(QMouseEvent *) = 0;
    virtual void onMouseRelease(QMouseEvent *) = 0;
    virtual void onMouseDoubleClick(QMouseEvent *) {}

    virtual void onKeyPress(QKeyEvent *) = 0;
    virtual void onKeyRelease(QKeyEvent *) = 0;

    virtual void onWheel(QWheelEvent *) = 0;

protected:
    CanvasView *view() const;
    MainWindow *window() const;
    QGraphicsScene *scene() const;
    QColor color() const;

private:
    CanvasView *m_view;
};
#endif
