#include "tools/edittools/edittool.h"

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

void DeleteTool::activate() {
    auto items = scene()->selectedItems();
    Q_ASSERT(items.size() >= 1);
    window()->pushCommand(new DeleteItemsCommand(scene(), items));
    emit view() -> selectionCountChanged(0);
}

void DeleteTool::deactivate() {
}