#include "ThreeD.h"
extern "C" {
#include "OLED.h"
}

#define MAX_ID 1024

namespace ThreeD {

static unsigned char s_idPool[MAX_ID] = {0};

cube::cube(int x, int y, int z, float anglex, float angley, float anglez, int bianchang)
    : m_x(x), m_y(y), m_z(z),
      m_bianchang(bianchang),
      m_anglex(anglex), m_angley(angley), m_anglez(anglez)
{
    for (int i = 0; i < MAX_ID; i++) {
        if (s_idPool[i] == 0) {
            s_idPool[i] = 1;
            m_objectId = i;
            return;
        }
    }
    m_objectId = -1;  // 池满
}

cube::~cube() {
    if (m_objectId >= 0 && m_objectId < MAX_ID)
        s_idPool[m_objectId] = 0;
}

void cube::display() {
    OLED_DrawACube(m_x, m_y, m_z, m_anglex, m_angley, m_anglez, m_bianchang);
}

}
