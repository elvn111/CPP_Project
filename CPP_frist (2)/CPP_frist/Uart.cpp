#include "Uart.h"
using namespace std;


int  uart::Uart_Init(void)
{
    // 1. 打开串口 COM7
    hCom = CreateFileA(
        "\\\\.\\COM7",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hCom == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    // 2. 配置串口参数（必须和发送端一模一样）
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hCom, &dcb);

    dcb.BaudRate = CBR_9600;   // 波特率 115200
    dcb.ByteSize = 8;            // 数据位 8
    dcb.Parity = NOPARITY;       // 无校验
    dcb.StopBits = ONESTOPBIT;   // 停止位 1
    SetCommState(hCom, &dcb);

    char buf[1];
    DWORD len = 0;
    int i = 0;
    char ch[15] = { 0 };
    int num = 0;

    return 1;
}

void uart::Uart_Close(void)
{
    CloseHandle(hCom);
}

int uart::Uart_Recive(void)
{
    char buf;
    char_uint16 num[4] = { 0 };
    DWORD len = 0;
    int i = 0;
    char ch[30] = { 0 };
    while (1)
    {
        if (ReadFile(hCom, &buf, 1, &len, NULL))
        {
            if (len > 0)
            {
                if (buf == '\n')
                {
                    i = 0;
                    cout << num[0].c << ' ' << num[1].c << ' ' << num[2].c << ' ' << num[3].c << endl;
                }
                else
                {
                    if (i % 2)
                    {
                        num[i / 2].total.b = buf;
                    }
                    else
                    {
                        num[i / 2].total.a = buf;
                    }
                    i++;
                }
                buf = 0;
            }
        }
    }
}