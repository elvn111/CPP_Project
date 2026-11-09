#include <Windows.h>
#include "pch.h"
// 关键：extern "C" 防止名字重整
extern "C"
{
    // 导出加法函数
    __declspec(dllexport) int CalcAdd(int a, int b)
    {
        return a + b;
    }

    // 弹窗提示
    __declspec(dllexport) void PopTip()
    {
        MessageBoxA(NULL, "来自 C++ DLL 的弹窗", "混合开发", MB_OK);
    }
}
