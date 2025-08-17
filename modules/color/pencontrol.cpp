#include "color/pencontrol.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

PenControl::PenControl(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    curPen = QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);

    // 画笔宽度控制
    QLabel *widthTitle = new QLabel(tr("画笔宽度:"));
    auto *widthLayout = new QHBoxLayout();
    widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setRange(1, 60);
    widthSlider->setValue(2);
    widthSpin = new QSpinBox();
    widthSpin->setRange(1, 60);
    widthSpin->setValue(2);
    widthSpin->setFixedWidth(80);
    widthPressureCheck = new QCheckBox(tr("压感控制"));

    // 不透明度控制
    QLabel *opacityTitle = new QLabel(tr("不透明度:"));
    auto *opacityLayout = new QHBoxLayout();
    opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(1, 100);
    opacitySlider->setValue(100);
    opacitySpin = new QSpinBox();
    opacitySpin->setRange(1, 100);
    opacitySpin->setValue(100);
    opacitySpin->setFixedWidth(80);
    opacitySpin->setSuffix("%");
    opacityPressureCheck = new QCheckBox(tr("压感控制"));

    connect(widthSlider, &QSlider::valueChanged, [this](int value) {
        if (widthSpin->value() != value) {
            widthSpin->setValue(value);
            curPen.setWidth(value);
            emit penChanged(curPen);
        }
    });
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        if (widthSlider->value() != value) {
            widthSlider->setValue(value);
            curPen.setWidth(value);
            emit penChanged(curPen);
        }
    });
    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        curPen.setWidth(value);
        emit penChanged(curPen);
    });
    connect(opacitySlider, &QSlider::valueChanged, [this](int value) {
        if (opacitySpin->value() != value) {
            opacitySpin->setValue(value);
            QColor curColor = curPen.color();
            curColor.setAlphaF(value / 100.0);
            curPen.setColor(curColor);
            emit penChanged(curPen);
        }
    });
    connect(opacitySpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        if (opacitySlider->value() != value) {
            opacitySlider->setValue(value);
            QColor curColor = curPen.color();
            curColor.setAlphaF(value / 100.0);
            curPen.setColor(curColor);
            emit penChanged(curPen);
        }
    });
    connect(opacitySpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        QColor curColor = curPen.color();
        curColor.setAlphaF(value / 100.0);
        curPen.setColor(curColor);
        emit penChanged(curPen);
    });

    // 连接压感控制信号
    connect(widthPressureCheck, &QCheckBox::toggled, [this](bool checked) {
        widthPressure = checked;
        emit widthPressureChanged(widthPressure);
    });
    connect(opacityPressureCheck, &QCheckBox::toggled, [this](bool checked) {
        opacityPressure = checked;
        emit opacityPressureChanged(opacityPressure);
    });

    // 布局
    widthLayout->addWidget(widthSlider);
    widthLayout->addWidget(widthSpin);
    opacityLayout->addWidget(opacitySlider);
    opacityLayout->addWidget(opacitySpin);

    mainLayout->addWidget(widthTitle);
    mainLayout->addLayout(widthLayout);
    mainLayout->addWidget(widthPressureCheck);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(opacityTitle);
    mainLayout->addLayout(opacityLayout);
    mainLayout->addWidget(opacityPressureCheck);

    this->setMaximumHeight(250);
    emit penChanged(curPen);
}

void PenControl::changeColor(QColor newColor) {
    newColor.setAlphaF(curPen.color().alphaF());
    curPen.setColor(newColor);
    emit penChanged(curPen);
}

/// @brief 显式通知画笔更新
void PenControl::updatePen() {
    emit penChanged(curPen);
    emit widthPressureChanged(widthPressure);
    emit opacityPressureChanged(opacityPressure);
}