#ifndef COMMON_H
#define COMMON_H

#include <QGraphicsItem>

// 工具类型枚举
enum class ToolType {
    FREEHAND,
    RECTANGLE,
    TRIANGLE,
    ELLIPSE,
    POLYGON,
    LINE,
    SELECT,
    ZOOMIN,
    ZOOMOUT,
    ROTATEVIEWCW,
    ROTATEVIEWCCW,
    COMBINE_SELECTED,
    DELETE_SELECTED,
};

enum class CATEGORY {
    SELECT_TOOL,
    DEPENDENT_SELECTION,  // 依赖选中（如组合/删除）
    EXCLUSIVE_OPERATION,  // 排斥选中（如绘图）
    INDEPENDENT_OPERATION // 独立操作（如视图旋转）
};

QRectF makeNormalizedRect(const QPointF &p1, const QPointF &p2);

#endif