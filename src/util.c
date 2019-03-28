#include "header.h"
#include <stdarg.h>

int rand_int(int x, int y) 
{
	int num;
	num = rand() % (y + 1 - x) + x;
	return num;
}

float rand_float(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float p = random * diff;
	float ret = (p + min);
	return ret;
}

int checkexist(uint val, uint* arr, int size)
{
	for (int i = 0; i < size; i++) {
		if (arr[i] == val) {
			return i;
		}
	}
	return -1;
}

int file_num_lines(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("unable to open file\n");
		exit(1);
	}
	int lines = 0;
	char line[1024];
	while (fgets(line, 1024, file)) {
		lines += 1;
	}
	fclose(file);
	return lines;
}

void save_brain(struct brain* b, char* filename)
{
	FILE* file = fopen(filename, "wb");
	if (file) {
		fwrite(b, sizeof(struct brain), 1, file);
		fclose(file);
	}
}

struct brain* gen_brain(char* filename)
{
	struct brain* ret_b = (struct brain*)malloc(sizeof(struct brain));
	FILE* file = fopen(filename, "r");
	int ret = 0;
	if (file) {
		fread(ret_b, sizeof(struct brain), 1, file);
		ret = 1;
	}
	if (ret == 1) {
		for (int i = 0; i < ret_b->nc - 1; i++) {
			show_stat(ret_b->neurons[i]);
		}
		return ret_b;
	}
	else
		return NULL;

}

int cbrain_print(int level, const char* str, ...)
{
	va_list list;
	va_start(list, str);
	fflush(stdout);
	fflush(stderr);
	if (level <= DEBUG) {
		vfprintf(stdout, str, list);
	}
	return 0;
}
