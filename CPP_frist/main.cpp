
#include <fstream>
#include "bmp.hpp"
#include "Uart.h"
#include <thread>
#include "Radar_Algorithm.h"
#include "ThreeD.h"
extern "C" {
    #include <Windows.h>
    
    #include <math.h>
    #include"OLED.h"
    extern int VanishingPointLength_y;
    extern int VanishingPointLength_Length;
    extern unsigned int  WINDOWS_LENGTH;
    extern unsigned int WINDOWS_HIGH;
    extern int OLED_Sample_value[WINDOWS_LENGTH_MAX];
    extern int OLED_Sample_value1[WINDOWS_LENGTH_MAX];
    extern int OLED_Sample_value2[WINDOWS_LENGTH_MAX];
     int OLED_Sample_value3[WINDOWS_LENGTH_MAX];
     int OLED_Sample_value4[WINDOWS_LENGTH_MAX];
     int OLED_Sample_value5[WINDOWS_LENGTH_MAX] = { 0 };
     int OLED_Sample_value6[WINDOWS_LENGTH_MAX];
    extern BYTE* g_frameBuf;
    HDC hdc;
}

#define KEY_DOWN(vk)  (GetAsyncKeyState(vk))

using namespace std;
using namespace Radar_Algorithm;
fstream file;

int wheel;

// 窗口过程回调
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // 窗口绘制时显示文字
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_CLOSE:
    {
        file.open("test.txt",ios::out);
        file.write((char*)g_frameBuf, WINDOWS_HIGH_MAX*WINDOWS_LENGTH_MAX * 4);
        //file << "smqk" << endl;
        file.close();
        DestroyWindow(hWnd);
        break;
    }
       

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_MOUSEWHEEL:
        wheel += GET_WHEEL_DELTA_WPARAM(wParam); // 保存到全局
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// 程序入口（Win32 标准入口）
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Sleep(1000);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    // 窗口类名（宽字符 L 前缀）
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    // 注册窗口类
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 加载光标

    if (!RegisterClass(&wc))
        return 1; // 注册失败返回

    // 创建窗口（使用 CreateWindowExW 宽字符版本）
    HWND hWnd = CreateWindowExW(
        0,                                  // 扩展风格
        CLASS_NAME,                         // 类名
        L"OLED_TEST",                    // 窗口标题（L 前缀）
        WS_OVERLAPPEDWINDOW,                // 窗口样式
        0, 0, 640, 480,                 // 位置 x,y 宽,高
        NULL,                               // 父窗口
        NULL,                               // 菜单
        hInstance,                          // 实例句柄
        NULL                                // 附加参数
    );

    if (hWnd == NULL)
        return 2; // 创建失败返回
    RECT rc;

    // 显示窗口
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    // 消息循环（标准 Win32 消息循环）
    MSG msg;

    int anglex = 0;
    int anglez = 0;
    OLED_DrawInit(hWnd);
    int t = 0;

    POINT pt;
    POINT pt1;

    GetCursorPos(&pt);
    GetCursorPos(&pt1);

    int wheel1 = 0;

    int windows_length=WINDOWS_LENGTH;
    int windows_high=WINDOWS_HIGH;
    Color color;
    int x_n, x_old, y_n, y_old;
    //ofstream file1("test.txt");
    //file1.close();
    //file.close();
    //file.open("test.txt",ios::out|ios::in);
    //file.read((char *)g_frameBuf, WINDOWS_HIGH_MAX * WINDOWS_LENGTH_MAX * 4);
    //file.close();

    //MessageBoxA(NULL, "111", "wc", MB_OK);

    float v_x=10, v_y=0;
    float a_x=1080, a_y=0;
    float radv=0;
    //v_y = 10;
    int bianchang = 200;
    if (BMP_Open()==0)
        OLED_DrawAString(50, 50, (char *)"ERROR", COLOR_RED);
    //for (int i = 0;i < WINDOWS_LENGTH_MAX;i++)
    //{
    //    OLED_Sample_value[i] =  cos(i * 0.05 + 1.8) * 10;
    //    //OLED_Sample_value3[i] = cos(i * 0.05) * 1000;
    //    //OLED_Sample_value[i]+= (cos(i * 0.05+1) * 100);
    //    //OLED_Sample_value1[i] = cos(i * 0.1) * 100;
    //    //OLED_Sample_value1[i] += cos(i * 0.1+2) * 100;
    //}
    uart uart1;
    uart1.Uart_Init();
    char buf;
    char_uint16 num[4] = { 0 };
    DWORD len = 0;
    int i = 0;
    char ch[30] = { 0 };

    ThreeD::cube a;
    int  vd_x=0, vd_y=0, vd_z=0;


    while (1)
    {

        radv = atan2((v_y > 0) ? v_y : -v_y,(v_x>0)?v_x:-v_x);
        v_y += 0.5;
        a_y += v_y;
        a_x += v_x;
        if (a_y > 1040 && v_y > 0)
        {
            //if(v_x>0)
            //    rotate_point_Rad(0, 0, -(2 * radv),v_x, v_y,  &v_x, &v_y);
            //else
            //    rotate_point_Rad(0, 0, (2 * radv),v_x, v_y, &v_x, &v_y);
            v_y = -v_y;
            a_y = 1040;
        }
        if (a_x > 1940)
        {
            v_x = -v_x;
            a_x = 1940;
        }
        if (a_x <40)
        {
            v_x = -v_x;
            a_x = 40;
        }
            
            //rotate_point_Rad(0, 0, v_x, v_y, 2 * radv, &v_x, &v_y);

        ////OLED_DrawARound(a_x, a_y, 10, 0, COLOR_RED);
        //file << "smqk" << endl;
        //窗口尺寸检测和画布重新生成
        GetCursorPos(&pt);
        GetWindowRect(hWnd, &rc);

        WINDOWS_LENGTH = rc.right - rc.left;
        WINDOWS_HIGH = rc.bottom - rc.top;

        if (windows_length != WINDOWS_LENGTH || windows_high != WINDOWS_HIGH)
        {
            //ReCreateCanvas(WINDOWS_LENGTH, WINDOWS_HIGH);
            //OLED_DrawClear(0, 0, WINDOWS_LENGTH, WINDOWS_HIGH, 0xffffff);
        }
        windows_length = WINDOWS_LENGTH;
        windows_high = WINDOWS_HIGH;



        //看这里
       // OLED_Sample_value[t] = -(pt.y-470)/2;





        //for (int b = 0;b < 45;b++)
        //{
        //    if (ReadFile(uart1.hCom, &buf, 1, &len, NULL))
        //    {
        //        if (len > 0)
        //        {
        //            if (buf == '\n')
        //            {
        //                i = 0;
        //                //cout << num[0].c << ' ' << num[1].c << ' ' << num[2].c << ' ' << num[3].c << endl;
        //                OLED_Sample_value[t] = num[0].c/32;
        //                OLED_Sample_value1[t] = num[1].c / 32;
        //                OLED_Sample_value2[t] = num[2].c / 32;
        //                OLED_Sample_value3[t] = num[3].c / 32;
        //                num[0].c = num[1].c = num[2].c = num[3].c = 0;
        //                t = (t + 1) % (1000);
        //            }
        //            else
        //            {
        //                if (i % 2)
        //                {
        //                    num[i / 2].total.b = buf;
        //                }
        //                else
        //                {
        //                    num[i / 2].total.a = buf;
        //                }
        //                i++;
        //            }
        //            buf = 0;
        //        }
        //    }
        //}
       
        UpdateWindow(hWnd);
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            // 如果收到退出消息，就退出循环
            if (msg.message == WM_QUIT)
                break;
        }
        VanishingPointLength_Length += wheel;
        if (VanishingPointLength_Length < 0)VanishingPointLength_Length = 0;
        //wheel1 = (wheel1 + ((wheel) ? 1 : ((wheel==0)?0:5))) % 6;
        wheel = 0;
        switch (wheel1)
        {
            case 0:color = COLOR_RED;break;
            case 1:color = COLOR_ORANGE;break;
            case 2:color = COLOR_YELLOW;break;
            case 3:color = COLOR_GREEN;break;
            case 4:color = COLOR_BLUE;break;
            case 5:color = COLOR_PURPLE;break;
        }
        //if(KEY_DOWN(VK_LBUTTON) & 0x8000) { 
        //    //OLED_DrawAPen(pt.x - rc.left, pt.y - rc.top,5, OLED_White);
        //    anglex += (pt.y - pt1.y) * 720 / 1080;
        //    anglez -= (pt.x - pt1.x) * 720 / 1980;
        //    pt1.x = pt.x;
        //    pt1.y = pt.y;
        //} else{
        //    pt1.x = pt.x;
        //    pt1.y = pt.y;
        //}
        anglex += (pt.y - pt1.y) * 720 / 1080;
        anglez -= (pt.x - pt1.x) * 720 / 1980;
        pt1.x = pt.x;
        pt1.y = pt.y;

        if (anglex > 90)
        {
            anglex = 90;
        }
        if (anglex < -90)
        {
            anglex = -90;
        }
        //OLED_DrawAPen(50,50, 50, color);
        
        x_n = pt.x - rc.left - 9;
        y_n = pt.y - rc.top - 40;

        if (KEY_DOWN(VK_LBUTTON))
        {
            a_x = 1080;
            a_y = 0;
            v_x = 10;
            v_y = 0;
        }

        if (KEY_DOWN('W') & 0x8000)
        {
            vd_y += 10 * cos((3.1415926 / 180) * anglez);
            vd_x -= 10 * sin((3.1415926 / 180) * anglez);
        }
        if (KEY_DOWN('A') & 0x8000)
        {
            vd_y -= 10 * sin((3.1415926 / 180) * anglez);
            vd_x-= 10 * cos((3.1415926 / 180) * anglez);
        }
        if (KEY_DOWN('S') & 0x8000)
        {
            vd_y -= 10 * cos((3.1415926 / 180) * anglez);
            vd_x += 10 * sin((3.1415926 / 180) * anglez);
        }
        if (KEY_DOWN('D') & 0x8000)
        {
            vd_y += 10 * sin((3.1415926 / 180) * anglez);
            vd_x += 10 * cos((3.1415926 / 180) * anglez);
        }
        if (KEY_DOWN(VK_SPACE) & 0x8000)
        {
            vd_z-= 20;
        }
        if (KEY_DOWN(VK_LSHIFT) & 0x8000)
        {
            vd_z += 20;
        }

        x_old = x_n;
        y_old = y_n;

        OLED_DrawClear(0,0, WINDOWS_LENGTH,WINDOWS_HIGH,0);
        Vedio_Updata(vd_x, vd_y,vd_z, anglex, 0, anglez);
        //OLED_DrawACube(WINDOWS_LENGTH / 2, 500, WINDOWS_HIGH / 2, anglex, 0, anglez, bianchang);
        a = ThreeD::cube(WINDOWS_LENGTH / 2, 500, WINDOWS_HIGH / 2,0, 0,0, bianchang);
        a.display();
        //OLED_DrawAAixs(0, 200, 1000, 300, 1000, OLED_Sample_value, 0, t, COLOR_RED, 1, 0);
        //OLED_DrawAAixs(0, 300, 1000, 300, 1000, OLED_Sample_value, 0, 0, COLOR_RED, 1, 0);
        //OLED_DrawAAixs(0, 500, 1024, 800, 1024, photo_reg, 0, 0,COLOR_BLUE, 1, 0);
        //t = (t + 1)%(1000);
        int chang = 500;
        int x_l = 50;
        int z_l = 50;
        //for (int i = -x_l;i < x_l;i++)
        //{
        //    for (int j = -z_l;j < z_l;j++)
        //    {
        //        if ((i + j) % 2 != 0)
        //        {
        //            OLED3D_DrawARotateSquareFace(1000, 2500, 500,
        //                 i * chang, j*chang, 0,
        //                 i * chang, (j+1)*chang, 0,
        //                ( i + 1) * chang, (j + 1) * chang, 0,
        //                ( i + 1) * chang, j*chang, 0,
        //                0, 0, 0,
        //                0xffffffff);
        //        }
        //        else
        //        {
        //            OLED3D_DrawARotateSquareFace(1000, 2500, 500,
        //                i * chang, j * chang, 0,
        //                i * chang, (j + 1) * chang, 0,
        //                (i + 1) * chang, (j + 1) * chang, 0,
        //                (i + 1) * chang, j * chang, 0,
        //                0, 0, 0,
        //                0xff000000);
        //        }
        //    }
        //}
        OLED_DrawANum(70, 70, VanishingPointLength_y, 20,OLED_White);
        OLED_DrawANum(70, 90, vd_x, 20, OLED_White);
        OLED_DrawANum(70, 110, vd_y, 20, OLED_White);
        OLED_DrawANum(70, 130, vd_z, 20, OLED_White);
        OLED_ShowPicture();
        OLED_Update(hWnd);
    }
    uart1.Uart_Close();
    return 0;
}
