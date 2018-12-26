#include "header.h"

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

void save_brain(struct brain* b, char* filename)
{
	FILE *file;
	file = fopen(filename, "a");
	if (file == NULL) {
		printf("unable to open file\n");
		exit(1);
	}

	for (int i = 0; i < b->nc; i++) {
		struct neuron* n = b->neurons[i];
		fprintf(file, "%u %u %u ", n->id, n->lc, n->lmax);
		for (int j = 0; j < n->lc; j++) {
			fprintf(file, "%u %u ", n->links[j], n->wts[j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);

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
struct brain* gen_brain(char* filename)
{
	FILE* file;
	file = fopen(filename, "r");
	char line[1024];
	int lines = file_num_lines(filename);
	uint* ids = (uint*)malloc(sizeof(uint) * lines);
	uint* links = (uint*)malloc(sizeof(uint) * lines);
	uint* wts = (uint*)malloc(sizeof(uint) * lines);
	int c = 0;
	int b = 0;
	int a = 0;
	int set_l = 0;
	int set_w = 0;
	int set_lc = 0;
	int set_lmax = 0;
	uint lc, lmax;
	while (fgets(line, 1024, file)) {
		char* tmp = strdup(line);
		char* token = strtok(tmp, " ");
		ids[c] = (uint)atoi(token);
		c += 1;
		token = strtok(NULL, " ");
		printf("token is (lc): %s\n", token);
		lc = (uint)atoi(token);
		token = strtok(NULL, " ");
		printf("token is (lmax): %s\n", token);
		lmax = (uint)atoi(token);
		while (token != NULL) {
			token = strtok(NULL, " ");
			if (token == NULL) {
				printf("NULL token, exiting loop\n");
				break;
			}
			printf("token is: %s\n", token);
			if (set_lc == 0) {
				lc = (uint)atoi(token);
				set_lc = 1;
				continue;
			}
			if (set_lmax == 0) {
				lmax = (uint)atoi(token);
				set_lmax = 1;
				continue;
			}
			if (set_l == 0) {
				links[b] = (uint)atoi(token);
				set_l = 1;
				set_w = 0;
				b += 1;
				continue;
			}
			if (set_w == 0) {
				wts[a] = (uint)atoi(token);
				set_w = 1;
				set_l = 0;
				a += 1;
				continue;
			}
			token = strtok(NULL, " ");
		}
		free(tmp);
	}
}
