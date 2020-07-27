#include "cbrain.h"
#include <stdarg.h>

int rand_int(int x, int y) 
{
	int num;
	num = rand() % (y + 1 - x) + x;
	return num;
}

char* get_type_name(type t)
{
	switch(t) {
		case undefined: return "undefined";
		case sensory: return "sensory";
		case intermediate: return "intermediate";
		case motor: return "motor";
		default: return "invalid type";
	}
}

float rand_float(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float p = random * diff;
	float ret = (p + min);
	return ret;
}

int checkexist(int val, int* arr, int size)
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
	cbrain_print(1, "saving brain to file %s\n", filename);
	if (file) {
		for (int i = 0; i < b->nc; i++) {
			for (int j = 0; j < b->neurons[i]->lc; j++) {
				char str[15];
				sprintf(str, "%d, %d, %d\n", i, b->neurons[i]->links[j], b->neurons[i]->wts[j]);
				fwrite(str, sizeof(char), strlen(str), file);
			}
		}
		fclose(file);
	} else {
		printf("unable to open file\n");
		exit(1);
	}
}

struct brain* gen_brain(char* filename)
{
	struct brain* ret_b = (struct brain*)malloc(sizeof(struct brain));
	FILE* file = fopen(filename, "r");
	int ret = 0;
	if (file) {
		fread(ret_b, sizeof(struct brain), 1, file);
		fclose(file);
		ret = 1;
	}
	if (ret == 1) {
		for (int i = 0; i < ret_b->nc - 1; i++) {
			show_stat(ret_b->neurons[i]);
		}
		return ret_b;
	}
	else {
		free(ret_b);
		return NULL;
	}
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
	va_end(list);
	return 0;
}

struct brain* parse_model_csv(char* file)
{
	struct brain* b = brain_init(500);
	FILE* f = fopen(file, "r");
	if (f == NULL) {
		printf("failed to load file %s\n", file);
		exit(1);
	}
	char line[64];

	while (fgets(line, sizeof(line), f)) {
		char* token = strtok(line, "£");
		int src = atoi(strtok(line, ","));
		int dst = atoi(strtok(NULL, ","));
		int wt = atoi(strtok(NULL, ","));
		cbrain_print(1, "adding neuron link %d -> %d : %d\n", src, dst, wt);
		neuron_link(b->neurons[src], b->neurons[dst], wt);
		token++;
	}
	fclose(f);
	return b;
}