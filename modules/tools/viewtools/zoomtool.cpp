#include "tools/viewtools/viewtool.h"

#include "vision/canvasview.h"

void ZoomTool::activate() {
    switch (m_mode) {
    case MODE::IN: view()->scale(1.2, 1.2); break;
    case MODE::OUT: view()->scale(1 / 1.2, 1 / 1.2); break;
    default: break;
    }
}

void ZoomTool::deactivate() {
    qreal curAngle = view()->getRotateAngle();
    QTransform trans;
    trans.scale(1.0, 1.0);
    trans.rotate(curAngle);
    view()->setTransform(trans);
}