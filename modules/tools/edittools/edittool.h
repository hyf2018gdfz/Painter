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

class CombineTool : public EditTool {
public:
    explicit CombineTool(CanvasView *view) : EditTool(view) {}

    CATEGORY category() const override { return CATEGORY::DEPENDENT_SELECTION; }

    void onMousePress(QMouseEvent *event) override {}
    void onMouseMove(QMouseEvent *event) override {}
    void onMouseRelease(QMouseEvent *event) override {}
    void onMouseDoubleClick(QMouseEvent *) override {}

    void onKeyPress(QKeyEvent *) override {}
    void onKeyRelease(QKeyEvent *) override {}

    void onWheel(QWheelEvent *) override {}

    void activate() override;
    void deactivate() override;
};

class DeleteTool : public EditTool {
public:
    explicit DeleteTool(CanvasView *view) : EditTool(view) {}

    CATEGORY category() const override { return CATEGORY::DEPENDENT_SELECTION; }

    void onMousePress(QMouseEvent *event) override {}
    void onMouseMove(QMouseEvent *event) override {}
    void onMouseRelease(QMouseEvent *event) override {}
    void onMouseDoubleClick(QMouseEvent *) override {}

    void onKeyPress(QKeyEvent *) override {}
    void onKeyRelease(QKeyEvent *) override {}

    void onWheel(QWheelEvent *) override {}

    void activate() override;
    void deactivate() override;
};

#endif