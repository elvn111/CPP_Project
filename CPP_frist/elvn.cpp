#include "elvn.hpp"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.141592

namespace FFT {
	complex FFT_Value[3000] = { 0 };
	complex FFT_Value_2D[3000][3000] = { 0 };   // 72MB unused
	complex FFT_Value1_2D[3000][3000] = { 0 };  // 72MB unused
	complex FFT_CVL_2D[3000][3000] = { 0 };     // 72MB unused
	complex Gaussian_Tier[3000][3000] = { 0 };  // 72MB unused

	complex Euler_Formula(float angle){
		complex c;
		c.real = cos(angle);
		c.image =  (angle>0)? sin(angle) :-sin(angle);
		return c;
	}

	float ABV(complex a){
		return sqrtf(a.image * a.image + a.real * a.real);
	}

	void DFT(int * data, int * data1, int length){
		complex complex1;
		complex complex2 = {0,0};
		memset(data1, 0, sizeof(int) * length);
		for (int i = 0;i < length;i++){
			complex2.image = 0;
			complex2.real = 0;
			for (int j = 0;j < length;j++){
				complex1 = Euler_Formula(-j * i * 2 * PI / length);
				complex2= complex1* *(data + j)+complex2;
			}
			*(data1 + i)= ABV(complex2);
		}
	}

	void IDFT(int* data, int* data1, int length){
		complex complex1;
		complex complex2 = { 0,0 };
		memset(data1, 0, sizeof(int) * length);
		for (int i = 0;i < length;i++){
			complex2.image = 0;
			complex2.real = 0;
			for (int j = 0;j < length;j++){
				complex1 = Euler_Formula(j * i * 2 * PI / length);
				complex2 = complex1 * *(data + j) + complex2;
			}
			*(data1 + i) = ABV(complex2)/length;
		}
	}

	void bit_reverse(complex* x, int length){
		int n = length;
		int i, j, k;
		for (i = 1, j = 0; i < n; i++){
			for (k = n >> 1; j >= k; k >>= 1) j -= k;
			j += k;
			if (i < j){
				complex t = x[i];
				x[i] = x[j];
				x[j] = t;
			}
		}
	}

	void FFT(int* data, int* data1, int length1){
		int n=1,m=0;
		for (;n < length1;n = (n<<1))m++;
		complex* reg = (complex*)malloc(sizeof(complex) * n);
		memset(reg, 0, sizeof(complex) * n);
		memset(data1, 0, sizeof(int) * length1);
		for (int i = 0; i < length1; i++) {
			reg[i].real = data[i];
		}
		bit_reverse(reg, n);
		for (int i = 1, length = 1, x = 0;i <=m;i++){
			for (int j = 0;j < n/2;j++){
				complex complex1 = reg[ x];
				complex complex2 = reg[x + length]* Euler_Formula(-2 * PI * (x% (2 * length)) / (2*length));
				FFT_Value[ x] = reg[x] = complex1 + complex2 ;
				FFT_Value[ x+ length] = reg[x + length] = complex1 - complex2;
				x +=( ((j+1) % length) ? 1 : (length+1));
			}
			x = 0;
			length *= 2;
		}
		for (int i = 0;i < n;i++){
			data1 [ i] = ABV(reg[i]);
		}
		free(reg);
	}

	void IFFT(int* data, int* data1, int length1)
	{
		int n = 1, m = 0;
		for (;n < length1;n = (n << 1))m++;
		complex* reg = (complex*)malloc(sizeof(complex) * n);
		memset(reg, 0, sizeof(complex) * n);
		memset(data1, 0, sizeof(int) * length1);
		for (int i = 0; i < n; i++) {
			reg[i] = FFT_Value[i];
		}
		bit_reverse(reg, n);
		for (int i = 1, length = 1, x = 0;i <= m;i++) {
			for (int j = 0;j < n / 2;j++) {
				complex complex1 = reg[x];
				complex complex2 = reg[x + length] * Euler_Formula(2 * PI * (x % (2 * length)) / (2 * length));
				reg[x] = complex1 + complex2;
				reg[x + length] = complex1 - complex2;
				x += (((j + 1) % length) ? 1 : (length + 1));
			}
			x = 0;
			length *= 2;
		}
		for (int i = 0;i < length1;i++) {
			data1[i] = reg[n-i-1].real /n;
		}
		free(reg);
	}

	void FFT_2D(char* data, char* data1, int length1, int high){
		int n = 1, m = 0;
		memset(FFT_Value_2D, 0, 3000 * 3000 * 4);
		for (;n < length1 || n < high;n = (n << 1))m++;
		for (int i = 0; i < high; i++){
			for (int j = 0; j < length1; j++){
				FFT_Value_2D[i  ][ j].real = data[i * length1 + j];
				FFT_Value_2D[i ][ j].image = 0;
			}
		}
		for (int k = 0;k < n;k++){
			int i1, j1, k1;
			for (i1 = 1, j1 = 0; i1 < n; i1++){
				for (k1 = n >> 1; j1 >= k1; k1 >>= 1) j1 -= k1;
				j1 += k1;
				if (i1 < j1){
					complex t = FFT_Value_2D[i1][k];
					FFT_Value_2D[i1][k] = FFT_Value_2D[j1][k];
					FFT_Value_2D[j1][k] = t;
				}
			}
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_Value_2D[x][k];
					complex complex2 = FFT_Value_2D[x + length][k];
					FFT_Value_2D[x][k] = complex1 + complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					FFT_Value_2D[x + length][k] = complex1 - complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int k = 0;k < n;k++){
			bit_reverse(FFT_Value_2D[k], n);
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_Value_2D[k][x];
					complex complex2 = FFT_Value_2D[k][x + length];
					FFT_Value_2D[k][x] = complex1 + complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					FFT_Value_2D[k][x + length] = complex1 - complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				int z = ABV(FFT_Value_2D[i][j])*4/n;
					data1[i * n + j] = (z > 255) ? 255 : z;
			}
		}
	}

	void IFFT_2D(char* data, char* data1, int length1, int high){
		int n = 1, m = 0;
		for (;n < length1 || n < high;n = (n << 1))m++;
		for (int k = 0;k < n;k++){
			bit_reverse(FFT_Value_2D[k], n);
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_Value_2D[k][x];
					complex complex2 = FFT_Value_2D[k][(x + length)];
					FFT_Value_2D[k][x] = complex1 + complex2 * Euler_Formula(2 * PI * (x % (2 * length)) / (2 * length));
					FFT_Value_2D[k][(x + length)] = complex1 - complex2 * Euler_Formula(2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				FFT_Value1_2D[i][j] = FFT_Value_2D[i][ j+1];
			}
		}
		for (int k = 0;k < n;k++){
			int i1, j1, k1;
			for (i1 = 1, j1 = 0; i1 < n; i1++){
				for (k1 = n >> 1; j1 >= k1; k1 >>= 1) j1 -= k1;
				j1 += k1;
				if (i1 < j1){
					complex t = FFT_Value1_2D[i1][k];
					FFT_Value1_2D[i1][k] = FFT_Value1_2D[j1][k];
					FFT_Value1_2D[j1][k] = t;
				}
			}
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_Value1_2D[x][k];
					complex complex2 = FFT_Value1_2D[x + length][k];
					FFT_Value1_2D[x][k] = complex1 + complex2 * Euler_Formula(2 * PI * (x % (2 * length)) / (2 * length));
					FFT_Value1_2D[x + length][k] = complex1 - complex2 * Euler_Formula(2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				FFT_Value_2D[i][j] = FFT_Value1_2D[i][j];
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				int x = FFT_Value_2D[i][j].real/(n*n) ;
				data1[i * n + j] = (x > 255) ? 255 : x;
			}
		}
	}

	void FFT_2D_CVL(char* data, char* data1, int length1, int high){
		int n = 1, m = 0;
		memset(FFT_CVL_2D, 0, 3000 * 3000 * 4);
		for (;n < length1 || n < high;n = (n << 1))m++;
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				FFT_CVL_2D[i][j].image = 0;
			}
		}
		FFT_CVL_2D[0][0].real = 1;FFT_CVL_2D[1][0].real = 2;FFT_CVL_2D[2][2].real = 1;
		FFT_CVL_2D[0][1].real = 2;FFT_CVL_2D[1][1].real = 4;FFT_CVL_2D[2][2].real = 2;
		FFT_CVL_2D[0][2].real = 1;FFT_CVL_2D[1][2].real = 2;FFT_CVL_2D[2][2].real = 1;
		for (int k = 0;k < 3;k++){
			int i1, j1, k1;
			for (i1 = 1, j1 = 0; i1 < n; i1++){
				for (k1 = n >> 1; j1 >= k1; k1 >>= 1) j1 -= k1;
				j1 += k1;
				if (i1 < j1){
					complex t = FFT_CVL_2D[i1][k];
					FFT_CVL_2D[i1][k] = FFT_CVL_2D[j1][k];
					FFT_CVL_2D[j1][k] = t;
				}
			}
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_CVL_2D[x][k];
					complex complex2 = FFT_CVL_2D[x + length][k];
					FFT_CVL_2D[x][k] = complex1 + complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					FFT_CVL_2D[x + length][k] = complex1 - complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int k = 0;k < n;k++){
			bit_reverse(FFT_CVL_2D[k], n);
			for (int i = 1, length = 1, x = 0;i <= m;i++){
				for (int j = 0;j < n / 2;j++){
					complex complex1 = FFT_CVL_2D[k][x];
					complex complex2 = FFT_CVL_2D[k][x + length];
					FFT_CVL_2D[k][x] = complex1 + complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					FFT_CVL_2D[k][x + length] = complex1 - complex2 * Euler_Formula(-2 * PI * (x % (2 * length)) / (2 * length));
					x += (((j + 1) % length) ? 1 : (length + 1));
				}
				x = 0;
				length *= 2;
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				FFT_CVL_2D[i][j].image /= 16;
				FFT_CVL_2D[i][j].real /= 16;
				FFT_Value_2D[i][j] = FFT_Value_2D[i][j]*FFT_CVL_2D[i][j];
			}
		}
	}

	void Gaussian_Filter(char* data1,int n){
		const int sigma = 15;
		int r = n / 2;
		for (int i = 0;i < r-1;i++){
			for (int j = 0;j < r-1;j++){
				Gaussian_Tier[i][j].real=expf(-(float)((i +1)* (i + 1) + (j + 1) * (j + 1)) / (2 * sigma * sigma));
				Gaussian_Tier[(n-1) - i][ j].real = Gaussian_Tier[ i][ j].real;
				Gaussian_Tier[ i][(n-1)- j].real = Gaussian_Tier[i][j].real;
				Gaussian_Tier[(n-1) - i][(n-1) - j].real = Gaussian_Tier[i][j].real;
			}
		}
		for (int i = 0;i < n;i++){
			for (int j = 0;j < n;j++){
				FFT_Value_2D[i][j] = FFT_Value_2D[i][j] * Gaussian_Tier[i][j];
			}
		}
	}
};