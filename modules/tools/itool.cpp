#include "tools/itool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"
#include "common/common.h"

CanvasView *ITool::view() const {
    return m_view;
}

MainWindow *ITool::window() const {
    return m_view->window;
}

QGraphicsScene *ITool::scene() const {
    return m_view->scene;
}