#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include <string.h>
#include "stdio.h"
#include <math.h>
#include <Windows.h>

#define WINDOWS_LENGTH_MAX 2700
#define WINDOWS_HIGH_MAX 1500


//#define WINDOWS_LENGTH 640
//#define WINDOWS_HIGH 480

#define WHITEPOINT 0
#define COLORPOINT 1

// typedef enum{
//	OLED_Black = 0,
//	OLED_White = 1,
//	OLED_Contr = 2
//}Color;

#define OLED_Black 0
#define OLED_White 0xffffff
#define OLED_Contr 

#define  Color uint32_t

#define COLOR_RED      RGB(255, 0,   0)    // 后面 红色
#define COLOR_GREEN    RGB(0,   255, 0)    // 前面 绿色
#define COLOR_BLUE     RGB(0,   0,   255)  // 右面 蓝色
#define COLOR_YELLOW   RGB(255, 255, 0)    // 左面 黄色
#define COLOR_PURPLE   RGB(180, 0,   255)  // 上面 紫色
#define COLOR_ORANGE   RGB(255, 120, 0)    // 下面 橙色

void ReCreateCanvas(int w, int h);

void OLED_ShowPicture(void);
void OLED_DrawInit(HWND hWnd);
void OLED_DrawClear(int x, int y, int x1, int y1, Color color);
void OLED_Update(HWND hWnd);
void OLED_DrawAPoint(int x, int y, Color color);

void OLED_ShowRowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowMinChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowMinString(uint8_t Line, uint8_t Column, char *String);


void OLEDMin_DrawAWord(int x,int y,char CH,Color color);
void OLEDMin_DrawARatateWord(int x,int y,float angle,char CH,Color color);
void OLEDMin_AMPDrawAWord(int x,int y,char CH,Color color,uint8_t num);

void OLEDMin_DrawARatateString(int x,int y,float angle,char * CH,Color color);

void OLEDMin_DrawARatateNum(int x,int y,float angle,int num,int length,Color color);
void OLEDMin_DrawANum(int x,int y,int num,int length,Color color);
void OLEDMin_DrawAString(int x,int y,char * CH,Color color);


void OLED_DrawAPolygon(int x,int y,int side, int lenght , float angle,Color color);
void OLED_DrawARound(int x_a,int y_a,int r,int r_a,Color color);
void OLED_DrawAThriangle(int x_a,int y_a,int x_1,int y_1,int x_2,int y_2,Color color);
void OLED_DrawAquard(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,Color color);
void OLED_DrawAGeDiao(void);

void OLED_DrawAWord(int x,int y,char CH,Color color);
void OLED_AMPDrawAWord(int x,int y,char CH,Color color,uint8_t num);
void OLED_DrawARatateWord(int x,int y,float angle,char CH,Color color);
void OLED_AMPDrawARatateWord(int x,int y,float angle ,char CH,Color color,uint8_t num);

void OLED_DrawAString(int x,int y,char * CH,Color color);
void OLED_DrawARatateString(int x,int y,float angle,char* CH,Color color);

void OLED_DrawARatateNum(int x,int y,float angle,int num,int length,Color color);
void OLED_DrawANum(int x,int y,int num,int length,Color color);

void OLED3D_DrawATriangleFace(int x_c, int y_c, int z_c, int x_1, int y_1, int z_1, int x_2, int y_2, int z_2, int x_3, int y_3, int z_3, Color color);
void OLED3D_DrawARotateTriangleFace(int x_c, int y_c, int z_c, int x_1, int y_1, int z_1, int x_2, int y_2, int z_2, int x_3, int y_3, int z_3, int angle_x, int angle_y, int angle_z, Color color);


void rotate_point(float cx, float cy, float angle, float	x, 	float y, int *out_x, int *out_y);
void rotate_point_Rad(float 	cx, float   cy, float rad, float		 x, float    y, float* out_x, float* out_y);
void rotate_point_float(float cx, float   cy, float angle, float x, float y, float* out_x, float* out_y);
void OLED_DrawAAixs(int x, int y, int wide, int high, int cyc, int* data, int16_t num, uint16_t t, Color color, int AMP, int fangxiang);

void OLED_DrawALine(int x,int y,int x1,int y1,Color color);
void OLED_DrawALine_Wide(int x, int y, int x1, int y1, int wide, Color color);
void OLED_DrawALine_For_Axis(int x,int y,int x1,int y1,Color color);

void OLED_DrawAClock(int h,int min,int scend);

void OLED3D_DrawASquareFace(int x_c, int y_c, int z_c, int x_1, int y_1, int z_1, int x_2, int y_2, int z_2, int x_3, int y_3, int z_3, int x_4, int y_4, int z_4, Color color);
void OLED3D_DrawARotateSquareFace(int x_c, int y_c, int z_c, int x_1, int y_1, int z_1, int x_2, int y_2, int z_2, int x_3, int y_3, int z_3, int x_4, int y_4, int z_4, int angle_x, int angle_y, int angle_z, Color color);
void OLED3D_AMPDrawARatateWord(int x, int y, int z, int x_c, int y_c, int z_c, int  anglex, int  angley, int  anglez, char CH, Color color, int num);
void OLED3DTKN_AMPDrawARatateWord(int x, int y, int z, int x_c, int y_c, int z_c, int TKN, int  anglex, int  angley, int  anglez, char CH, Color color, int num);
void OLED3DTKN_AMPDrawARatateString(int x, int y, int z, int x_c, int y_c, int z_c, int TKN, int  anglex, int  angley, int  anglez, char* CH, Color color, int num);
void OLED3D_AMPDrawARatateString(int x, int y, int z, int x_c, int y_c, int z_c, int  anglex, int  angley, int  anglez, char* CH, Color color, int num);
void OLED3D_DrawARound(int x_a, int y_a, int z_a, int r, int  anglex, int  angley, int  anglez, Color color);

void OLED_DrawACube(int x, int y, int z, float anglex, float angley, float anglez, int bianchang);
void OLED_DrawAPen(int x, int y, int wide, Color color);

void Vedio_Updata(int x, int y, int z, float anglex, float angley, float anglez);
#endif