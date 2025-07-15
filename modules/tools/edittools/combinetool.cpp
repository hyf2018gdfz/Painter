#include "tools/edittools/edittool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"
#include "commands/groupcommand.h"

void CombineTool::activate() {
    auto items = scene()->selectedItems();
    window()->pushCommand(new GroupCommand(scene(), items));
}

void CombineTool::deactivate() {
}