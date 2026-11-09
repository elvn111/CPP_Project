#pragma
/*bsp.h*/

#include <stdlib.h>
#include <stdint.h>

typedef struct GPS_Module_Info {
	double latitude;
	double longitude;
	double ElpHeight;
	double Heading_Angle;
	double Baseline;
	double default_base_line;
	uint8_t GPS_Satellite;
} GPS_Module_Info;

typedef enum {
	INFO_ERROE = 0,
	GPNAV = 1,
	GPYBM = 2
}info_type_def;

int gps_info_rsl(char* ptr, GPS_Module_Info* pInfo);

