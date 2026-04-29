#include <stdint.h>
#include "OLED_Font.h"
#include <string.h>
#include "stdio.h"
#include <math.h>
#include "OLED.h"
#include <Windows.h>

extern HDC hdc;


static HWND      g_hWnd;
static HDC       g_hdc;
static HDC       g_memDC;
static HBITMAP   g_hBmp;
BYTE* g_frameBuf;   // 显存缓冲区 BGRA 每像素4字节

COLORREF OLEDPicture[WINDOWS_LENGTH_MAX][WINDOWS_HIGH_MAX] = { 0 };

unsigned int  WINDOWS_LENGTH = 640;
unsigned int WINDOWS_HIGH = 480;

int16_t OLED_Sample_value[WINDOWS_LENGTH_MAX*2][4] = { 0 };

void ReCreateCanvas(int w, int h)
{
	// 1. 释放旧资源，防止内存泄漏
	if (g_hBmp)   DeleteObject(g_hBmp);
	if (g_memDC)  DeleteDC(g_memDC);

	// 2. 更新画布尺寸

	// 3. 重新初始化 BITMAPINFO
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = w;
	bi.bmiHeader.biHeight = -h; // 从上到下
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	// 4. 新建内存DC + 新画布 + 新frameBuf
	g_memDC = CreateCompatibleDC(NULL);
	g_hBmp = CreateDIBSection(g_memDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuf, NULL, 0);
	SelectObject(g_memDC, g_hBmp);
}


void OLED_DrawInit(HWND hWnd)
{
	g_hWnd = hWnd;
	g_hdc = GetDC(g_hWnd);

	// 创建内存画布
	g_memDC = CreateCompatibleDC(g_hdc);

	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = WINDOWS_LENGTH_MAX;
	bi.bmiHeader.biHeight = -(int)WINDOWS_HIGH_MAX; // 从上到下
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	g_hBmp = CreateDIBSection(g_memDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuf, NULL, 0);
	SelectObject(g_memDC, g_hBmp);
}


/**
  * @brief  在缓冲区上打一个点
  * @param  x，横坐标
  * @param  y，纵坐标
  * @param  color 颜色
  * @retval 无
  */

void OLED_DrawAPoint(int x, int y, Color color)
{
	if (x < 0 || x >= WINDOWS_LENGTH || y < 0 || y >= WINDOWS_HIGH)
		return;

	// Windows 像素排布：B G R A
	BYTE a = (color >> 24) & 0xFF;
	BYTE b = (color >> 16) & 0xFF;
	BYTE g = (color >> 8) & 0xFF;
	BYTE r = color & 0xFF;

	UINT idx = (UINT)y * WINDOWS_LENGTH_MAX * 4 + (UINT)x * 4;
	if (a)
	{
		if ((g_frameBuf[idx + 0] == 0) && (g_frameBuf[idx + 1] == 0) && (g_frameBuf[idx + 2] == 0))
		{
			g_frameBuf[idx + 0] = b;//blue
			g_frameBuf[idx + 1] = g ;//green
			g_frameBuf[idx + 2] = r;//red
			g_frameBuf[idx + 3] = 255;//a
		}
	}
	else
	{
		g_frameBuf[idx + 0] = b;//blue
		g_frameBuf[idx + 1] = g;//green
		g_frameBuf[idx + 2] = r;//red
		g_frameBuf[idx + 3] = 255;//a
	}
}

/**
  * @brief  在缓冲区上打一排点
  * @param  x，横坐标
  * @param  y，纵坐标
  * @param  color 颜色
  * @retval 无
  */
void OLED_AMPDrawAPoint(int x, int y,int num, Color color)
{
	for (int i = 0;i < num; i++)
	{
		OLED_DrawAPoint(x, y + i, color);
	}
	
}

/**
  * @brief  在缓冲区上打一个Byte
  * @param  x，横坐标
  * @param  y，纵坐标
  * @param Byte，byte
  * @param  color 颜色
  * @retval 无
  */
void OLED_DrawAByte(int x, int y, int Byte, Color color)
{
	for (int i = 0;i < 8;i++)
	{
		if (Byte & (1 << i))
			OLED_DrawAPoint(x, y + i, color);
	}
}

void OLED_Update(HWND hWnd)
{
	//HDC hdc = GetDC(hWnd);
	//BitBlt(hdc, 0, 0, WINDOWS_LENGTH, WINDOWS_HIGH, g_memDC, 0, 0, SRCCOPY);
	//ReleaseDC(hWnd, hdc);
}

void OLED_DrawClear(int x,int y,int x1,int y1,Color color)
{
	int w=x1-x, h=y1-y;
	UINT idx = (UINT)y * WINDOWS_LENGTH * 4 + (UINT)x * 4;
	for (int i=0;i < h;i++)
	{
		memset(g_frameBuf+(idx+WINDOWS_LENGTH*i)*4, color, (size_t)w* 4);
	}
}

/**
  * @brief  OLED显示一帧画面
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowPicture(void){
	//StretchBlt(
	//	g_hdc,
	//	0, 0,
	//	WINDOWS_LENGTH, WINDOWS_HIGH,
	//	g_memDC,
	//	0, 0,
	//	WINDOWS_LENGTH_MAX, WINDOWS_HIGH_MAX,
	//	SRCCOPY
	//);
	BitBlt(
		g_hdc,
		0, 0,
		WINDOWS_LENGTH, WINDOWS_HIGH,
		g_memDC,
		0, 0,
		SRCCOPY
	);
}

/**
  * @brief  OLED显示地平线,地平线底下的像素亮起
  * @param  x x轴加速度，范围：-20000~20000
  * @param  y y轴加速度，范围：-20000~20000
  * @param  z z轴加速度，范围：-20000~20000
  * @retval 无
  */
//void OLED_Horizon(int X,int Y,int Z){
//	int Axis_k=0;
//	int Axis_h=((Z+20000)*32/20000)%WINDOWS_HIGH;
//	int Axis_l=Axis_h;
//	uint64_t Fx16 = (uint64_t)-1;
//	memset(OLEDPicture[0],0,sizeof(int)* WINDOWS_HIGH * WINDOWS_LENGTH);
//    for(int i=0;i<WINDOWS_LENGTH;i++){
//		if(X<=0){
//			if(Y>=0){
//				Axis_k=((i-Axis_l*2)*Y/X)+(Axis_h%WINDOWS_HIGH);
//			}else{
//				Axis_k=((i-WINDOWS_LENGTH+Axis_l*2)*Y/X)+(Axis_h % WINDOWS_HIGH);
//			}
//            if(Axis_k<0){
//				continue;
//			}else if(Axis_k>WINDOWS_HIGH){
//                memset(OLEDPicture[i],0xff,s);
//			}else{
//                OLEDPicture_total.OLEDPicture_64[i]=Fx16<<(WINDOWS_HIGH-Axis_k);
//			}
//		}else{
//			if(Y>=0){
//                Axis_k=((i-Axis_l*2)*Y/X)+((WINDOWS_HIGH-Axis_h)%WINDOWS_HIGH);
//			}else{
//                Axis_k=((i-WINDOWS_LENGTH+Axis_l*2)*Y/X)+((WINDOWS_HIGH-Axis_h)%WINDOWS_HIGH);
//			}
//			if(Axis_k<0){
//				memset(OLEDPicture_total.OLEDPicture_8[i],0xff,8);
//			}else if(Axis_k>WINDOWS_HIGH){
//				continue;
//			}else{
//				OLEDPicture_total.OLEDPicture_64[i]=Fx16>>(Axis_k);
//			}
//		}
//	}
//}

/**
  * @brief  画一个圆
  * @param  x_a 圆心x坐标，范围：0~127
  * @param  y_a 圆心y坐标，范围：0~63
  * @param  r 圆半径，范围：0~WINDOWS_HIGH
  * @param  r_a 内圆半径，范围：0~WINDOWS_HIGH
  * @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  * @retval 无
  */
void OLED_DrawARound(int x_a,int y_a,int r,int r_a,Color color){
	int x,y;
	int x_max,y_max,x_min,y_min;
	x_max=x_a+r;
	x_min=x_a-r;
	y_max=y_a+r;
	y_min=y_a-r;
	for(x=x_min;x<=x_max;x++){
		for(y=y_min;y<=y_max;y++){
			if(x < 0 || x >= WINDOWS_LENGTH || y <0 || y >=WINDOWS_HIGH) continue;
			if (( (x-x_a)*(x-x_a) + (y-y_a)*(y-y_a) >= r_a*r_a ) &&( (x-x_a)*(x-x_a) + (y-y_a)*(y-y_a) <= r*r ) &&r > 0){
				OLED_DrawAPoint(x, y, color);
			}
		}
	}
}

/**
  * @brief  显示字符
  * @param  x x坐标，范围：0~127
  * @param  y y坐标，范围：0~63
  * @param  CH 字符
  * @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  * @retval 无
  */
void OLED_DrawAWord(int x,int y,char CH,Color color){
	for(int i=0;i<8;i++){
		OLED_DrawAByte(x + i, y, OLED_F8x16[CH - ' '][i], color);
		OLED_DrawAByte(x + i, y+8, OLED_F8x16[CH - ' '][i+8], color);
	}
}

/**
  * @brief  显示字符串
  * @param  x x坐标，范围：0~127
  * @param  y y坐标，范围：0~63
  * @param  CH 字符串
  * @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  * @retval 无
  */
void OLED_DrawAString(int x,int y,char * CH,Color color){
	while(*CH){
		OLED_DrawAWord(x,y,*CH,color);
		CH++;
		x+=8;
	}
}

/**
  * @brief  显示数字
  * @param  x x坐标，范围：0~127
  * @param  y y坐标，范围：0~63
  * @param  num 要显示的数字，范围：0~4294967295
  * @param  length 要显示数字的长度，范围：1~10
  * @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  * @retval 无
  */
void OLED_DrawANum(int x,int y,int num,int length,Color color){
	char CH[15]={0};
	for(int i=length-1;i>=0;i--){
		CH[i]='0'+num%10;
		num/=10;
	}
	CH[length] = '\0';
	OLED_DrawAString(x,y,CH,color);
}

//原来的时间复杂度是O（8*8*num*num），空间复杂度是O（8*8*8）；现在时间复杂度为O（8*8*num），空间复杂度为O（0）
/**
	* @brief  显示放大字符(最大8倍)
	* @param  x x坐标，范围：0-127
	* @param  y y坐标，范围：0-63
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @param  num 放大倍数，范围：1~8
  	* @retval 无
  	*/
void OLED_AMPDrawAWord(int x,int y,char CH,Color color,uint8_t num){
	uint64_t block = (1ULL << num) - 1; // 生成 num 个连续 1
  	int idx = CH - ' ';
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			for(int k=0;k<num;k++){
				if(OLED_F8x16[idx][i]&(1ull<<j)){
					OLED_AMPDrawAPoint(num * i + k + x, num * j + y, num,color);
				}
				if(OLED_F8x16[idx][i+8]&(1ull<<j)){
					OLED_AMPDrawAPoint(num * i + k + x, num * j + y + 8 * num, num, color);
				}
			}
		}
	}
}

/**
	* @brief  显示放大mini字符(最大8倍)
	* @param  x x坐标，范围：0-127
	* @param  y y坐标，范围：0-63
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @param  num 放大倍数，范围：1~8
  	* @retval 无
  	*/
void OLEDMin_AMPDrawAWord(int x,int y,char CH,Color color,uint8_t num){
	uint64_t block = (1ULL << num) - 1; // 生成 num 个连续 1
  	int idx = CH - ' ';
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			for(int k=0;k<num;k++){
				if(OLEDMin_F8x8[idx][i]&(1ull<<j)){
					OLED_AMPDrawAPoint(num * i + k + x, num * j + y,num, color);
				}
			}
		}
	}
}

/**
  	* @brief  显示字符(mini)
	* @param  x x坐标，范围：0-127
	* @param  y y坐标，范围：0-63
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawAWord(int x,int y,char CH,Color color)
{
	int ch1= CH - ' ';
	for(int i=0;i<8;i++){
		OLED_DrawAByte(x + i, y, OLEDMin_F8x8[ch1][i],color);
	}
}

/**
	* @brief  显示字符串(mini)
  	* @param  x x坐标，范围：0-127
  	* @param  y y坐标，范围：0-63
  	* @param  CH 字符串
  	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawAString(int x,int y,char * CH,Color color){
	while(*CH){
		OLEDMin_DrawAWord(x,y,*CH,color);
		CH++;
		x+=8;
	}
}

/**
  	* @brief  求向量叉乘绝对值
	* @param  x1 y1 向量1坐标
	* @param  x2 y2 向量2坐标
	* @param  x3 y3 向量3坐标
  	* @retval 向量叉乘绝对值
  	*/

int area2(	int x1, int y1,
			int x2, int y2,
			int x3, int y3){
    int val = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
    return val < 0 ? -val : val;  // 绝对值
}

/**
 	* @brief  判断点(px,py) 是否在三角形内
	* @param  px py 点坐标
	* @param  x0 y0 三角形顶点坐标
	* @param  x1 y1 三角形顶点坐标
	* @param  x2 y2 三角形顶点坐标
	* @retval 布尔值
  	*/
int pointInTriangle(int px, int py,
                    int x0, int y0,
                    int x1, int y1,
                    int x2, int y2){
    int A  = area2(x0,y0, x1,y1, x2,y2);
    int A1 = area2(px,py, x1,y1, x2,y2);
    int A2 = area2(x0,y0, px,py, x2,y2);
    int A3 = area2(x0,y0, x1,y1, px,py);
    return (A == A1 + A2 + A3);
}

/**
  	* @brief  画一个三角形
	* @param  x1 y1 三角形顶点坐标
	* @param  x2 y2 三角形顶点坐标
	* @param  x3 y3 三角形顶点坐标
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawAThriangle(	int x1,int y1,
							int x2,int y2,
							int x3,int y3,Color color){
	int x,y;
	int x_max,y_max,x_min,y_min;
	x_min = (x1 < x2) ? ((x1 < x3) ? x1 : x3) : ((x2 < x3) ? x2 : x3);
	x_max = (x1 > x2) ? ((x1 > x3) ? x1 : x3) : ((x2 > x3) ? x2 : x3);
	y_min = (y1 < y2) ? ((y1 < y3) ? y1 : y3) : ((y2 < y3) ? y2 : y3);
	y_max = (y1 > y2) ? ((y1 > y3) ? y1 : y3) : ((y2 > y3) ? y2 : y3);
	for(x=x_min;x<=x_max;x++){
		for(y=y_min;y<=y_max;y++){
			if(x < 0 || x >= WINDOWS_LENGTH || y <0 || y >=WINDOWS_HIGH) continue;
			if(pointInTriangle(x,y,x3,y3,x1,y1,x2,y2)){
				OLED_DrawAPoint(x, y, color);
			}
		}
	}
}

/**
  	* @brief  画一个四边形
	* @param  x1 y1 四边形顶点坐标
	* @param  x2 y2 四边形顶点坐标
	* @param  x3 y3 四边形顶点坐标
	* @param  x4 y4 四边形顶点坐标
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawAquard(	int x1,int y1,
						int x2,int y2,
						int x3,int y3,
						int x4,int y4,Color color){
	OLED_DrawAThriangle(x1,y1,x2,y2,x3,y3,color);
	OLED_DrawAThriangle(x1,y1,x3,y3,x4,y4,color);
}

/**
  	* @brief  画一个五边形
	* @param  x1 y1 五边形顶点坐标
	* @param  x2 y2 五边形顶点坐标
	* @param  x3 y3 五边形顶点坐标
	* @param  x4 y4 五边形顶点坐标
	* @param  x5 y5 五边形顶点坐标
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawAPentagon(int x1,int y1,
						int x2,int x3,
						int y3,int y2,
						int x4,int y4,
						int x5,int y5,Color color){
	OLED_DrawAThriangle(x1,y1,x2,y2,x3,y3,color);
	OLED_DrawAThriangle(x1,y1,x3,y3,x4,y4,color);
	OLED_DrawAThriangle(x1,y1,x4,y4,x5,y5,color);
}

/**
  	* @brief  画一个正多边形
	* @param  x y 正多边形中心坐标
	* @param  side 边数，范围：3~8
	* @param  lenght 边长，范围：0~WINDOWS_HIGH
	* @param  angle 旋转角度，范围：0~360
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
#define pi 3.1415926
void OLED_DrawAPolygon(int x,int y,int side, int lenght , float angle,Color color){
	float angle1[100];
	float rad[100];
	for(int i=0;i<side;i++){
		angle1[i]=360*i/side+angle;
		rad[i]=angle1[i]*pi/180;
	}
	OLED_DrawAThriangle(x,y,x+sin(rad[0])*lenght,y+cos(rad[0])*lenght,x+sin(rad[side-1])*lenght,y+cos(rad[side-1])*lenght,OLED_White);
	for(int i=0;i<side-1;i++){
		OLED_DrawAThriangle(x,y,x+sin(rad[i])*lenght,y+cos(rad[i])*lenght,x+sin(rad[i+1])*lenght,y+cos(rad[i+1])*lenght,OLED_White);
	}
}

/**
  	* @brief  画一格调
  	* @retval 无
 	 */
void OLED_DrawAGeDiao(void){
	int a=32;
	int a_a=1;
  	while (1){
		//OLED_DrawClear();
		OLED_DrawARound(a,40,8,0,OLED_White);
		OLED_DrawARound(96,48,8,0,OLED_White);
		OLED_DrawARound(96,32,8,0,OLED_White);
		OLED_DrawAquard(a,32,96,32,a,48,96,48,OLED_White);
		OLED_ShowPicture();
		if(a>=WINDOWS_HIGH||a<=32){
			a=32;
			a_a=-a_a;
		}
		a+=a_a;
  	}
}

/**
  	* @brief  求旋转坐标
	* @param  cx cy 旋转中心坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  x y 旋转前坐标
	* @param  out_x out_y 旋转后坐标
  	* @retval 无
  	*/
void rotate_point(float 	cx, float   cy, float angle, 
                  float		 x, float    y, 
                  int *out_x, int *out_y){
	float rad =(pi/180)*angle;
    float s = sin(rad);
    float c = cos(rad);

    float dx = x - cx;
    float dy = y - cy;

    *out_x = (int)round(cx + dx * c - dy * s);
    *out_y = (int)round(cy + dx * s + dy * c);
}

/**
	* @brief  求旋转坐标
	* @param  cx cy 旋转中心坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  x y 旋转前坐标
	* @param  out_x out_y 旋转后坐标
	* @retval 无
	*/
void rotate_point_float(float 	cx, float   cy, float angle,
	float		 x, float    y,
	float* out_x, float* out_y) {
	float rad = (pi / 180) * angle;
	float s = sin(rad);
	float c = cos(rad);

	float dx = x - cx;
	float dy = y - cy;

	*out_x = (cx + dx * c - dy * s);
	*out_y = round(cy + dx * s + dy * c);
}

/**
  	* @brief  显示旋转字符
	* @param  x y 旋转中心坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawARatateWord(int x,int y,float angle,char CH,Color color){
	int x_r,y_r;
	int idx = CH - ' ';
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(OLED_F8x16[idx][i]&(1ull<<j)){
				rotate_point(x+4,y+8,angle,x+i,y+j,&x_r,&y_r);
				if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
				OLED_DrawAPoint(x_r, y_r, color);
			}
			if(OLED_F8x16[idx][i+8]&(1ull<<j)){
				rotate_point(x+4,y+8,angle,x+i,y+j+8,&x_r,&y_r);
				if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
				OLED_DrawAPoint(x_r, y_r, color);
			}
		}
	}
}

/**
  	* @brief  显示旋转字符(Mini)
	* @param  x y 旋转中心坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawARatateWord(int x,int y,float angle,char CH,Color color){
	int x_r,y_r;
	int idx = CH - ' ';
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(OLEDMin_F8x8[idx][i]&(1ull<<j)){
				rotate_point(x+4,y+4,angle,x+i,y+j,&x_r,&y_r);
				if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
				OLED_DrawAPoint(x_r, y_r, color);
			}
		}
	}
}

/**
  	* @brief  显示旋转字符串
	* @param  x y 旋转中心坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符串
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawARatateString(int x,int y,float angle,char* CH,Color color){
	int stringlen =x+4*strlen(CH);
	while(*CH){
		int x_r,y_r;
		int idx = *CH - ' ';
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(OLED_F8x16[idx][i]&(1ull<<j)){
					rotate_point(stringlen,y+8,angle,x+i,y+j,&x_r,&y_r);
					if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
					OLED_DrawAPoint(x_r, y_r, color);
				}
				if(OLED_F8x16[idx][i+8]&(1ull<<j)){
					rotate_point(stringlen,y+8,angle,x+i,y+j+8,&x_r,&y_r);
					if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
					OLED_DrawAPoint(x_r, y_r, color);
				}
			}
		}
		CH++;
		x+=8;
	}
}

/**
  	* @brief  显示旋转字符串(Mini)
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符串
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawARatateString(int x,int y,float angle,char * CH,Color color){
	int stringlen =x+4*strlen(CH);
	while(*CH){
		int x_r,y_r;
		int idx = *CH - ' ';
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(OLEDMin_F8x8[idx][i]&(1ull<<j)){
					rotate_point(stringlen,y+4,angle,x+i,y+j,&x_r,&y_r);
					if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
					OLED_DrawAPoint(x_r, y_r, color);
				}
			}
		}
		CH++;
		x+=10;
	}
}

/**
  	* @brief  显示字符串(Mini)
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符串
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/

/**
  	* @brief  显示旋转数字
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  num 要显示的数字，范围：0~4294967295
	* @param  length 要显示数字的长度，范围：1~10
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawARatateNum(int x,int y,float angle,int num,int length,Color color){
	char CH[15]={0};
	for(int i=length-1;i>=0;i--){
		CH[i]='0'+num%10;
		num/=10;
	}
	CH[length] = '\0';
	OLED_DrawARatateString(x,y,angle,CH,color);
}

/**
 	* @brief  显示旋转数字(min)
  	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  num 要显示的数字，范围：0~4294967295
	* @param  length 要显示数字的长度，范围：1~10
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawARatateNum(int x,int y,float angle,int num,int length,Color color){
	char CH[15]={0};
	for(int i=length-1;i>=0;i--){
		CH[i]='0'+num%10;
		num/=10;
	}
	CH[length] = '\0';
	OLEDMin_DrawARatateString(x,y,angle,CH,color);
}

/**
 	* @brief  显示旋转数字(min)
  	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  num 要显示的数字，范围：0~4294967295
	* @param  length 要显示数字的长度，范围：1~10
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLEDMin_DrawANum(int x,int y,int num,int length,Color color){
	char CH[15]={0};
	for(int i=length-1;i>=0;i--){
		CH[i]='0'+num%10;
		num/=10;
	}
	CH[length] = '\0';
	OLEDMin_DrawAString(x,y,CH,color);
}

/**
  	* @brief  显示旋转放大字符
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @param  num 放大倍数，范围：1~8
  	* @retval 无
  	*/
void OLED_AMPDrawARatateWord(int x,int y,float angle ,char CH,Color color,uint8_t num){
	uint64_t block = (1ULL << num) - 1; // 生成 num 个连续 1

	static uint64_t amp[2][64]={0};
	memset(amp,0,1028);
	if (num > 8)
		num = 8;
	int x_r,y_r;
  	int idx = CH - ' ';
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			for(int k=0;k<num;k++){
				if(OLED_F8x16[idx][i]&(1ull<<j))
					amp[0][num*i+k]|=(block<<(num*j));
				if(OLED_F8x16[idx][i+8]&(1ull<<j))
					amp[1][num*i+k]|=(block<<(num*j));
			}
		}
	}
	for(int i=0;i<(8*num);i++){
		for(int j=0;j<(8*num);j++){
			if(amp[0][i]&(1ull<<j)){
				rotate_point(x+4*num,y+8*num,angle,x+i,y+j,&x_r,&y_r);
				if(x_r < 0 || x_r >= WINDOWS_LENGTH || y_r <0 || y_r >=WINDOWS_HIGH) continue;
				OLED_DrawAPoint(x_r, y_r, color);
			}
			if (amp[1][i] & (1ull << j)) {
				rotate_point(x + 4 * num, y + 8 * num, angle, x + i, y + j+8*num, &x_r, &y_r);
				if (x_r < 0 || x_r >= WINDOWS_LENGTH || y_r < 0 || y_r >= WINDOWS_HIGH) continue;
				OLED_DrawAPoint(x_r, y_r, color);
			}
		}
	}
}

/**
  	* @brief  显示ADC波形
  	* @param  num 通道
  	* @param  t 时间
  	* @param  AMP 是否放大，范围：1、2
  	* @retval 无
  	*/
void OLED_DrawAAixs(int16_t num,uint16_t t,int AMP){
	OLED_DrawALine(0, WINDOWS_HIGH / 2, WINDOWS_LENGTH, WINDOWS_HIGH / 2, 0xffffff);
	OLED_DrawALine(WINDOWS_LENGTH / 2, 0, WINDOWS_LENGTH / 2, WINDOWS_HIGH, 0xffffff);
	if(AMP==1)
	{
		for(int i=0;i<WINDOWS_LENGTH;i++)
		{
			OLED_DrawAPoint(i, WINDOWS_HIGH /2- OLED_Sample_value[(t + i+ WINDOWS_LENGTH_MAX * 2 -WINDOWS_LENGTH) % (WINDOWS_LENGTH_MAX*2)][num] ,0xffffff);
			OLED_DrawALine_For_Axis(i-1,(WINDOWS_HIGH/2-OLED_Sample_value[(t+i-1 + WINDOWS_LENGTH_MAX * 2 - WINDOWS_LENGTH)% (WINDOWS_LENGTH_MAX*2)][num]),i,(WINDOWS_HIGH/2-OLED_Sample_value[(t+i + WINDOWS_LENGTH_MAX * 2 - WINDOWS_LENGTH)% (WINDOWS_LENGTH_MAX*2)][num]),OLED_White);
		}
	}
	if(AMP==2)
	{
		for(int i=0;i<WINDOWS_LENGTH;i++)
		{
			OLED_DrawAPoint(i, WINDOWS_HIGH /2- OLED_Sample_value[(t + i+ WINDOWS_LENGTH) % (WINDOWS_LENGTH*2)][num] ,0xffffff);
		}
	}
}

/**
  	* @brief  画一条线
  	* @param  x、y坐标
	* @param  x1、y1坐标
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawALine(int x,int y,int x1,int y1,Color color){
	int dx=x1-x;
	int dy=y1-y;
	int x_a=(x1>x)?1:-1;
	int y_a=(y1>y)?1:-1;
	int x_n=x,y_n=y;
	int length_x = (dx>0)?dx:-dx;
	int length_y = (dy>0)?dy:-dy;
	if(length_x>=length_y){
		for(int i=0;i<length_x;i++){
			y_n=((x_n-x)*dy/dx)+y;
			if(x_n > 0 && x_n <= WINDOWS_LENGTH && y_n >0 && y_n <=WINDOWS_HIGH){
				OLED_DrawAPoint(x_n, y_n, color);
			}
			x_n+=x_a;
		}
	}else{
		for(int i=0;i<length_y;i++){
			x_n=((y_n-y)*dx/dy)+x;
			if(x_n > 0 && x_n <= WINDOWS_LENGTH && y_n >0 && y_n <=WINDOWS_HIGH){
				OLED_DrawAPoint(x_n, y_n, color);
			}
			y_n+=y_a;
		}
	}
}

/**
	* @brief  画一条有宽度的线
	* @param  x、y坐标
	* @param  x1、y1坐标
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @retval 无
	*/
void OLED_DrawALine_Wide(int x, int y, int x1, int y1,int wide, Color color) {
	int dx = x1 - x;
	int dy = y1 - y;
	int x_a = (x1 > x) ? 1 : -1;
	int y_a = (y1 > y) ? 1 : -1;
	int x_n = x, y_n = y;
	int x_o, y_o;
	int length_x = (dx > 0) ? dx : -dx;
	int length_y = (dy > 0) ? dy : -dy;
	if (length_x >= length_y) {
		for (int i = 0;i < length_x;i++) {
			y_n = ((x_n - x) * dy / dx) + y;
			x_o = x_n - wide / 2;
			y_o = y_n - wide / 2;
			for (int k = 0;k < wide;k++)
			{
				for (int j = 0;j < wide;j++)
				{
					OLED_DrawAPoint(x_o + k, y_o + j, color);
				}
			}
			x_n += x_a;
		}
	}
	else {
		for (int i = 0;i < length_y;i++) {
			x_n = ((y_n - y) * dx / dy) + x;
			x_o = x_n - wide / 2;
			y_o = y_n - wide / 2;
			for (int k = 0;k < wide;k++)
			{
				for (int j = 0;j < wide;j++)
				{
					OLED_DrawAPoint(x_o + k, y_o + j, color);
				}
			}
			y_n += y_a;
		}
	}
}


/**
  	* @brief  Axis专用画线函数
  	* @param  x、y坐标
  	* @param  x1、y1坐标
  	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  	* @retval 无
  	*/
void OLED_DrawALine_For_Axis(int x,int y,int x1,int y1,Color color){
	int dx=x1-x;
	int dy=y1-y;
	int x_a=(x1>x)?1:-1;
	int y_a=(y1>y)?1:-1;
	int x_n=x,y_n=y;
	int length_x = (dx>0)?dx:-dx;
	int length_y = (dy>0)?dy:-dy;
	if(length_x<length_y){
		for(int i=0;i<length_y;i++){
			x_n=((y_n-y)*dx/dy)+x;
			if(x_n > 0 && x_n <= WINDOWS_LENGTH && y_n >0 && y_n <=WINDOWS_HIGH){
				OLED_DrawAPoint(x_n, y_n, color);
			}
			y_n+=y_a;
		}
	}
}

/**
  	* @brief  显示旋转字符
  	* @param  h 小时，范围：0~11
  	* @param  min 分钟，范围：0~59
  	* @param  second 秒钟，范围：0~59
  	* @retval 无
  	*/
void OLED_DrawAClock(int h,int min,int second){
	int h_x,h_y;
	int m_x,m_y;
	int s_x,s_y;
	h_x=sin((float)-(h*pi/6+pi+min*pi/720))*10+WINDOWS_HIGH;
	h_y=cos((float)-(h*pi/6+pi+min*pi/720))*10+32;
	m_x=sin((float)-(min*pi/30+pi+second*pi/3600))*20+WINDOWS_HIGH;
	m_y=cos((float)-(min*pi/30+pi+second*pi/3600))*20+32;
	s_x=sin((float)-second*pi/30+pi)*25+WINDOWS_HIGH;
	s_y=cos((float)-second*pi/30+pi)*25+32;
	OLED_DrawARound(WINDOWS_HIGH,32,30,29,OLED_White);
	OLED_DrawALine(WINDOWS_HIGH,32,h_x,h_y,OLED_White);
	OLED_DrawALine(WINDOWS_HIGH,32,m_x,m_y,OLED_White);
	OLED_DrawALine(WINDOWS_HIGH,32,s_x,s_y,OLED_White);
	OLEDMin_DrawAWord(60,4,'0',OLED_White);
	OLEDMin_DrawAWord(36,28,'9',OLED_White);
	OLEDMin_DrawAWord(84,28,'3',OLED_White);
	OLEDMin_DrawAWord(60,52,'6',OLED_White);
}

/**
	* @brief  显示一个三维面
	* @param  对应坐标
	* @retval 无
	*/
const int VanishingPointLength = 2000;//灭点距离
void OLED3D_DrawAFace(int x_c, int y_c, int z_c,
	int x_1, int y_1, int z_1,
	int x_2, int y_2, int z_2,
	int x_3, int y_3, int z_3,
	int x_4, int y_4, int z_4,Color color) {
	float k1 = (float)(VanishingPointLength - y_c - y_1) / VanishingPointLength;
	float k2 = (float)(VanishingPointLength - y_c - y_2) / VanishingPointLength;
	float k3 = (float)(VanishingPointLength - y_c - y_3) / VanishingPointLength;
	float k4 = (float)(VanishingPointLength - y_c - y_4) / VanishingPointLength;
	x_1 = (float)x_1 * k1 + x_c;
	z_1 = (float)z_1 * k1 + z_c;
	x_2 = (float)x_2 * k2 + x_c;
	z_2 = (float)z_2 * k2 + z_c;
	x_3 = (float)x_3 * k3 + x_c;
	z_3 = (float)z_3 * k3 + z_c;
	x_4 = (float)x_4 * k4 + x_c;
	z_4 = (float)z_4 * k4 + z_c;
	//printf("%d %d\n %d %d\n %d %d\n ",x_1,z_1,x_2,z_2,x_3,z_3);
	OLED_DrawAquard(x_1, z_1, x_2, z_2, x_3, z_3, x_4, z_4, color);
	//OLED_DrawALine(x_1, z_1, x_2, z_2, color);
	//OLED_DrawALine(x_2, z_2, x_3, z_3, color);
	//OLED_DrawALine(x_3, z_3, x_4, z_4, color);
	//OLED_DrawALine(x_4, z_4, x_1, z_1, color);
}

/**
	* @brief  显示一个旋转三维面
	* @param  对应坐标
	* @param  对应旋转角
	* @retval 无
	*/
void OLED3D_DrawARotateFace(int x_c, int y_c, int z_c,
	int x_1, int y_1, int z_1,
	int x_2, int y_2, int z_2,
	int x_3, int y_3, int z_3,
	int x_4, int y_4, int z_4,
	int angle_x, int angle_y, int angle_z,Color color)
{
	rotate_point(0, 0, angle_y, x_1, z_1, &x_1, &z_1);
	rotate_point(0, 0, angle_y, x_2, z_2, &x_2, &z_2);
	rotate_point(0, 0, angle_y, x_3, z_3, &x_3, &z_3);
	rotate_point(0, 0, angle_y, x_4, z_4, &x_4, &z_4);

	rotate_point(0, 0, angle_z, x_1, y_1, &x_1, &y_1);
	rotate_point(0, 0, angle_z, x_2, y_2, &x_2, &y_2);
	rotate_point(0, 0, angle_z, x_3, y_3, &x_3, &y_3);
	rotate_point(0, 0, angle_z, x_4, y_4, &x_4, &y_4);

	rotate_point(0, 0, angle_x, y_1, z_1, &y_1, &z_1);
	rotate_point(0, 0, angle_x, y_2, z_2, &y_2, &z_2);
	rotate_point(0, 0, angle_x, y_3, z_3, &y_3, &z_3);
	rotate_point(0, 0, angle_x, y_4, z_4, &y_4, &z_4);

	OLED3D_DrawAFace(x_c, y_c, z_c, x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3, x_4, y_4, z_4,color);
}

/**
	* @brief  显示旋转放大字符
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @param  num 放大倍数，范围：1~8
	* @retval 无
	*/
void OLED3D_AMPDrawARatateWord(int x, int y, int z, int x_c, int y_c, int z_c, int  anglex, int  angley, int  anglez, char CH, Color color, int num) {
	float k1 =  0 ;
	int x_r[4], y_r[4], z_r[4];
	int idx = CH - ' ';
	for (int i = 0;i < 8;i++) {//x
		for (int j = 0;j < 8;j++) {//y
			if (OLED_F8x16[idx][i] & (1ull << j))
			{
				x_r[0] = (i - 4) * num;
				z_r[0] = (j - 8) * num;

				x_r[1] = (i - 4 + 1) * num;
				z_r[1] = (j - 8) * num;

				x_r[2] = (i - 4 + 1) * num;
				z_r[2] = (j - 8 + 1) * num;

				x_r[3] = (i - 4) * num;
				z_r[3] = (j - 8 + 1) * num;
				for (int k = 0;k < 4;k++)
				{
					rotate_point(0, 0, angley, x_r[k] + x_c, z_r[k] + z_c, &x_r[k], &z_r[k]);
					rotate_point(0, 0, anglez, x_r[k], y_c, &x_r[k], &y_r[k]);
					rotate_point(0, 0, anglex, y_r[k], z_r[k], &y_r[k], &z_r[k]);//A面

					k1 = (float)(VanishingPointLength - y - y_r[k]) / VanishingPointLength;
					x_r[k] = (float)x_r[k] * k1 + x;
					z_r[k] = (float)z_r[k] * k1 + z;
				}
				OLED_DrawAquard(x_r[0], z_r[0], x_r[1], z_r[1], x_r[2], z_r[2], x_r[3], z_r[3], color);
			}
			if (OLED_F8x16[idx][i + 8] & (1ull << j))
			{
				x_r[0] = (i - 4) * num;
				z_r[0] = (j ) * num;

				x_r[1] = (i - 4 + 1) * num;
				z_r[1] = (j ) * num;

				x_r[2] = (i - 4 + 1) * num;
				z_r[2] = (j  + 1) * num;

				x_r[3] = (i - 4) * num;
				z_r[3] = (j  + 1) * num;
				for (int k = 0;k < 4;k++)
				{
					rotate_point(0, 0, angley, x_r[k] + x_c, z_r[k] + z_c, &x_r[k], &z_r[k]);
					rotate_point(0, 0, anglez, x_r[k], y_c, &x_r[k], &y_r[k]);
					rotate_point(0, 0, anglex, y_r[k], z_r[k], &y_r[k], &z_r[k]);//A面

					k1 = (float)(VanishingPointLength - y - y_r[k]) / VanishingPointLength;
					x_r[k] = (float)x_r[k] * k1 + x;
					z_r[k] = (float)z_r[k] * k1 + z;
				}
				OLED_DrawAquard(x_r[0], z_r[0], x_r[1], z_r[1], x_r[2], z_r[2], x_r[3], z_r[3], color);
			}
		}
	}
}

/**
	* @brief  显示旋转放大字符
	* @param  x、y坐标
	* @param  angle 旋转角度，范围：0~360
	* @param  CH 字符
	* @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
	* @param  num 放大倍数，范围：1~8
	* @retval 无
	*/
void OLED3DTKN_AMPDrawARatateWord(int x, int y, int z, int x_c, int y_c, int z_c, int TKN, int  anglex, int  angley, int  anglez, char CH, Color color, int num) {
	float k1[2][4] = { 0 };
	int x_r[2][4], y_r[2][4], z_r[2][4];
	float n_vector[3][3] = {
		{10,0,0},
		{0,10,0},
		{0,0,10}
	};
	for (int i = 0;i < 3;i++)
	{
		rotate_point_float(0, 0, angley, n_vector[i][0], n_vector[i][2], &n_vector[i][0], &n_vector[i][2]);
		rotate_point_float(0, 0, anglez, n_vector[i][0], n_vector[i][1], &n_vector[i][0], &n_vector[i][1]);
		rotate_point_float(0, 0, anglex, n_vector[i][1], n_vector[i][2], &n_vector[i][1], &n_vector[i][2]);
	}
	int idx = CH - ' ';
	for (int i = 0;i < 8;i++) {//x
		for (int j = 0;j < 8;j++) {//y
			if (OLED_F8x16[idx][i] & (1ull << j))
			{
				x_r[0][0] = (i - 4) * num;
				z_r[0][0] = (j - 8) * num;

				x_r[0][1] = (i - 4 + 1) * num;
				z_r[0][1] = (j - 8) * num;

				x_r[0][2] = (i - 4 + 1) * num;
				z_r[0][2] = (j - 8 + 1) * num;

				x_r[0][3] = (i - 4) * num;
				z_r[0][3] = (j - 8 + 1) * num;
				for (int k = 0;k < 4;k++)
				{
					rotate_point(0, 0, angley, x_r[0][k] + x_c, z_r[0][k] + z_c, &x_r[1][k], &z_r[1][k]);
					rotate_point(0, 0, anglez, x_r[1][k], y_c + TKN, &x_r[1][k], &y_r[1][k]);
					rotate_point(0, 0, anglex, y_r[1][k], z_r[1][k], &y_r[1][k], &z_r[1][k]);//A面
					k1[1][k] = (float)(VanishingPointLength - y - y_r[1][k]) / VanishingPointLength;
					x_r[1][k] = (float)x_r[1][k] * k1[1][k] + x;
					z_r[1][k] = (float)z_r[1][k] * k1[1][k] + z;

					rotate_point(0, 0, angley, x_r[0][k] + x_c, z_r[0][k] + z_c, &x_r[0][k], &z_r[0][k]);
					rotate_point(0, 0, anglez, x_r[0][k], y_c, &x_r[0][k], &y_r[0][k]);
					rotate_point(0, 0, anglex, y_r[0][k], z_r[0][k], &y_r[0][k], &z_r[0][k]);//A面
					k1[0][k] = (float)(VanishingPointLength - y - y_r[0][k]) / VanishingPointLength;
					x_r[0][k] = (float)x_r[0][k] * k1[0][k] + x;
					z_r[0][k] = (float)z_r[0][k] * k1[0][k] + z;
				}
				//y
				if (n_vector[1][1] < 0)
					OLED_DrawAquard(x_r[1][0], z_r[1][0], x_r[1][1], z_r[1][1], x_r[1][2], z_r[1][2], x_r[1][3], z_r[1][3], color);
				else
					OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[0][1], z_r[0][1], x_r[0][2], z_r[0][2], x_r[0][3], z_r[0][3], color);
				//x
				if (n_vector[0][1] < 0)
				{
					if (0 == (OLED_F8x16[idx][i + 1] & (1ull << j)))
						OLED_DrawAquard(x_r[0][1], z_r[0][1], x_r[1][1], z_r[1][1], x_r[1][2], z_r[1][2], x_r[0][2], z_r[0][2], ~color);
				}
				else
				{
					if (0 == (OLED_F8x16[idx][i -1] & (1ull << j)))
						OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[1][0], z_r[1][0], x_r[1][3], z_r[1][3], x_r[0][3], z_r[0][3], ~color);
				}
				//z
				if (n_vector[2][1] > 0)
				{
					if (0 == (OLED_F8x16[idx][i] & (1ull << j-1)) )
						OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[1][0], z_r[1][0], x_r[1][1], z_r[1][1], x_r[0][1], z_r[0][1], ~color);
				}
				else
				{
					if (0 == (OLED_F8x16[idx][i] & (1ull << j+1)) )
						OLED_DrawAquard(x_r[0][2], z_r[0][2], x_r[1][2], z_r[1][2], x_r[1][3], z_r[1][3], x_r[0][3], z_r[0][3], ~color);
				}
				
			}
			if (OLED_F8x16[idx][i + 8] & (1ull << j))
			{
				x_r[0][0] = (i - 4) * num;
				z_r[0][0] = (j ) * num;

				x_r[0][1] = (i - 4 + 1) * num;
				z_r[0][1] = (j ) * num;

				x_r[0][2] = (i - 4 + 1) * num;
				z_r[0][2] = (j  + 1) * num;

				x_r[0][3] = (i - 4) * num;
				z_r[0][3] = (j  + 1) * num;
				for (int k = 0;k < 4;k++)
				{
					rotate_point(0, 0, angley, x_r[0][k] + x_c, z_r[0][k] + z_c, &x_r[1][k], &z_r[1][k]);
					rotate_point(0, 0, anglez, x_r[1][k], y_c + TKN, &x_r[1][k], &y_r[1][k]);
					rotate_point(0, 0, anglex, y_r[1][k], z_r[1][k], &y_r[1][k], &z_r[1][k]);//A面
					k1[1][k] = (float)(VanishingPointLength - y - y_r[1][k]) / VanishingPointLength;
					x_r[1][k] = (float)x_r[1][k] * k1[1][k] + x;
					z_r[1][k] = (float)z_r[1][k] * k1[1][k] + z;

					rotate_point(0, 0, angley, x_r[0][k] + x_c, z_r[0][k] + z_c, &x_r[0][k], &z_r[0][k]);
					rotate_point(0, 0, anglez, x_r[0][k], y_c, &x_r[0][k], &y_r[0][k]);
					rotate_point(0, 0, anglex, y_r[0][k], z_r[0][k], &y_r[0][k], &z_r[0][k]);//A面
					k1[0][k] = (float)(VanishingPointLength - y - y_r[0][k]) / VanishingPointLength;
					x_r[0][k] = (float)x_r[0][k] * k1[0][k] + x;
					z_r[0][k] = (float)z_r[0][k] * k1[0][k] + z;
				}
				//y
				if (n_vector[1][1] < 0)
					OLED_DrawAquard(x_r[1][0], z_r[1][0], x_r[1][1], z_r[1][1], x_r[1][2], z_r[1][2], x_r[1][3], z_r[1][3], color);
				else
					OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[0][1], z_r[0][1], x_r[0][2], z_r[0][2], x_r[0][3], z_r[0][3], color);
				//x
				if (n_vector[0][1] < 0)
				{
					if(0==(OLED_F8x16[idx][i + 9] & (1ull << j)))
						OLED_DrawAquard(x_r[0][1], z_r[0][1], x_r[1][1], z_r[1][1], x_r[1][2], z_r[1][2], x_r[0][2], z_r[0][2], ~color);
				}
				else
				{
					if (0 == (OLED_F8x16[idx][i + 7] & (1ull << j)))
						OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[1][0], z_r[1][0], x_r[1][3], z_r[1][3], x_r[0][3], z_r[0][3], ~color);
				}
				//z
				if (n_vector[2][1] > 0)
				{
					if (0 == (OLED_F8x16[idx][i+8] & (1ull << j - 1)))
						OLED_DrawAquard(x_r[0][0], z_r[0][0], x_r[1][0], z_r[1][0], x_r[1][1], z_r[1][1], x_r[0][1], z_r[0][1], ~color);
				}
				else
				{
					if (0 == (OLED_F8x16[idx][i+8] & (1ull << j + 1)))
						OLED_DrawAquard(x_r[0][2], z_r[0][2], x_r[1][2], z_r[1][2], x_r[1][3], z_r[1][3], x_r[0][3], z_r[0][3], ~color);
				}
			}
		}
	}
}

void OLED3DTKN_AMPDrawARatateString(int x, int y, int z, int x_c, int y_c, int z_c, int TKN, int  anglex, int  angley, int  anglez, char * CH, Color color, int num) {
	int x_n, y_n, _z_n;
	int stringlength = strlen(CH);
	for (int i=0;i<stringlength;i++)
	{
		OLED3DTKN_AMPDrawARatateWord(x, y, z, x_c- num * 4 * (stringlength - 2*i), y_c, z_c, TKN, anglex, angley, anglez, *(CH+i), color, num);
	}
}

void OLED3D_AMPDrawARatateString(int x, int y, int z, int x_c, int y_c, int z_c, int  anglex, int  angley, int  anglez, char* CH, Color color, int num) {
	int x_n, y_n, _z_n;
	int stringlength = strlen(CH);
	for (int i = 0;i < stringlength;i++)
	{
		OLED3D_AMPDrawARatateWord(x , y, z, x_c - num * 4 * (stringlength - 2 * i), y_c, z_c,  anglex, angley, anglez, *(CH + i), color, num);
	}
}


/**
  * @brief  画一个3D圆
  * @param  x_a 圆心x坐标，范围：0~127
  * @param  y_a 圆心y坐标，范围：0~63
  * @param  r 圆半径，范围：0~WINDOWS_HIGH
  * @param  r_a 内圆半径，范围：0~WINDOWS_HIGH
  * @param  color 颜色，范围：OLED_Black、OLED_White、OLED_Contr
  * @retval 无
  */
void OLED3D_DrawARound(int x_a, int y_a,int z_a, int r,  int  anglex, int  angley, int  anglez, Color color) {
	int x, y,z;
	int x_r, y_r, z_r;
	int x_max, z_max, x_min, z_min;
	float k1;
	int r2 = r * r;
	x_max = x_a + r;
	x_min = x_a - r;
	z_max = z_a + r;
	z_min = z_a - r;

	for (int i = -r;i < r;i++)
	{
		for (int j = -r;j <r;j++)
		{
			if (i * i + j * j <= r2)
			{
				rotate_point(0, 0, angley, i, j, &x_r, &z_r);
				rotate_point(0, 0, anglez, x_r, 0, &x_r, &y_r);
				rotate_point(0, 0, anglex, y_r, z_r, &y_r, &z_r);//A面

				k1 = (float)(VanishingPointLength - y_a - y_r) / VanishingPointLength;
				x_r = (float)x_r * k1+x_a;
				z_r = (float)z_r * k1+z_a;
				OLED_DrawAPoint(x_r, z_r, color);
			}
		}
	}
}

void OLED_DrawAPen(int x, int y,int wide, Color color)
{
	x = x - wide / 2;
	y = y - wide / 2;
	for (int i = 0;i < wide;i++)
	{
		for (int j = 0;j < wide;j++)
		{
			OLED_DrawAPoint(x + i, y + j, color);
		}
	}
}