#include "tools/toolmanager.h"

#include "vision/canvasview.h"
#include "tools/itool.h"

// ToolManager::~ToolManager() {
//     m_tools.clear();
// }

ITool *ToolManager::getTool(ToolType type) const {
    if (auto it = m_tools.find(type); it != m_tools.end()) {
        return it->second.get();
    }
    return nullptr;
}