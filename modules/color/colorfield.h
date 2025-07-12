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

protected:
    void paintEvent(QPaintEvent *event) override;

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void drawColorWheel();
    void drawColorSquare(int hue);

    QColor curColor;

    static const int wheelWidth;
    static const int wheelThickness;
};

#endif