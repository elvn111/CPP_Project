#include<Windows.h>
#include <iostream>
#include <fstream>
#include "Radar_Algorithm.h"
using namespace std;
using namespace Radar_Algorithm;
#define WINDOWS_LENGTH_MAX 2700
#define WINDOWS_HIGH_MAX 1500

extern "C" {
    
    extern BYTE* g_frameBuf;

}

int BMP_Open(void)
{
    unsigned char fileHeader[14];
    unsigned char infoHeader[40];
    int n=1;
    int m = 0;
    ifstream file("C:\\Users\\admin\\Desktop\\111(1).bmp", ios::binary);
    if (!file)
    {
        return 0;
    }

    file.read((char*)fileHeader, 14);
    file.read((char*)infoHeader, 40);
    
    int fileSize = fileHeader[2] | (fileHeader[3] << 8) | (fileHeader[4] << 16) | (fileHeader[5] << 24);
    int pixelOffset = fileHeader[10] | (fileHeader[11] << 8) | (fileHeader[12] << 16) | (fileHeader[13] << 24);
    int width = infoHeader[4] | (infoHeader[5] << 8) | (infoHeader[6] << 16) | (infoHeader[7] << 24);
    int height = infoHeader[8] | (infoHeader[9] << 8) | (infoHeader[10] << 16) | (infoHeader[11] << 24);
    int bitCount = infoHeader[14] | (infoHeader[15] << 8);
    for (;n < height||n<width;n = (n << 1))m++;
    //n = 1024;
    //m = 10;
    Complex* photo_matrix_in = (Complex*)malloc(sizeof(Complex) * n * n);
    Complex* photo_matrix_out = (Complex*)malloc(sizeof(Complex) * n * n);

    char* photo_reg = (char*)malloc(sizeof(char) * n * n);
    memset(photo_reg, 0, sizeof(char) * n * n);

    char* photo_reg1 = (char*)malloc(sizeof(char) * n * n);
    memset(photo_reg1, 0, sizeof(char) * n * n);

    char* photo_reg2 = (char*)malloc(sizeof(char) * n * n);
    memset(photo_reg2, 0, sizeof(char) * n * n);

    memset(photo_matrix_in, 0, sizeof(Complex) * n * n);
    memset(photo_matrix_out, 0, sizeof(Complex) * n * n);
    char feiwu[4];
    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            //file.read(feiwu, 2);
            
            file.read((char*)(photo_reg + i * n + j), 1);
            file.read((char*)(photo_reg1 + i * n + j), 1);
            file.read((char*)(photo_reg2 + i * n + j), 1);
            
            //*(poto_reg + i * width + j) = (*(poto_reg + i * width + j) + feiwu[0] + feiwu[1]) / 3;
    
            //file.read((char*)(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4), 1);
            //file.read((char*)(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1), 1);
            //file.read((char*)(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+2), 1);
            //file.read((char*)(g_frameBuf+width*4 + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1), 1);
            //file.read((char*)(g_frameBuf  +(height+ i) * WINDOWS_LENGTH_MAX * 4 + j * 4+2 ), 1);
        }
        if ((width * 3 % 4) != 0) {
            file.read((char*)(feiwu),( width * 3 % 4));
        }
    }

    for (int i = 0;i < n * n;i++){
        photo_matrix_in[i].real = photo_reg[i];
    }
    //FFT_2D(photo_matrix_in, m, photo_matrix_out, FFT_Prograde);
    //FFT_2D(photo_matrix_out, m, photo_matrix_out, FFT_Retrograde);
    Gaussian_Filter(photo_matrix_in, m, photo_matrix_out);
    for (int i = 0;i < n * n;i++){
        photo_reg[i] = photo_matrix_out[i].real;
    }

    for (int i = 0;i < n * n;i++){
        photo_matrix_in[i].real = photo_reg1[i];
    }
    //FFT_2D(photo_matrix_in, m, photo_matrix_out, FFT_Prograde);
    //FFT_2D(photo_matrix_out, m, photo_matrix_out, FFT_Retrograde);
    Gaussian_Filter(photo_matrix_in, m, photo_matrix_out);
    for (int i = 0;i < n * n;i++){
        photo_reg1[i] = photo_matrix_out[i].real;
        //ABV(photo_matrix_out[i]);
        //photo_matrix_out[i].real;
    }

    for (int i = 0;i < n * n;i++){
        photo_matrix_in[i].real = photo_reg2[i];
    }
    //FFT_2D(photo_matrix_in, m, photo_matrix_out, FFT_Prograde);
    //FFT_2D(photo_matrix_out, m, photo_matrix_out, FFT_Retrograde);
    Gaussian_Filter(photo_matrix_in, m, photo_matrix_out);
    for (int i = 0;i < n * n;i++){
        photo_reg2[i] = photo_matrix_out[i].real;
    }

    for (int i = 0;i < n;i++)
    {
        for (int j = 0;j < n;j++)
        {
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(photo_reg + i * n + j);
            //*(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1) = *(photo_reg + i * n + j);
            //*(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+2) = *(photo_reg + i * n + j);
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(photo_reg1 + i * n + j);
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(photo_reg2 + i * n + j);
        }
    }

    free(photo_matrix_in);
    free(photo_matrix_out);
    free(photo_reg);
    free(photo_reg1);
    free(photo_reg2);
    file.close();
   return 1;
}