#ifndef COLOR_FIELD_H
#define COLOR_FIELD_H

#include <QWidget>
#include <QColor>

class ColorField : public QWidget {
    Q_OBJECT

public:
    explicit ColorField(QWidget *parent = nullptr);

    QColor color() const { return curColor; }
    void setColor(const QColor &color);

signals:
    void colorChanged(QColor newColor);

protected:
    void paintEvent(QPaintEvent *event) override;

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    bool isInWheel(const QPointF &pos) const;
    bool isInSquare(const QPointF &pos) const;
    void drawColorWheel();
    void drawColorSquare(int hue);
    void drawColorHint();
    void updateCursor(const QPointF &pos);
    void updateWheelGeometry();

    QColor curColor;

    static const int wheelWidth;
    static const int wheelThickness;
    static const int hintWidth;
    static const int hintHeight;

    QRectF wheelRect, squareRect;
};

#endif