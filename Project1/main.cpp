#include <windows.h>
#include <iostream>

#define COMX "COM12"

extern "C" {
#include "bsp.h"
}




using namespace std;

char info[1024] = { 0 };

union char_uint16 {
    struct {
        char a;
        char b;
    }total;
    uint16_t c;
    char_uint16() {
        c = 0;
    }
    char_uint16(uint16_t v) : c(v) {}
};

int main()
{
    // 1. 打开串口 COM12

    GPS_Module_Info GPS_Info;

    HANDLE hCom = CreateFileA(
        "\\\\.\\COMX",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hCom == INVALID_HANDLE_VALUE)
    {
        cout << "Open "<< COMX <<" failed!" << endl;
        system("pause");
        return 1;
    }
    cout << "Open " << COMX << " success!" << endl;

    // 2. 配置串口参数（必须和发送端一模一样）
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hCom, &dcb);

    dcb.BaudRate = CBR_115200;   // 波特率 115200
    dcb.ByteSize = 8;            // 数据位 8
    dcb.Parity = NOPARITY;       // 无校验
    dcb.StopBits = ONESTOPBIT;   // 停止位 1
    SetCommState(hCom, &dcb);

    // 3. 循环读取串口数据
    char *buf=info;
    char_uint16 num[4] = { 0 };
    DWORD len=0;
    int i = 0;
    char ch[30] = { 0 };
    while (1)
        if (ReadFile(hCom, buf, 1, &len, NULL))
            if (len > 0)
                if (*(buf - 2) == '*'){
                    buf = info;
                    gps_info_rsl(info, &GPS_Info);
                    cout << GPS_Info.Baseline;
                }
                else buf++;
    CloseHandle(hCom);
    return 0;
}