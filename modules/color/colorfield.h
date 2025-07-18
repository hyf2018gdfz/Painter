#ifndef COLOR_FIELD_H
#define COLOR_FIELD_H

#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QLabel>

class ColorWheel : public QWidget {
    Q_OBJECT
public:
    explicit ColorWheel(QWidget *parent = nullptr);
    QColor color() const { return curColor; }
    void setColor(const QColor &color, bool passive = true);

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
    void updateWheelGeometry();
    void updateCursor(const QPointF &pos);

    QColor curColor;

    QRectF wheelRect, squareRect;

    static const int wheelWidth;
    static const int wheelThickness;
};

class ColorField : public QWidget {
    Q_OBJECT

public:
    explicit ColorField(QWidget *parent = nullptr);

    QColor color() const { return curColor; }
    void setColor(const QColor &color);

signals:
    void colorChanged(QColor newColor);

private:
    void updateUIFromColor();

    QColor curColor;

    static const int previewWidth;
    static const int previewHeight;

    ColorWheel *wheelArea;

    QSpinBox *rSpin, *gSpin, *bSpin;
    QLabel *colorPreview;
    QLineEdit *hexEdit;
};

#endif