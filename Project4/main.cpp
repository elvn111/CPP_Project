#include <iostream>

using namespace std;


void Normal_Vector(int x_1, int y_1, int z_1,
    int x_2, int y_2, int z_2,
    int x_3, int y_3, int z_3,
    int *x,int*y ,int*z) {
    float ax = x_2 - x_1, ay = y_2 - y_1, az = z_2 - z_1;
    float bx = x_3 - x_1, by = y_3 - y_1, bz = z_3 - z_1;
    *x = (int)ay * bz - az * by;
    *y = (int)az * bx - ax * bz;
    *z = (int)ax * by - ay * bx;
}

int main(void){
    int x, y, z;
	Normal_Vector(3,0,0,
		0,3,0,
		0,0,3,
        &x,&y,&z);
    cout << x << y << z;
}