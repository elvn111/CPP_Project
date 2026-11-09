#include "dot.h"

#define POINT_SIZE 8
#define SIDE_NUM 2

struct dot {
	int x;
	int y;
	dot* Dot[POINT_SIZE];
	//dot(int x = 0, int y = 0) :x(x), y(y) {};
};

const int k = 1;
int side = 100;
dot dot_face[SIDE_NUM][SIDE_NUM] = { 0 };

void Dot_Init(void) {
	for (int i = 0;i < SIDE_NUM;i++) {
		for (int j = 0;j < SIDE_NUM;j++) {
			dot_face[i][j].x = 500+j*side;
			dot_face[i][j].y = 500+i*side;
			if((i-1)>=0)
				dot_face[i][j].Dot[0] = &dot_face[i-1][j];
			if((i+1)<SIDE_NUM)
				dot_face[i][j].Dot[1] = &dot_face[i+1][j];
			if((j-1)>=0)
				dot_face[i][j].Dot[2] = &dot_face[i][j-1];
			if((j+1)<SIDE_NUM)
				dot_face[i][j].Dot[3] = &dot_face[i][j+1];

			if ((i - 1) >= 0&& (j - 1) >= 0)
				dot_face[i][j].Dot[4] = &dot_face[i - 1][j-1];
			if ((i + 1) < SIDE_NUM&& (j + 1) < SIDE_NUM)
				dot_face[i][j].Dot[5] = &dot_face[i + 1][j+1];
			if ((j - 1) >= 0&& (i + 1) < SIDE_NUM)
				dot_face[i][j].Dot[6] = &dot_face[i+1][j - 1];
			if ((j + 1) < SIDE_NUM&& (i - 1) >= 0)
				dot_face[i][j].Dot[7] = &dot_face[i-1][j + 1];

		}
	}
}

float modulo(int x, int y) {
	return sqrtf(x * x + y * y);
}

void dot_F(dot* Dot1, dot* Dot2, float* x1, float* y1) {
	float f;
	float x = Dot2->x - Dot1->x;
	float y = Dot2->y - Dot1->y;
	float mod = modulo(x, y);
	if (mod == 0)
	{
		*x1 = 0; *y1 = 0;
		return;
	}
	x /= mod;
	y /= mod;
	f = (mod - 50);
	x *= f;
	y *= f;
	*x1 = x;
	*y1 = y;
}

//void dot_F(dot* Dot1, dot* Dot2, int* x1, int* y1) {
//	int x = Dot2->x - Dot1->x;
//	int y = Dot2->y - Dot1->y;
//	int mod = (int)sqrtf((float)(x * x + y * y));
//	if (mod == 0) {          // 两点重合，防除零
//		*x1 = 0; *y1 = 0;
//		return;
//	}
//	int f = mod - 50;
//	*x1 = x * f / mod;       // 先乘 f 再除 mod，方向保住了
//	*y1 = y * f / mod;
//}

void dot_show(dot* Dot) {
	float x =  0 , y =  0 ;
	float f_x=0, f_y=0;
	OLED_DrawAquard(Dot->x-3, Dot->y-3, Dot->x+3, Dot->y-3, Dot->x+3, Dot->y+3, Dot->x-3, Dot->y+3, OLED_White);
	for (int i = 0;i < POINT_SIZE;i++) {
		if (Dot->Dot[i] != 0) {
			dot_F(Dot, Dot->Dot[i], &x, &y);
			OLED_DrawALine(Dot->x, Dot->y, Dot->Dot[i]->x, Dot->Dot[i]->y, OLED_White);
			f_x += x;
			f_y += y;
		}
	}
	OLED_DrawANum(Dot->x, Dot->y, f_x, 3, OLED_White);
	OLED_DrawANum(Dot->x+32, Dot->y, f_y, 3, OLED_White);
}


void dot_test(void) {
	Dot_Init();
	for (int i = 0;i < SIDE_NUM;i++) {
		for (int j = 0;j < SIDE_NUM;j++) {
			dot_show(&dot_face[i][j]);
		}
	}
}