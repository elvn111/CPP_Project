#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int my_print(const char* fmt, ...) {
    int stringlen = strlen(fmt);
    int sign = 0;
    for (int i = 0;i < stringlen;i++)
    {

        if (fmt[i] == '%')
        {
            switch(fmt[i+1])
                case'd': {
                ..........;
            }
        }
        else {
            putchar(fmt[i]);
        }
    }
    va_list ap;
    va_start(ap, fmt);   // ① 让 ap 指向第一个可变参数(固定参数 fmt 之后)
    int   a = va_arg(ap, int);    // ② 取一个 int
    char* b = va_arg(ap, char*);  // ② 再取一个 char*
    va_end(ap);          // ③ 结束
    return 0;
}