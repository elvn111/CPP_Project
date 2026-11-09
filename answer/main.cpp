#include <iostream>
#include <thread>
#include <windows.h>
#include <vector>

using namespace std;

// 线程1任务
void task1()
{
    while (true)
    {
        std::cout << "【线程1】正在运行\n";
        // 不加sleep，抢占CPU，最大化体现时间片切换
    }
}

// 线程2任务
void task2()
{
    while (true)
    {
        std::cout << "【线程2】正在运行\n";
    }
}

int main()
{
    
    // 解决VS控制台中文乱码
    SetConsoleOutputCP(65001);
    // 开启两个并发线程
    std::thread t1(task1);
    std::thread t2(task2);

    // 等待线程（死循环不会结束，按控制台关闭键终止）
    t1.join();
    t2.join();

    return 0;
}