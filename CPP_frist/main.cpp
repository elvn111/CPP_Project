#include <Windows.h>
#include <math.h>

extern "C" {
    int wheel;
    extern unsigned int  WINDOWS_LENGTH;
    extern unsigned int WINDOWS_HIGH;
    #include"OLED.h"
    #define KEY_DOWN(vk)  (GetAsyncKeyState(vk))
    HDC hdc;
    extern int16_t OLED_Sample_value[WINDOWS_LENGTH_MAX][4];
}

// 窗口过程回调
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MSG msg1;
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
        DestroyWindow(hWnd);
        break;

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
    while (1)
    {
        GetCursorPos(&pt);
        GetWindowRect(hWnd, &rc);

        WINDOWS_LENGTH = rc.right - rc.left;
        WINDOWS_HIGH = rc.bottom - rc.top;

        if (windows_length != WINDOWS_LENGTH || windows_high != WINDOWS_HIGH)
        {
            ReCreateCanvas(WINDOWS_LENGTH, WINDOWS_HIGH);
        }
        
        windows_length = WINDOWS_LENGTH;
        windows_high = WINDOWS_HIGH;
        OLED_Sample_value[t][0] = -(pt.y-470)/2;
        UpdateWindow(hWnd);
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            // 如果收到退出消息，就退出循环
            if (msg.message == WM_QUIT)
                break;
        }
        wheel1 = (wheel1 + ((wheel) ? 1 : ((wheel==0)?0:5))) % 6;
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
        if(KEY_DOWN(VK_LBUTTON) & 0x8000) { 
            //OLED_DrawAPen(pt.x - rc.left, pt.y - rc.top,5, OLED_White);
            anglex += (pt.y - pt1.y) * 720 / 1080;
            anglez += (pt.x - pt1.x) * 720 / 1980;
            pt1.x = pt.x;
            pt1.y = pt.y;
        } else{
            pt1.x = pt.x;
            pt1.y = pt.y;
        }
        OLED_DrawAPen(50,50, 50, color);
        
        if (KEY_DOWN(VK_LBUTTON) ){
            OLED_DrawAPen(pt.x - rc.left-9, pt.y - rc.top-40, 5, color);
        }
        if (KEY_DOWN(VK_RBUTTON)) {
            OLED_DrawAPen(pt.x - rc.left - 9, pt.y - rc.top - 40, 50,0x00);
        }

        //OLED_DrawClear(0,0, WINDOWS_LENGTH,WINDOWS_HIGH);
        
        //OLED_DrawANum(200, 80, rc.right - rc.left, 4, OLED_White);
        //OLED_DrawANum(200, 100, rc.bottom - rc.top, 4, OLED_White);
        //OLED_DrawANum(200, 120, wheel, 4, OLED_White);
        //OLED_AMPDrawARatateWord(20, 20, anglez, 'A', COLOR_RED, 8);
        //float n_vector[3][3] = {
        //{10,0,0},
        //{0,10,0},
        //{0,0,10}
        //};
        //for (int i = 0;i < 3;i++)
        //{
        //    rotate_point_float(0, 0, 0, n_vector[i][0], n_vector[i][2], &n_vector[i][0], &n_vector[i][2]);
        //    rotate_point_float(0, 0, anglez, n_vector[i][0], n_vector[i][1], &n_vector[i][0], &n_vector[i][1]);
        //    rotate_point_float(0, 0, anglex, n_vector[i][1], n_vector[i][2], &n_vector[i][1], &n_vector[i][2]);
        //}

        //if (n_vector[2][1]>0)
        //{
        //    // 后面 红色
        //    OLED3D_DrawARotateFace(320, -200, 240, -50, -50, -50, 50, -50, -50, 50, 50, -50, -50, 50, -50, anglex, 0, anglez, COLOR_RED);
        //}
        //else
        //{
        //    // 前面 绿色
        //    OLED3D_DrawARotateFace(320, -200, 240, -50, -50, 50, 50, -50, 50, 50, 50, 50, -50, 50, 50, anglex, 0, anglez, COLOR_GREEN);
        //}
        //if (n_vector[0][1]>0)
        //{
        //    // 左面 黄色
        //    OLED3D_DrawARotateFace(320, -200, 240, -50, -50, -50, -50, -50, 50, -50, 50, 50, -50, 50, -50, anglex, 0, anglez, COLOR_YELLOW );
        //}
        //else
        //{
        //    // 右面 蓝色
        //    OLED3D_DrawARotateFace(320, -200, 240, 50, -50, -50, 50, -50, 50, 50, 50, 50, 50, 50, -50, anglex, 0, anglez, COLOR_BLUE);
        //}
        //if (n_vector[1][1]>0)
        //{
        //    // 下面 橙色
        //    OLED3D_DrawARotateFace(320, -200, 240, -50, -50, -50, 50, -50, -50, 50, -50, 50, -50, -50, 50, anglex, 0, anglez, COLOR_ORANGE);
        //    OLED3D_AMPDrawARatateWord(320, 0, 240, 0, 50, 0, anglex, -0, anglez + 180, 'B', COLOR_RED, 8);
        //}
        //else
        //{
        //    // 上面 紫色
        //    OLED3D_DrawARotateFace(320, -200, 240, -50, 50, -50, 50, 50, -50, 50, 50, 50, -50, 50, 50, anglex, 0, anglez, COLOR_PURPLE );
        //    OLED3D_AMPDrawARatateWord(320, 0, 240, 0, 50, 0, anglex, 0 + 90, anglez, 'A', COLOR_RED, 8);
        //}

        //OLED_DrawARatateString(0, 80,-anglez, (char *)"Author :ELVN", 0xFFFFFF);
        ////OLED3DTKN_AMPDrawARatateString(1080, 0, 540, 0, -5, 0,20, anglex, -0, anglez, (char *)"E L V N", COLOR_RED, wheel1);
        // OLED3D_DrawARound(1080, 0, 540, wheel1, anglex, 0, anglez, COLOR_RED);

       
        //OLED3D_AMPDrawARatateWord(160, 200, 120, 0, 50, 0, anglex, -0, anglez + 180, 'B', COLOR_RED, 8);
        //OLED_DrawAAixs(0,  t, 1);
        //t = (t + 1)%(WINDOWS_LENGTH_MAX*2);
        OLED_ShowPicture();
        OLED_Update(hWnd);
       // Sleep(1);
    }
    return 0;
}

