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
	if (argc < 3) {
		sleep_t = 1;
	}
	else
		sleep_t   = atoi(argv[2]);

	printf("creating brain...\n");
	struct brain* b = brain_init((uint)neurons_no);
	printf("creating random links...\n");
	neuron_link_random(b);
	printf("creating threads...\n");
	struct nthread* nt1 = thread_struct_new(0, 999);
	struct nthread* nt2 = thread_struct_new(1000, 1999);
	struct nthread* nt3 = thread_struct_new(2000, 2999);
	struct nthread* at  = thread_struct_new(0, 0);
	thread_create(nt1, b, 0, sleep_t);
	thread_create(nt2, b, 0, sleep_t);
	thread_create(nt3, b, 0, sleep_t);
	thread_create(at, b, 1, sleep_t);
	pthread_join(nt1->tid, NULL);
	pthread_join(nt2->tid, NULL);
	pthread_join(nt3->tid, NULL);
	pthread_join(at->tid, NULL);
	return 0;
}
