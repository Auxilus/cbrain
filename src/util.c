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
		for (int j = 0; j < n->lc; j++) {
			fprintf(file, "%u,%u,%u\n", n->id, n->links[j], n->wts[j]);
		}
	}
	fclose(file);

}
