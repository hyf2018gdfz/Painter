#ifndef DRAW_TOOL_H
#define DRAW_TOOL_H

#include <QGraphicsItem>
#include "tools/itool.h"

class DrawTool : public ITool {
public:
    explicit DrawTool(CanvasView *view) : ITool(view) {}
    virtual ~DrawTool() = default;

    virtual CATEGORY category() const override { return CATEGORY::EXCLUSIVE_OPERATION; }

    virtual void onKeyPress(QKeyEvent *) override {}
    virtual void onKeyRelease(QKeyEvent *) override {}

    virtual void onWheel(QWheelEvent *) override {}

    virtual void activate() override {}
    virtual void deactivate() override {}

    bool isBlocked() const override { return isDrawing; };

protected:
    bool isDrawing = false;
};

class LineTool : public DrawTool {
public:
    explicit LineTool(CanvasView *view) : DrawTool(view) {}

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

private:
    QPointF startPos;
    QGraphicsLineItem *previewItem = nullptr;
};

class RectangleTool : public DrawTool {
public:
    explicit RectangleTool(CanvasView *view) : DrawTool(view) {}

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

private:
    QPointF startPos;
    QGraphicsRectItem *previewItem = nullptr;
};

class EllipseTool : public DrawTool {
public:
    explicit EllipseTool(CanvasView *view) : DrawTool(view) {}

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

private:
    QPointF startPos;
    QGraphicsEllipseItem *previewItem = nullptr;
};

class PolygonTool : public DrawTool {
public:
    explicit PolygonTool(CanvasView *view) : DrawTool(view) {}

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;
    void onMouseDoubleClick(QMouseEvent *event) override;

    void activate() override;
    void deactivate() override;

private:
    QVector<QPointF> points;
    QGraphicsPathItem *previewItem = nullptr;
    QGraphicsPathItem *previewDashLine = nullptr;
    QGraphicsEllipseItem *snapIndicator = nullptr;

    bool isUpdating = false;

    void updateDrawing(const QPointF &pos);
    void cancelDrawing();
    void finishDrawing();
};

class FreeHandTool : public DrawTool {
public:
    explicit FreeHandTool(CanvasView *view) : DrawTool(view) {}

    void onMousePress(QMouseEvent *event) override;
    void onMouseMove(QMouseEvent *event) override;
    void onMouseRelease(QMouseEvent *event) override;

private:
    QGraphicsPathItem *previewItem = nullptr;
    QPainterPath tempPath;
};
#endif