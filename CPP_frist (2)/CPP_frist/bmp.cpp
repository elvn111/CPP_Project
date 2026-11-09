#include<Windows.h>
#include <iostream>
#include <fstream>
#include "elvn.hpp"
using namespace std;

#define WINDOWS_LENGTH_MAX 2700
#define WINDOWS_HIGH_MAX 1500

extern "C" {
    
    extern BYTE* g_frameBuf;

}

int BMP_Open(void)
{
    unsigned char fileHeader[14];
    unsigned char infoHeader[40];
    char* poto_reg=0;
    char* poto_reg2 = 0;
    char* poto_reg3 = 0;
    char* poto_reg1;
    int n=1;
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
    for (;n < height||n<width;n = (n << 1));
    poto_reg = (char *)malloc(width * height);
    poto_reg2 = (char*)malloc(width * height);
    poto_reg3 = (char*)malloc(width * height);
    memset(poto_reg,0, width * height);
    memset(poto_reg2, 0, width * height);
    memset(poto_reg3, 0, width * height);
    poto_reg1 = (char *)malloc(n*n );
    char feiwu[4];
    for (int i = 0;i < height;i++)
    {
        for (int j = 0;j < width;j++)
        {
            //file.read(feiwu, 2);
            file.read((char*)(poto_reg +i*width+j ), 1);
            file.read((char*)(poto_reg2 + i * width + j), 1);
            file.read((char*)(poto_reg3 + i * width + j), 1);
            //*(poto_reg + i * width + j) = (*(poto_reg + i * width + j) + feiwu[0] + feiwu[1]) / 3;
    
            //file.read((char*)(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4+j*4), 1);
            //file.read((char*)(g_frameBuf+width*4 + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1), 1);
            //file.read((char*)(g_frameBuf  +(height+ i) * WINDOWS_LENGTH_MAX * 4 + j * 4+2 ), 1);
        }
    }
    //FFT::FFT_2D(poto_reg, poto_reg1, width, height);
    //FFT::Gaussian_Filter(poto_reg1,n);
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::FFT_2D_CVL(poto_reg, poto_reg1, width, height);
    //FFT::FFT_2D_CVL(poto_reg, poto_reg1, width, height);
    //FFT::FFT_2D_CVL(poto_reg, poto_reg1, width, height);
    //FFT::FFT_2D_CVL(poto_reg, poto_reg1, width, height);
    //FFT::IFFT_2D(poto_reg, poto_reg1, width, height);
    
    //for (int i = 0;i < height;i++)
    //{
    //    for (int j = 0;j < width;j++)
    //    {
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg + i * width + j);
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg + i * width + j);
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg + i * width + j);
    //    }
    //}
    FFT::FFT_2D(poto_reg, poto_reg1, width, height);
    for (int i = 0;i < n/2;i++)
    {
        for (int j = 0;j < n/2;j++)
        {
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + n/2+(i+n/2) * n + j);
        }
    }
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2*n  + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + (i + n / 2) * n + j);
        }
    }
     for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + n / 2 + i * n + j);
        }
    }for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + i * n + j);
        }
    }
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::IFFT_2D(poto_reg, poto_reg1, width, height);
    //for (int i = 0;i < n;i++)
    //{
    //    for (int j = 0;j < n;j++)
    //    {
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + i * n + j);
    //    }
    //}
    FFT::FFT_2D(poto_reg2, poto_reg1, width, height);
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + n / 2 + (i + n / 2) * n + j);
        }
    }
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2 * n + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + (i + n / 2) * n + j);
        }
    }
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1+ n / 2 + i * n + j);
        }
    }for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + i * n + j);
        }
    }
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::IFFT_2D(poto_reg2, poto_reg1, width, height);
    //for (int i = 0;i < n;i++)
    //{
    //    for (int j = 0;j < n;j++)
    //    {
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1) = *(poto_reg1 + i * n + j);
    //    }
    //}
    FFT::FFT_2D(poto_reg2, poto_reg1, width, height);
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + n / 2 + (i + n / 2) * n + j);
        }
    }
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2 * n + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + (i + n / 2) * n + j);
        }
    }
    for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + n / 2 + i * n + j);
        }
    }for (int i = 0;i < n / 2;i++)
    {
        for (int j = 0;j < n / 2;j++)
        {
            *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + i * n + j);
        }
    }
    //FFT::Gaussian_Filter(poto_reg1, n);
    //FFT::IFFT_2D(poto_reg2, poto_reg1, width, height);
    //for (int i = 0;i < n;i++)
    //{
    //    for (int j = 0;j < n;j++)
    //    {
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+2) = *(poto_reg1 + i * n + j);
    //    }
    //}
    
    //for (int i = 0;i < n/2;i++)
    //{
    //    for (int j = 0;j < n/2;j++)
    //    {
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + n/2+(i+n/2) * n + j);
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+1) = *(poto_reg1 + n / 2 + (i + n / 2) * n + j);
    //        *(g_frameBuf + i * WINDOWS_LENGTH_MAX * 4 + j * 4+2) = *(poto_reg1 + n / 2 + (i + n / 2) * n + j);
    //    }
    //}
    //for (int i = 0;i < n / 2;i++)
    //{
    //    for (int j = 0;j < n / 2;j++)
    //    {
    //        *(g_frameBuf + 2*n  + i * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + (i + n / 2) * n + j);
    //        *(g_frameBuf +2* n  + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + (i + n / 2) * n + j);
    //        *(g_frameBuf + 2*n  + i * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + (i + n / 2) * n + j);
    //    }
    //}
    // for (int i = 0;i < n / 2;i++)
    //{
    //    for (int j = 0;j < n / 2;j++)
    //    {
    //        *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + n / 2 + i * n + j);
    //        *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + n / 2 + i * n + j);
    //        *(g_frameBuf + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + n / 2 + i * n + j);
    //    }
    //}for (int i = 0;i < n / 2;i++)
    //{
    //    for (int j = 0;j < n / 2;j++)
    //    {
    //        *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4) = *(poto_reg1 + i * n + j);
    //        *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 1) = *(poto_reg1 + i * n + j);
    //        *(g_frameBuf + 2 * n + (i + n / 2) * WINDOWS_LENGTH_MAX * 4 + j * 4 + 2) = *(poto_reg1 + i * n + j);
    //    }
    //}
    
    file.close();
   return 1;
}