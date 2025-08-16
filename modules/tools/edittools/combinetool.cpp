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

void DecombineTool::activate() {
    auto items = scene()->selectedItems();
    if (items.size() != 1) return;
    auto group = dynamic_cast<IGraphicsItemGroup *>(items.first());
    if (!group) return;
    window()->pushCommand(new UngroupCommand(scene(), group));
}

void DecombineTool::deactivate() {
}