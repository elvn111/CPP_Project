#ifndef __ELVN_HPP__
#define __ELVN_HPP__

namespace FFT {

	struct complex{
		float real, image;
		complex (float real=0,float image=0):real(real),image(image){}
		complex operator+(const complex& b) {
			return complex(real + b.real, image + b.image);
		}
		complex operator-(const complex& b) {
			return complex(real - b.real,image - b.image);
		}
		complex operator*(const complex& b) {
			return complex(real*b.real-image*b.image, image * b.real + real * b.image);
		}
	};

	complex Euler_Formula(float angle);
	float ABV(complex a);
	void DFT(int * data, int* data1, int length);
	void FFT(int* data, int* data1, int length1);
	void IFFT(int* data, int* data1, int length1);
	void IDFT(int* data, int* data1, int length);
	void FFT_2D(char* data, char* data1, int length, int high);
	void IFFT_2D(char* data, char* data1, int length1, int high);
	void FFT_2D_CVL(char* data, char* data1, int length1, int high);
	void Gaussian_Filter(char* data1,int n);
};

#endif
