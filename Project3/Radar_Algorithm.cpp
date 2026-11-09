#include "Radar_Algorithm.h"
#include <cstdlib>
#include <cstring>

namespace Radar_Algorithm {

// ---- 欧拉公式 ----
Complex Euler_Formula(double a) {
    return Complex(cos(a), sin(a));
}

// ---- 位逆序 (MSB方向进位, O(N)) ----
void Bit_Reverse(Complex* x, int n) {
    for (int i = 1, j = 0; i < n; i++) {
        int k;
        for (k = n >> 1; j >= k; k >>= 1) j -= k;  // 连续1→0
        j += k;                                       // 第一个0→1
        if (i < j) { Complex t = x[i]; x[i] = x[j]; x[j] = t; }
    }
}

// ---- 复数取模 ----
double ABV(Complex a) {
    return sqrt(a.real * a.real + a.imagi * a.imagi);
}

// ---- 汉明窗 ----
void Hamming_Window(Complex* window, int n) {
    for (int i = 0; i < n; i++)
        window[i] = Complex(0.54 - 0.46 * cos(2 * PI * i / (n - 1)), 0);
}

// ---- 一维 FFT ----
// 行: 连续读; 列: 跨步 n
void FFT_1D(Complex* in, int m, Complex* out, FFT_Side side, FFT_Direction dir) {
    int n = 1 << m;
    Complex* reg = (Complex*)malloc(sizeof(Complex) * n);
    memset(reg, 0, sizeof(Complex) * n);

    if (side == FFT_Widgth)
        for (int i = 0; i < n; i++) reg[i] = in[i];
    else
        for (int i = 0; i < n; i++) reg[i] = in[i * n];

    Bit_Reverse(reg, n);

    // 蝴蝶循环: log2(N) 级
    for (int s = 1, len = 1, x = 0; s <= m; s++) {
        for (int j = 0; j < n / 2; j++) {
            Complex u = reg[x];
            Complex w = Euler_Formula(dir * 2 * PI * (x % (2 * len)) / (2 * len));  // 旋转因子
            Complex t = reg[x + len] * w;
            out[x] = reg[x] = u + t;           // 和 = 左翼 + 右翼×W
            out[x + len] = reg[x + len] = u - t; // 差 = 左翼 - 右翼×W

            x += ((j + 1) % len) ? 1 : (len + 1);  // 跳跃到下一蝶
        }
        x = 0; len *= 2;
    }
    if (dir == FFT_Retrograde){
        if (side == FFT_Widgth)
            for (int i = 0; i < n; i++) in[i] = reg[i]/n;
        else
            for (int i = 0; i < n; i++) in[i * n] = reg[i]/n;
    }
    else {
        if (side == FFT_Widgth)
            for (int i = 0; i < n; i++) in[i] = reg[i];
        else
            for (int i = 0; i < n; i++) in[i * n] = reg[i];
    }
    free(reg);
}

// ---- 二维 FFT ----
// 正变换: 先行后列; 逆变换: 先列后行
void FFT_2D(Complex* in, int m, Complex* out, FFT_Direction dir) {
    int n = 1 << m;
    if (dir == FFT_Prograde) {
        for (int i = 0; i < n; i++) FFT_1D(in + n * i, m, out + n * i, FFT_Widgth, dir);
        for (int i = 0; i < n; i++) FFT_1D(out + i, m, out + i, FFT_Heigth, dir);
    } else {
        for (int i = 0; i < n; i++) FFT_1D(out + i, m, out + i, FFT_Heigth, dir);
        for (int i = 0; i < n; i++) FFT_1D(in + n * i, m, out + n * i, FFT_Widgth, dir);
    }
}

void CVL(Complex* in, int m, Complex* out, FFT_Direction dir) {
    int n = 1 << m;
    Complex** CVL_Reg = (Complex**)malloc(n * sizeof(Complex*));
    Complex *reg= (Complex*)malloc(n * n * sizeof(Complex));
    memset(reg, 0, n * n * sizeof(Complex));
    for (int i = 0; i < n; i++) { 
        CVL_Reg[i] = &reg[i * n];
    }
    CVL_Reg[0][0].real = 1;CVL_Reg[0][1].real = 2;CVL_Reg[0][2].real = 1;
    CVL_Reg[1][0].real = 2;CVL_Reg[1][1].real = 4;CVL_Reg[1][2].real = 2;
    CVL_Reg[2][0].real = 1;CVL_Reg[2][1].real = 2;CVL_Reg[2][2].real = 1;
    FFT_2D(in, m, out, FFT_Prograde);
    FFT_2D(reg, m, reg, FFT_Prograde);
    for (int i = 0;i < n*n;i++) {
        out[i] = out[i] * reg[i]/16;
    }
    FFT_2D(out, m, out, FFT_Retrograde);
    free(reg);
    free(CVL_Reg);
}

}
