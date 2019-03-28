#include "header.h"

int main(int argc, char* argv[])
{
	srand(time(0));
	if (argc < 2) {
		printf("Number of neurons required\n");
		exit(1);
	}
	int neurons_no = atoi(argv[1]);
	uint sleep_t;
	sleep_t = (argc < 3) ? 1 : atoi(argv[2]);

	struct brain* b = brain_init((uint)neurons_no);
	neuron_link_random(b);
	if (DEBUG >= 1) { printf("%d neurons generated and randomly linked\n", neurons_no); }
	struct nthread* nt1 = thread_struct_new(0, neurons_no-1);
	struct nthread* at  = thread_struct_new(0, 0);
	thread_create(nt1, b, 0, sleep_t);
	thread_create(at, b, 1, sleep_t);
	pthread_join(nt1->tid, NULL);
	pthread_join(at->tid, NULL);
	return 0;
}
