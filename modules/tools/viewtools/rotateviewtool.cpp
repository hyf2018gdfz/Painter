#include "tools/viewtools/viewtool.h"

#include "vision/canvasview.h"

void RotateViewTool::activate() {
    view()->rotate(m_angle);
}

void RotateViewTool::deactivate() {
}