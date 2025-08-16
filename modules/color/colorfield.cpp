#include "color/colorfield.h"

#include <QPainter>
#include <QEvent>
#include <QEnterEvent>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>

#include "vision/canvasview.h"

const int ColorWheel::wheelWidth = 45;
const int ColorWheel::wheelThickness = 20;
const int ColorField::previewWidth = 40;
const int ColorField::previewHeight = 20;

ColorWheel::ColorWheel(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(150, 150);
    curColor.setHsvF(0, 1, 0);

    int r = qMin(width(), height()), wheelRadius = r / 2 - wheelThickness / 2;
    wheelRect = QRectF(wheelThickness / 2, wheelThickness / 2, wheelRadius * 2, wheelRadius * 2);
    squareRect = QRectF(wheelWidth, wheelWidth, r - wheelWidth * 2, r - wheelWidth * 2);
}

void ColorWheel::setColor(const QColor &color, bool passive) {
    if (curColor == color) return;
    curColor = color;
    if (!passive) {
        emit colorChanged(curColor);
    }
    update();
}

void ColorWheel::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    drawColorWheel();
    drawColorSquare(curColor.hue());
}

void ColorWheel::drawColorWheel() {
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

void ColorWheel::drawColorSquare(int hue) {
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

bool ColorWheel::isInWheel(const QPointF &pos) const {
    qreal distance = QLineF(pos, wheelRect.center()).length();
    qreal outerRadius = wheelRect.width() / 2 + wheelThickness / 2;
    qreal innerRadius = wheelRect.width() / 2 - wheelThickness / 2;
    return distance <= outerRadius && distance >= innerRadius;
}

bool ColorWheel::isInSquare(const QPointF &pos) const {
    return squareRect.contains(pos);
}

void ColorWheel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateWheelGeometry();
}

void ColorWheel::updateWheelGeometry() {
    int r = qMin(width(), height());
    int centerX = width() / 2;
    int centerY = height() / 2;

    wheelRect = QRectF(centerX - r / 2 + wheelThickness / 2, centerY - r / 2 + wheelThickness / 2,
                       r - wheelThickness, r - wheelThickness);

    int squareSize = r - wheelWidth * 2;
    squareRect = QRectF(centerX - squareSize / 2, centerY - squareSize / 2, squareSize, squareSize);
}

void ColorWheel::updateCursor(const QPointF &pos) {
    if (isInWheel(pos) || isInSquare(pos)) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void ColorWheel::mousePressEvent(QMouseEvent *event) {
    auto pos = event->position();
    if (isInWheel(pos)) {
        // 计算颜色
        qreal angle = QLineF(wheelRect.center(), pos).angle();
        QColor newColor = curColor;
        newColor.setHsvF(angle / 360.0, curColor.hsvSaturationF(), curColor.valueF());
        setColor(newColor, false);
    } else if (isInSquare(pos)) {
        qreal hue = curColor.hueF();
        qreal saturation = (pos.x() - squareRect.left()) / squareRect.width();
        qreal value = 1.0 - (pos.y() - squareRect.top()) / squareRect.height();
        QColor newColor;
        newColor.setHsvF(hue, saturation, value);
        setColor(newColor, false);
    }
    updateCursor(event->position());
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event) {
    auto pos = event->position();
    if (event->buttons() & Qt::LeftButton) {
        mousePressEvent(event); // 处理颜色更新
    }
    updateCursor(pos);
    QWidget::mouseMoveEvent(event);
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event) {
    updateCursor(event->position());
    QWidget::mouseReleaseEvent(event);
}

void ColorWheel::enterEvent(QEnterEvent *event) {
    updateCursor(event->position());
    QWidget::enterEvent(event);
}

void ColorWheel::leaveEvent(QEvent *event) {
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);
}

ColorField::ColorField(QWidget *parent) : QWidget(parent) {
    setCursor(Qt::ArrowCursor);
    setMouseTracking(true);
    setMinimumHeight(300);
    curColor.setHsvF(0, 1, 0);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 上方色轮
    wheelArea = new ColorWheel(this);

    // 左下方 RGB 输入
    auto *leftBottomLayout = new QFormLayout();
    rSpin = new QSpinBox();
    gSpin = new QSpinBox();
    bSpin = new QSpinBox();
    for (auto *spin : {rSpin, gSpin, bSpin}) {
        spin->setRange(0, 255);
        spin->setSingleStep(1);
        spin->setFixedWidth(80);
    }
    leftBottomLayout->addRow("R:", rSpin);
    leftBottomLayout->addRow("G:", gSpin);
    leftBottomLayout->addRow("B:", bSpin);

    // 右下方色彩预览和 RGB 十六进制值
    auto *rightBottomLayout = new QVBoxLayout();
    colorPreview = new QLabel();
    colorPreview->setFixedSize(previewWidth, previewHeight);
    colorPreview->setStyleSheet("background: black; border: 1px solid black;");
    hexEdit = new QLineEdit();
    hexEdit->setReadOnly(true);
    rightBottomLayout->addWidget(colorPreview);
    rightBottomLayout->addWidget(hexEdit);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addLayout(leftBottomLayout);
    bottomLayout->addLayout(rightBottomLayout);

    mainLayout->addWidget(wheelArea);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();

    connect(rSpin, &QSpinBox::textChanged, [this]() {
        QColor c(rSpin->text().toInt(), gSpin->text().toInt(), bSpin->text().toInt());
        setColor(c);
    });
    connect(gSpin, &QSpinBox::textChanged, [this]() {
        QColor c(rSpin->text().toInt(), gSpin->text().toInt(), bSpin->text().toInt());
        setColor(c);
    });
    connect(bSpin, &QSpinBox::textChanged, [this]() {
        QColor c(rSpin->text().toInt(), gSpin->text().toInt(), bSpin->text().toInt());
        setColor(c);
    });
    connect(wheelArea, &ColorWheel::colorChanged, [this](QColor col) { setColor(col); });
}

void ColorField::updateUIFromColor() {
    for (auto *spin : {rSpin, gSpin, bSpin}) {
        spin->blockSignals(true);
    }
    rSpin->setValue(curColor.red());
    gSpin->setValue(curColor.green());
    bSpin->setValue(curColor.blue());
    for (auto *spin : {rSpin, gSpin, bSpin}) {
        spin->blockSignals(false);
    }
    hexEdit->setText(curColor.name());
    colorPreview->setStyleSheet(
        QString("background: %1; border: 1px solid black;").arg(curColor.name()));
}

void ColorField::setColor(const QColor &color) {
    if (curColor == color) return;
    curColor = color;
    wheelArea->setColor(color, true);
    updateUIFromColor();
    emit colorChanged(color);
    update();
}