#include "util.h"
#include <stdlib.h>

int rand_int(int x, int y)
{
	int num;
	num = rand() % (y + 1 - x) + x;
	return num;
}

int checkexist(uint val, uint* arr, int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == val) {
			return i;
		}
	}
	return -1;
}
