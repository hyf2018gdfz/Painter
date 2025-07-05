#ifndef EDIT_TOOL_H
#define EDIT_TOOL_H

#include "tools/itool.h"

class CanvasView;

class EditTool : public ITool {
public:
    explicit EditTool(CanvasView *view) : ITool(view) {}
    virtual ~EditTool() = default;
};

class SelectTool : public EditTool {
public:
    explicit SelectTool(CanvasView *view) : EditTool(view) {}
    virtual ~SelectTool() = default;

    CATEGORY category() const override { return CATEGORY::SELECT_TOOL; }

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;
    void onMouseDoubleClick(QMouseEvent *) override {}

    void onKeyPress(QKeyEvent *) override {}
    void onKeyRelease(QKeyEvent *) override {}

    void onWheel(QWheelEvent *) override {}

    void activate() override;
    void deactivate() override;
};
#endif