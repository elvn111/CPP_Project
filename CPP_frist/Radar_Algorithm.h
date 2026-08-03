#pragma once
#include <cmath>

#define PI 3.1415926

namespace Radar_Algorithm {

/// @brief 复数类型, 支持 +-*/、共轭、正负号
struct Complex {
    double real, imagi;
    Complex(double r = 0, double i = 0) : real(r), imagi(i) {}

    // ---- 加法 ----
    Complex operator+(const Complex& b) const { return Complex(real + b.real, imagi + b.imagi); }
    friend Complex operator+(double a, const Complex& b) { return Complex(a, 0) + b; }
    Complex operator+(double b) const { return Complex(real + b, imagi); }
    Complex operator+() const { return *this; }

    // ---- 减法 ----
    Complex operator-(const Complex& b) const { return Complex(real - b.real, imagi - b.imagi); }
    friend Complex operator-(double a, const Complex& b) { return Complex(a, 0) - b; }
    Complex operator-(double b) const { return Complex(real - b, imagi); }
    Complex operator-() const { return Complex(-real, -imagi); }

    // ---- 共轭 ----
    Complex operator~() const { return Complex(real, -imagi); }

    // ---- 乘法 ----
    Complex operator*(const Complex& b) const {
        return Complex(real * b.real - imagi * b.imagi, imagi * b.real + real * b.imagi);
    }
    friend Complex operator*(double a, const Complex& b) { return Complex(a, 0) * b; }
    Complex operator*(double b) const { return *this * Complex(b, 0); }

    // ---- 除法 (分子乘分母共轭, 再除模长平方) ----
    Complex operator/(const Complex& b) const {
        return (*this) * ~b * Complex(1 / (b.imagi * b.imagi + b.real * b.real), 0);
    }
    friend Complex operator/(double a, const Complex& b) { return Complex(a, 0) / b; }
    Complex operator/(double b) const { return *this / Complex(b, 0); }
};

struct Matrix {

};

/// @brief FFT 方向: 正变换(DFT) 或 逆变换(IDFT)
enum FFT_Direction {
    FFT_Prograde = -1,   ///< 正变换 e^(-jθ)
    FFT_Retrograde = 1   ///< 逆变换 e^(+jθ)
};

/// @brief FFT 维度: 行 或 列
enum FFT_Side {
    FFT_Heigth = -1,     ///< 列 (按 in[i*stride] 跨步读)
    FFT_Widgth = 1       ///< 行 (连续读)
};

/// @brief 欧拉公式 e^(j*angle) = cos + j*sin
Complex Euler_Formula(double angle);

/// @brief 位逆序重排, O(N), 不额外分配
void Bit_Reverse(Complex* x, int n);

/// @brief 复数取模
double ABV(Complex a);

/// @brief 一维 FFT
/// @param in   输入数组
/// @param m    点数 = 1 << m
/// @param out  输出数组
/// @param side 行(FFT_Widgth) 或 列(FFT_Heigth)
/// @param dir  正/逆变换
void FFT_1D(Complex* in, int m, Complex* out, FFT_Side side, FFT_Direction dir);

/// @brief 二维 FFT (n×n, 先行后列或先列后行)
/// @param dir 正变换: 先行后列; 逆变换: 先列后行
void FFT_2D(Complex* in, int m, Complex* out, FFT_Direction dir);

void CVL(Complex* in, int m, Complex* out);

void Gaussian_Filter(Complex* in, int m, Complex* out);

}
