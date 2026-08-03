#ifndef __UART_H__
#define __UART_H__
#include <windows.h>
#include <iostream>

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

class uart
{
public:
	HANDLE hCom;
	int Uart_Init(void);
	void Uart_Close(void);

	int Uart_Recive(void);
};

#endif //__UART_H__

