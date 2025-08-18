#include "tools/drawtools/drawtool.h"

#include <QApplication>

#include "vision/canvasview.h"
#include "vision/mainwindow.h"

/// INFO:
/// 目前的做法是段内进一步线性细分成多段，但是仍然无法根本上解决放大后看像香肠的问题。
/// 另外，细分带来的性能问题也比较棘手。或许采取其它的细分方式会更好？真搞不明白商业软件怎么做到的

class VariableSegItem : public QGraphicsItem {
public:
    VariableSegItem(const QVector<QPointF> &pts, const QVector<qreal> &pressures,
                    const QPen &basePen, bool widthPressure, bool opacityPressure) :
        m_pts(pts), m_pressures(pressures), m_basePen(basePen), m_widthPressure(widthPressure),
        m_opacityPressure(opacityPressure) {
        prepareGeometry();
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

    QRectF boundingRect() const override { return m_bounds; }

    QPainterPath shape() const override {
        // 针对 tablet 绘制的曲线，shape 设置为曲线+首尾相连的闭合路径
        if (m_pts.isEmpty()) return QPainterPath();

        QPainterPath path;
        path.moveTo(m_pts.first());
        for (int i = 1; i < m_pts.size(); ++i) {
            path.lineTo(m_pts[i]);
        }
        path.lineTo(m_pts.first());
        path.closeSubpath();

        QPainterPathStroker stroker;
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        stroker.setWidth(qMax<qreal>(1.0, m_basePen.widthF())); // 最大宽度≈basePen
        path.addPath(stroker.createStroke(path));
        return path;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setRenderHint(QPainter::Antialiasing);

        if (m_pts.size() < 2) {
            // 单点点击，画一个小圆点
            QPen p = m_basePen;
            p.setCapStyle(Qt::RoundCap);
            p.setJoinStyle(Qt::RoundJoin);

            QColor c = p.color();
            if (m_opacityPressure && !m_pressures.isEmpty()) {
                c.setAlphaF(c.alphaF() * clamp01(m_pressures.first()));
            }
            p.setColor(c);

            qreal w = p.widthF();
            if (m_widthPressure && !m_pressures.isEmpty()) {
                w = segmentWidth(m_pressures.first(), w);
            }
            p.setWidthF(qMax<qreal>(0.5, w));
            painter->setPen(p);
            painter->drawPoint(m_pts.first());
            return;
        }

        QPen p = m_basePen;
        p.setCapStyle(Qt::RoundCap);
        p.setJoinStyle(Qt::RoundJoin);

        const QColor baseColor = m_basePen.color();
        const qreal baseAlpha = baseColor.alphaF();

        for (int i = 1; i < m_pts.size(); ++i) {
            QPointF p0 = m_pts[i - 1];
            QPointF p1 = m_pts[i];
            qreal pr0 = m_pressures.value(i - 1, 1.0);
            qreal pr1 = m_pressures.value(i, 1.0);

            // 在段内插值
            const int steps = 6;
            for (int s = 0; s < steps; ++s) {
                qreal t0 = qreal(s) / steps;
                qreal t1 = qreal(s + 1) / steps;

                QPointF q0 = (1 - t0) * p0 + t0 * p1;
                QPointF q1 = (1 - t1) * p0 + t1 * p1;
                qreal prA = (1 - t0) * pr0 + t0 * pr1;
                qreal prB = (1 - t1) * pr0 + t1 * pr1;

                qreal wA = m_basePen.widthF();
                qreal wB = m_basePen.widthF();
                if (m_widthPressure) {
                    wA = segmentWidth(prA, wA);
                    wB = segmentWidth(prB, wB);
                }

                QColor c = baseColor;
                if (m_opacityPressure) {
                    c.setAlphaF(2 * segmentAlpha((prA + prB) / 2, baseAlpha) / steps);
                }

                p.setWidthF(qMax<qreal>(0.5, 0.5 * (wA + wB)));
                p.setColor(c);
                painter->setPen(p);
                painter->drawLine(q0, q1);
            }
        }

        if (isSelected()) {
            QPen borderPen(Qt::black, 0.5, Qt::DashLine);
            painter->setPen(borderPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(boundingRect());
        }
    }
    void updateSeg(const QVector<QPointF> &pts, const QVector<qreal> &pressures) {
        prepareGeometryChange();
        m_pts = pts;
        m_pressures = pressures;
        prepareGeometry();
        update();
    }

private:
    static inline qreal clamp01(qreal x) { return x < 0 ? 0 : (x > 1 ? 1 : x); }
    static inline qreal segmentWidth(qreal pressure, qreal baseW) {
        return baseW * (0.15 + 0.85 * clamp01(pressure));
    }
    static inline qreal segmentAlpha(qreal pressure, qreal baseAlpha) {
        return segmentWidth(pressure, baseAlpha);
    }

    void prepareGeometry() {
        if (m_pts.isEmpty()) {
            m_bounds = QRectF();
            return;
        }
        qreal minx = m_pts[0].x(), maxx = minx;
        qreal miny = m_pts[0].y(), maxy = miny;
        for (const auto &p : m_pts) {
            minx = qMin(minx, p.x());
            maxx = qMax(maxx, p.x());
            miny = qMin(miny, p.y());
            maxy = qMax(maxy, p.y());
        }
        // 最大可能宽度 ≈ base width（按上面的映射，最大是1.0*base），额外加 2 处理抗锯齿
        qreal half = qMax<qreal>(1.0, m_basePen.widthF()) * 0.5 + 2.0;
        m_bounds = QRectF(QPointF(minx - half, miny - half), QPointF(maxx + half, maxy + half));
    }

private:
    QVector<QPointF> m_pts;
    QVector<qreal> m_pressures;
    QPen m_basePen;
    QRectF m_bounds;
    bool m_widthPressure = false;
    bool m_opacityPressure = false;
};

void FreeHandTool::onMousePress(QMouseEvent *event) {
    if (activeInput == InputSource::Tablet) {
        return;
    }
    auto pos = view()->mapToScene(event->pos());
    if (!isDrawing) {
        activeInput = InputSource::Mouse;
        if (previewSegItem) {
            scene()->removeItem(previewSegItem);
            delete previewSegItem;
            previewSegItem = nullptr;
        }
        isDrawing = true;
        tempPath = QPainterPath();
        tempPath.moveTo(pos);
        previewItem = new QGraphicsPathItem();
        previewItem->setPen(pen());
        scene()->addItem(previewItem);
        segPoints.clear();
        segPressures.clear();
    }
}

void FreeHandTool::onMouseMove(QMouseEvent *event) {
    if (activeInput == InputSource::Tablet) {
        return;
    }
    auto pos = view()->mapToScene(event->pos());
    if (isDrawing && previewItem) {
        tempPath.lineTo(pos);
        previewItem->setPath(tempPath);
    }
}

void FreeHandTool::onMouseRelease(QMouseEvent *event) {
    if (activeInput == InputSource::Tablet) {
        return;
    }
    if (isDrawing && previewItem) {
        auto finalItem = new QGraphicsPathItem(tempPath);
        finalItem->setPen(pen());
        finalItem->setFlags(QGraphicsItem::ItemIsSelectable);
        window()->pushCommand(new AddItemsCommand(scene(), nullptr, finalItem));
        scene()->removeItem(previewItem);
        delete previewItem;
        previewItem = nullptr;
        isDrawing = false;
        tempPath = QPainterPath();
        activeInput = InputSource::None;
        segPoints.clear();
        segPressures.clear();
    }
}

void FreeHandTool::changeWidthPressure(bool enabled) {
    qDebug() << "enable width pressure";
    widthPressure = enabled;
}

void FreeHandTool::changeOpacityPressure(bool enabled) {
    qDebug() << "enable opacity pressure";
    opacityPressure = enabled;
}

/// BUG: 在高速移动的时候，线条采样会呈现粗点、细线……的不均匀现象
void FreeHandTool::onTabletEvent(QTabletEvent *event) {
    if (activeInput == InputSource::Mouse) {
        event->accept();
        return;
    }
    const auto pos = view()->mapToScene(event->position().toPoint());

    switch (event->type()) {
    case QEvent::TabletPress: {
        if (!isDrawing) {
            isDrawing = true;
            activeInput = InputSource::Tablet;
            if (previewItem) {
                scene()->removeItem(previewItem);
                delete previewItem;
                previewItem = nullptr;
            }

            segPoints.clear();
            segPressures.clear();
            segPoints.push_back(pos);
            segPressures.push_back(qBound<qreal>(0.0, event->pressure(), 1.0));

            tempPath = QPainterPath();
            tempPath.moveTo(pos);

            if (previewSegItem) {
                scene()->removeItem(previewSegItem);
                delete previewSegItem;
            }
            previewSegItem =
                new VariableSegItem(segPoints, segPressures, pen(), widthPressure, opacityPressure);
            scene()->addItem(previewSegItem);
        }
        break;
    }
    case QEvent::TabletMove: {
        if (!isDrawing || !previewSegItem) {
            break;
        }

        /// INFO: 或许有更好的采样策略
        const qreal pr = qBound<qreal>(0.0, event->pressure(), 1.0);
        segPoints.push_back(pos);
        segPressures.push_back(pr);
        if (previewSegItem) {
            previewSegItem->updateSeg(segPoints, segPressures);
        }
        break;
    }
    case QEvent::TabletRelease: {
        if (isDrawing && previewSegItem) {
            scene()->removeItem(previewSegItem);
            previewSegItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
            window()->pushCommand(new AddItemsCommand(scene(), nullptr, previewSegItem));
            previewSegItem = nullptr;
            if (previewItem) {
                scene()->removeItem(previewItem);
                delete previewItem;
                previewItem = nullptr;
            }
            activeInput = InputSource::None;
            isDrawing = false;
            segPoints.clear();
            segPressures.clear();
            tempPath = QPainterPath();
        }
        break;
    }
    default: break;
    }
    event->accept();
}