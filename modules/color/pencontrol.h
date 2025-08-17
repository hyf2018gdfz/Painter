#ifndef PEN_CONTROL_H
#define PEN_CONTROL_H

#include <QWidget>
#include <QSlider>
#include <QCheckBox>
#include <QPen>
#include <QLabel>
#include <QSpinBox>

/// @brief 主要是针对画笔的控制面板
class PenControl : public QWidget {
    Q_OBJECT

public:
    explicit PenControl(QWidget *parent = nullptr);
    void updatePen();

signals:
    void penChanged(QPen newPen);
    void widthPressureChanged(bool enabled);
    void opacityPressureChanged(bool enabled);

public slots:
    void changeColor(QColor newColor);

private:
    QSlider *widthSlider;
    QSpinBox *widthSpin;
    QSlider *opacitySlider;
    QSpinBox *opacitySpin;
    QCheckBox *widthPressureCheck;
    QCheckBox *opacityPressureCheck;
    bool widthPressure = false;
    bool opacityPressure = false;
    QPen curPen;
};

#endif