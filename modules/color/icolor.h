/// NOTE: 未来需要使用 CIE-Lab 颜色相关的高级功能的时候再启用此文件，现在仅使用 QColor 即可。
// #ifndef I_COLOR_H
// #define I_COLOR_H

// #include <QString>

// struct IColor {
//     // 统一内部存储为 CIE-Lab 浮点值，其它用作缓存
//     qreal L, a, b;

//     qreal R, G, B;
//     QString hex;

//     qreal H, S, B;

//     qreal C, M, Y, K;

//     void updateFromLab(qreal _L, qreal _a, qreal _b);
//     void updateFromRGB(qreal _R, qreal _G, qreal _B);
//     void updateFromHex(const QString &hex);
//     void updateFromHSB(qreal _H, qreal _S, qreal _B);
//     void updateFromCMYK(qreal _C, qreal _M, qreal _Y, qreal _K);
// };

// #endif