#include "tools/edittools/edittool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"
#include "commands/groupcommand.h"

void CombineTool::activate() {
    auto items = scene()->selectedItems();
    window()->pushCommand(new GroupCommand(scene(), items));
    emit view() -> selectionCountChanged(0);
}

void CombineTool::deactivate() {
}