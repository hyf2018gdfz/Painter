#include "color/colorfield.h"

#include <QPainter>
#include <QEvent>
#include <QEnterEvent>

#include "vision/canvasview.h"

const int ColorField::wheelWidth = 45;
const int ColorField::wheelThickness = 20;
const int ColorField::hintWidth = 20;
const int ColorField::hintHeight = 20;

ColorField::ColorField(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(150, 150);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(true);
    curColor = Qt::red;

    int r = qMin(width(), height()), wheelRadius = r / 2 - wheelThickness / 2;
    wheelRect = QRectF(wheelThickness / 2, wheelThickness / 2, wheelRadius * 2, wheelRadius * 2);
    squareRect = QRectF(wheelWidth, wheelWidth, r - wheelWidth * 2, r - wheelWidth * 2);
}

void ColorField::setColor(const QColor &color) {
    if (curColor == color) return;
    curColor = color;
    emit colorChanged(curColor);
}

void ColorField::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    drawColorWheel();
    drawColorSquare(curColor.hue());
    drawColorHint();
}

void ColorField::drawColorWheel() {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QConicalGradient gradient(wheelRect.center(), 0);
    for (int i = 0; i < 12; ++i) {
        gradient.setColorAt(i / 12.0, QColor::fromHsvF(i / 12.0, 1.0, 1.0));
    }

    QPen pen;
    pen.setWidth(wheelThickness);
    pen.setBrush(gradient);
    painter.setPen(pen);
    painter.drawEllipse(wheelRect);
}

void ColorField::drawColorSquare(int hue) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 水平饱和度渐变叠加垂直明度渐变
    QLinearGradient saturationGradient(squareRect.left(), 0, squareRect.right(), 0);
    saturationGradient.setColorAt(0, QColor::fromHsv(hue, 0, 255));
    saturationGradient.setColorAt(1, QColor::fromHsv(hue, 255, 255));
    QLinearGradient valueGradient(0, squareRect.top(), 0, squareRect.bottom());
    valueGradient.setColorAt(0, Qt::transparent);
    valueGradient.setColorAt(1, Qt::black);
    painter.fillRect(squareRect, saturationGradient);
    painter.fillRect(squareRect, valueGradient);
}

void ColorField::drawColorHint() {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int hintSize = 40;
    QRect hintRect(width() - hintSize - 10, height() - hintSize - 10, hintSize, hintSize);

    painter.fillRect(hintRect, curColor);

    QPen pen(Qt::black, 1);
    painter.setPen(pen);
    painter.drawRect(hintRect);
}

bool ColorField::isInWheel(const QPointF &pos) const {
    qreal distance = QLineF(pos, wheelRect.center()).length();
    qreal outerRadius = wheelRect.width() / 2 + wheelThickness / 2;
    qreal innerRadius = wheelRect.width() / 2 - wheelThickness / 2;
    return distance <= outerRadius && distance >= innerRadius;
}

bool ColorField::isInSquare(const QPointF &pos) const {
    return squareRect.contains(pos);
}

void ColorField::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateWheelGeometry();
}

void ColorField::updateWheelGeometry() {
    int r = qMin(width(), height());
    int centerX = width() / 2;
    int centerY = height() / 2;

    wheelRect = QRectF(centerX - r / 2 + wheelThickness / 2, centerY - r / 2 + wheelThickness / 2,
                       r - wheelThickness, r - wheelThickness);

    int squareSize = r - wheelWidth * 2;
    squareRect = QRectF(centerX - squareSize / 2, centerY - squareSize / 2, squareSize, squareSize);
}

void ColorField::updateCursor(const QPointF &pos) {
    if (isInWheel(pos) || isInSquare(pos)) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void ColorField::mousePressEvent(QMouseEvent *event) {
    auto pos = event->position();
    if (isInWheel(pos)) {
        // 计算颜色
        qreal angle = QLineF(wheelRect.center(), event->position()).angle();
        QColor newColor = curColor;
        newColor.setHsvF(angle / 360.0, curColor.hsvSaturationF(), curColor.valueF());
        update();
        setColor(newColor);
    } else if (isInSquare(pos)) {
        qreal hue = curColor.hueF();
        qreal saturation = (event->position().x() - squareRect.left()) / squareRect.width();
        qreal value = 1.0 - (event->position().y() - squareRect.top()) / squareRect.height();
        QColor newColor;
        newColor.setHsvF(hue, saturation, value);
        update();
        setColor(newColor);
    }
    updateCursor(event->position());
}

void ColorField::mouseMoveEvent(QMouseEvent *event) {
    auto pos = event->position();
    if (event->buttons() & Qt::LeftButton) {
        mousePressEvent(event); // 处理颜色更新
    }
    updateCursor(pos);
    QWidget::mouseMoveEvent(event);
}

void ColorField::mouseReleaseEvent(QMouseEvent *event) {
    updateCursor(event->position());
    QWidget::mouseReleaseEvent(event);
}

void ColorField::enterEvent(QEnterEvent *event) {
    updateCursor(event->position());
    QWidget::enterEvent(event);
}

void ColorField::leaveEvent(QEvent *event) {
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);
}