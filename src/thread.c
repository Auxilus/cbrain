#include "header.h"

struct thread_args
{
	uint s;
	uint e;
	struct brain* b;
} thread_args;

struct nthread* thread_struct_new(uint start, uint end)
{
	struct nthread* nt = (struct nthread*)malloc(sizeof(struct nthread));
	nt->s = start;
	nt->e = end;
	nt->status = 0;
	return nt;
}

int thread_create(struct nthread* nt, struct brain* b)
{
	struct thread_args* ta = (struct thread_args*)malloc(sizeof(struct thread_args));
	ta->s = nt->s;
	ta->e = nt->e;
	ta->b = b;
	int ret = pthread_create(&nt->tid, NULL, thread_func, (void*)ta);
	return ret;
}

void *thread_func(void* args)
{
	uint start = ((struct thread_args*)args)->s;
	uint end   = ((struct thread_args*)args)->e;
	struct brain* b = ((struct thread_args*)args)->b;
	int fired = update_range(start, end, b);
}
