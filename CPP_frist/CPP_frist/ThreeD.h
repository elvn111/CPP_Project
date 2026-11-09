#pragma once
namespace ThreeD {

class cube {
    int m_objectId = -1;
    int m_x, m_y, m_z;
    int m_bianchang;
    float m_anglex, m_angley, m_anglez;

public:
    cube(int x = 0, int y = 0, int z = 0,
         float anglex = 0, float angley = 0, float anglez = 0,
         int bianchang = 0);
    ~cube();
    void display();
};

}

