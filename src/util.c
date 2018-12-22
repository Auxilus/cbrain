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

void save_brain_state(struct brain* b, char* filename)
{
	FILE *file;
	file = fopen(filename, "a");
	if (file == NULL) {
		printf("unable to open file\n");
		exit(1);
	}

	fprintf(file, "%u\n", b->nc);
	fprintf(file, "%u\n", b->nmax);
	for (int i = 0; i < b->nc; i++) {
		struct neuron* n = b->neurons[i];
		fprintf(file, "%u %u %u %u %u %u %u ", n->id, n->lc, n->lmax, n->thisstate, n->nextstate, n->fired, n->n_fired);
		for (int j = 0; j < n->lc; j++) {
			fprintf(file, "%u:%u ", n->links[j], n->wts[j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);

}
