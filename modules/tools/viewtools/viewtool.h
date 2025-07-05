#ifndef VIEW_TOOL_H
#define VIEW_TOOL_H

#include "tools/itool.h"

class CanvasView;

class ViewTool : public ITool {
public:
    explicit ViewTool(CanvasView *view) : ITool(view) {}
    virtual ~ViewTool() = default;

    virtual CATEGORY category() const override { return CATEGORY::INDEPENDENT_OPERATION; }

    virtual void onMousePress(QMouseEvent *) override {};
    virtual void onMouseMove(QMouseEvent *) override {};
    virtual void onMouseRelease(QMouseEvent *) override {};

    virtual void onKeyPress(QKeyEvent *) override {};
    virtual void onKeyRelease(QKeyEvent *) override {};

    virtual void onWheel(QWheelEvent *) override {};
};

// 以下的activate应当理解为一次性执行，deactivate理解为复位

class ZoomTool : public ViewTool {
public:
    enum class MODE { IN, OUT, RESET };
    explicit ZoomTool(CanvasView *view, MODE mode) : m_mode(mode), ViewTool(view) {}

    void activate() override;
    void deactivate() override;

private:
    MODE m_mode;
};

class RotateViewTool : public ViewTool {
public:
    explicit RotateViewTool(CanvasView *view, qreal angle) : m_angle(angle), ViewTool(view) {}

    void activate() override;
    void deactivate() override;

private:
    qreal m_angle;
};
#endif