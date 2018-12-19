#include "header.h"

int main(int argc, char* argv[])
{
	srand(time(0));
	if (argc < 2) {
		printf("Number of neurons required\n");
		exit(1);
	}
	pthread_t gmt;
	struct ngraph* ng = graphics_init();
	int neurons_no = atoi(argv[1]);
	struct brain* b = brain_init((uint)neurons_no);
	struct nthread* nt1 = thread_struct_new(0, 49);
	struct nthread* nt2 = thread_struct_new(50, 99);
	neuron_accum(b->neurons[rand_int(0, b->nc - 1)], 10);
	thread_create(nt1, b);
	thread_create(nt2, b);
	pthread_create(&gmt, NULL, graphics_event_monitor, (void*)ng);

	for (int i = 0; i < 100;i++) {
		int src = rand_int(0, b->nc - 1);
		neuron_link_random(b->neurons[src], b);
	}
	sleep(5);
	//pthread_join(nt1->tid, NULL);
	//pthread_join(nt2->tid, NULL);
	//pthread_join(gmt, NULL);
	graphics_quit(ng);
	return 0;
}
