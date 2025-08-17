#ifndef CANVAS_VIEW_H
#define CANVAS_VIEW_H

#include <QGraphicsView>
#include <QColor>
#include <QPen>
#include <QBrush>

#include <memory>

enum class CATEGORY;
enum class ToolType;
class ITool;
class ToolManager;
class MainWindow;

class CanvasView : public QGraphicsView {
    Q_OBJECT
public:
    CanvasView(MainWindow *mw, QWidget *parent = nullptr);
    void setTool(ToolType tool);
    void savePic();
    qreal getRotateAngle() const;
    QGraphicsScene *get_scene() const { return this->scene; }

    void executeCommand(ToolType tool);

    ToolManager *toolManager;

signals:
    void selectionCountChanged(int count);

public slots:
    void changeColor(QColor newColor);
    void changePen(QPen newPen);
    void changeBrush(QBrush newBrush);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // void keyPressEvent(QKeyEvent *event) override;
    // void keyReleaseEvent(QKeyEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void tabletEvent(QTabletEvent *event) override;

private:
    void initTools();
    // void updateSelectionState();

    MainWindow *window;
    QGraphicsScene *scene;

    ITool *curTool = nullptr; // 此处的tool应该是选择或者某种画图

    qreal rotateAngle;
    QGraphicsRectItem *border;

    QColor curColor;
    QPen curPen;
    QBrush curBrush;

    friend class ITool;
};
#endif