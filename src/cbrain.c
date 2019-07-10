#include "header.h"

int main(int argc, char* argv[])
{
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("mutex init failed\n");
		return 1;
	}
	cbrain_print(0, "mutex initiated at %p\n", &lock);
	srand(time(0));
	//if (argc < 2) {
	//	printf("Number of neurons required\n");
	//	exit(1);
	//}
	//int neurons_no = atoi(argv[1]);
	uint sleep_t;
	sleep_t = (argc < 3) ? SLEEP_T : strtof(argv[2], NULL);

	struct brain* b = parse_model_csv("src/models/conn.txt");

	//struct brain* b = brain_init((uint)neurons_no);
	//neuron_link_random(b);
	//cbrain_print(0, "%d neurons generated and randomly linked\n", b->nc);
	// run an infinite loop which gives food to c elegans
	for (;;) {
		neuron_fire(b->neurons[4], b);
		neuron_fire(b->neurons[5], b);
		neuron_fire(b->neurons[42], b);
		neuron_fire(b->neurons[41], b);
		neuron_fire(b->neurons[45], b);
		neuron_fire(b->neurons[46], b);
		neuron_fire(b->neurons[48], b);
		neuron_fire(b->neurons[76], b);
		neuron_fire(b->neurons[77], b);
		neuron_fire(b->neurons[72], b);
		neuron_fire(b->neurons[73], b);
		neuron_update_range(0, 499, b);
	}
	//struct nthread* nt1 = thread_struct_new(0, neurons_no-1);
	//thread_create(nt1, b, 0, sleep_t);
	//struct nthread* at  = thread_struct_new(0, 0);
	//thread_create(at, b, 1, sleep_t);
	//pthread_join(nt1->tid, NULL);
	//pthread_join(at->tid, NULL);
	return 0;
}
