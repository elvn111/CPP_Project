#include <iostream>

using namespace std;

int main(void)
{
	char CH[60] = { "€2d\
; ?  ? )\ ? 殭 ? 吞探吞 ? " };

	for (int i = 0;i < 60;i++)
	{
		cout << (int)CH[i] << ' ';
	}
}