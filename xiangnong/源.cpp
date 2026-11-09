#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

float dp[400][400] = { 0 };
int x[400] = { 0 };
int y[400] = { 0 };
int fare[100] = { 0 };
float min_fare[10] = { 0 };
int main(void) {
	int N;
	cin >> N;
	int S, T, A, B;
	for(int l=0;l<N;l++){
		memset(dp, 0, sizeof(int) * 400 * 400);
		memset(y, 0, sizeof(int) * 400);
		memset(x, 0, sizeof(int) * 400);
		memset(fare, 0, sizeof(int) * 100);
		cin >> S >> T >> A >> B;
		for (int i = 0;i < S;i++) {
			cin >> x[i * 4 + 0] >> y[i * 4 + 0] >> x[i * 4 + 1] >> y[i * 4 + 1] >> x[i * 4 + 2] >> y[i * 4 + 2] >> fare[i];
			int x1 = x[i * 4 + 0], x2 = x[i * 4 + 1], x3 = x[i * 4 + 2];
			int y1 = y[i * 4 + 0], y2 = y[i * 4 + 1], y3 = y[i * 4 + 2];
			if (((x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1)) == 0) {
				x[i * 4 + 3] = x3 + x2 - x1;
				y[i * 4 + 3] = y3 + y2 - y1;
			}
			else if (((x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2)) == 0) {
				x[i * 4 + 3] = x3 - x2 + x1;
				y[i * 4 + 3] = y3 - y2 + y1;
			}
			else if (((x1 - x3) * (x2 - x3) + (y1 - y3) * (y2 - y3)) == 0) {
				x[i * 4 + 3] = x1 + x2 - x3;
				y[i * 4 + 3] = y1 + y2 - y3;
			}
		}
		for (int i = 0;i < S * 4;i++)
			for (int j = 0;j < S * 4;j++) 
				if (i == j)continue;
				else if (i / 4 == j / 4)
					dp[i][j] = sqrtf((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) * fare[i / 4];
				
				else
					dp[i][j] = sqrtf((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) * T;
		for (int k = 0;k < S * 4;k++) 
			for (int i = 0;i < S * 4;i++) 
				for (int j = 0;j < S * 4;j++) 
					if (dp[i][k] + dp[k][j] < dp[i][j])
						dp[i][j] = dp[i][k] + dp[k][j];
		float  min = 1000000;
		for (int i = 0;i < 4;i++)
			for (int j = 0;j < 4;j++)
				if (min > dp[(A - 1) * 4 + i][(B - 1) * 4 + j])min = dp[(A - 1) * 4 + i][(B - 1) * 4 + j];
		min_fare[l] = min;
	}
	for (int i = 0;i < N;i++)
		printf("%.1f \n", min_fare[i]);
}