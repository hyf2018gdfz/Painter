#include "color/colorfield.h"

#include <QPainter>
#include <QEvent>

const int ColorField::wheelWidth = 45;
const int ColorField::wheelThickness = 20;

ColorField::ColorField(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(150, 150);
    setCursor(Qt::ArrowCursor);
    curColor = Qt::red;
}

void ColorField::setColor(const QColor &color) {
    if (curColor == color) return;
}

void ColorField::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    drawColorWheel();
    drawColorSquare(curColor.hue());
}

void ColorField::drawColorWheel() {
    int r = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect(wheelThickness / 2, wheelThickness / 2, r - wheelThickness, r - wheelThickness);

    QConicalGradient gradient(rect.center(), 0);
    for (int i = 0; i < 12; ++i) {
        gradient.setColorAt(i / 12.0, QColor::fromHsvF(i / 12.0, 1.0, 1.0));
    }

    QPen pen;
    pen.setWidth(wheelThickness);
    pen.setBrush(gradient);
    painter.setPen(pen);
    painter.drawEllipse(rect);
}

void ColorField::drawColorSquare(int hue) {
    int size = (qMin(width(), height()) - wheelWidth * 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 水平饱和度渐变叠加垂直明度渐变
    QRectF squareRect(wheelWidth, wheelWidth, size, size);
    QLinearGradient saturationGradient(squareRect.left(), 0, squareRect.right(), 0);
    saturationGradient.setColorAt(0, QColor::fromHsv(hue, 0, 255));
    saturationGradient.setColorAt(1, QColor::fromHsv(hue, 255, 255));
    QLinearGradient valueGradient(0, squareRect.top(), 0, squareRect.bottom());
    valueGradient.setColorAt(0, Qt::transparent);
    valueGradient.setColorAt(1, Qt::black);
    painter.fillRect(squareRect, saturationGradient);
    painter.fillRect(squareRect, valueGradient);
}

void ColorField::enterEvent(QEnterEvent *event) {
    this->setCursor(Qt::CrossCursor);
}

void ColorField::leaveEvent(QEvent *event) {
    this->setCursor(Qt::ArrowCursor);
}