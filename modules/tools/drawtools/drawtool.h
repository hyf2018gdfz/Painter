#ifndef DRAW_TOOL_H
#define DRAW_TOOL_H
#include <QGraphicsItem>

QRectF makeNormalizedRect(const QPointF &p1, const QPointF &p2);

class CanvasView;
class MainWindow;

class DrawTool {
public:
    virtual ~DrawTool() {
    }
    virtual void onMousePress(CanvasView *view, const QPointF &scenePos) = 0;
    virtual void onMouseMove(CanvasView *view, const QPointF &scenePos) = 0;
    virtual void onMouseRelease(CanvasView *view, const QPointF &scenePos) = 0;

protected:
    QGraphicsScene *getScene(CanvasView *view) const;
    MainWindow *getWindow(CanvasView *view) const;
};

class RectangleTool : public DrawTool {
public:
    void onMousePress(CanvasView *view, const QPointF &pos) override;
    void onMouseMove(CanvasView *view, const QPointF &pos) override;
    void onMouseRelease(CanvasView *view, const QPointF &pos) override;

private:
    QPointF startPos;
    QGraphicsRectItem *previewItem = nullptr;
};

class EllipseTool : public DrawTool {
public:
    void onMousePress(CanvasView *view, const QPointF &pos) override;
    void onMouseMove(CanvasView *view, const QPointF &pos) override;
    void onMouseRelease(CanvasView *view, const QPointF &pos) override;

private:
    QPointF startPos;
    QGraphicsEllipseItem *previewItem = nullptr;
};

// class PolygonTool : public DrawTool {
// public:
//     void onMousePress(CanvasView *view, const QPointF &pos) override;
//     void onMouseMove(CanvasView *view, const QPointF &pos) override;
//     void onMouseRelease(CanvasView *view, const QPointF &pos) override;

// private:
//     QVector<QPointF> m_points;
//     QGraphicsPolygonItem *m_preview = nullptr;
// };
#endif