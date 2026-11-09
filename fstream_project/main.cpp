#include <fstream>
#include <iostream>

using namespace std;

int main() {
    // 1. 创建文件输出流
    fstream file("C:\\Users\\admin\\Desktop\\test.txt");

    // 2. 判断是否打开成功
    if (!file) {
        return 1; // 打开失败
    }

    // 3. 像 cout 一样写入
    file <<"Hello"<< endl;
    file << 123 << endl;
    file << 3.14 << endl;

    
    //file.seekg(0);

    //string CH;
    // file>>CH;
    // cout << CH;

    // 4. 关闭文件
    file.close();

    return 0;
}