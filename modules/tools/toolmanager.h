#ifndef TOOL_MANAGER_H
#define TOOL_MANAGER_H

#include <unordered_map>
#include <memory>

#include "common/common.h"

class ITool;
class CanvasView;

class ToolManager {
public:
    explicit ToolManager(CanvasView *view) : m_view(view) {}

    template <typename T, typename... Args>
    void registerTool(ToolType type, Args &&...args) {
        m_tools[type] = std::make_unique<T>(m_view, std::forward<Args>(args)...);
    }

    ITool *getTool(ToolType type) const;
    // ~ToolManager();

private:
    CanvasView *m_view;
    std::unordered_map<ToolType, std::unique_ptr<ITool>> m_tools;
};

#endif