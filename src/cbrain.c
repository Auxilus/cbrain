#include "header.h"

int main(int argc, char* argv[])
{
	srand(time(0));
	if (argc < 2) {
		printf("Number of neurons required\n");
		exit(1);
	}
	int neurons_no = atoi(argv[1]);
	struct brain* b = brain_init((uint)neurons_no);
	neuron_link_random(b);
	struct nthread* nt1 = thread_struct_new(0, 99);
	struct nthread* at  = thread_struct_new(0, 0);
	thread_create(nt1, b, 0);
	thread_create(at, b, 1);
	pthread_join(nt1->tid, NULL);
	pthread_join(at->tid, NULL);
	return 0;
}
