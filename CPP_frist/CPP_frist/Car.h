
#ifndef __CAR_H
#define __CAR_H

#include "OLED.h"

/* 低模汽车：500 个三角形面，整数坐标（X[-11,11] Y[-27,27] Z[2,18]）
 * 调用示例：OLED3D_DrawACar(64, 100, 52, 2, angle, angle, angle, OLED_White);
 * x_c 水平位置(64=居中), y_c 深度, z_c 垂直居中(=32+10*scale), scale 放大倍数 */
void OLED3D_DrawACar(int x_c, int y_c, int z_c, int scale,
    int anglex, int angley, int anglez, Color color);

#endif /* __CAR_H */