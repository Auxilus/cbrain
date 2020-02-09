/*
Copyright (c) 2018 Auxilus raghavsphadke@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "cbrain.h"

struct thread_args {
	uint s;
	uint e;
	uint f;		/* -1:fire 0:update 1:accum*/
	uint sleep_t;
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

struct thread_bank* thread_bank_new(uint s)
{
	struct thread_bank* tb = (struct thread_bank*)malloc(sizeof(struct thread_bank));
	tb->tmax = s;
	tb->tc = 0;
	tb->threads = (struct nthread**)malloc(sizeof(struct nthread) * tb->tmax);
	return tb;
}

int thread_create(struct nthread* nt, struct brain* b, uint f, uint sleep_t)
{
	struct thread_args* ta = (struct thread_args*)malloc(sizeof(struct thread_args));
	ta->s = nt->s;
	ta->e = nt->e;
	ta->b = b;
	ta->f = f;
	ta->sleep_t = sleep_t;
	int ret = pthread_create(&nt->tid, NULL, thread_func, (void*)ta);
	return ret;
}

void* thread_func(void* args)
{
	uint start = ((struct thread_args*)args)->s;
	uint end   = ((struct thread_args*)args)->e;
	uint f     = ((struct thread_args*)args)->f;
	uint sleep_t = ((struct thread_args*)args)->sleep_t;
	struct brain* b = ((struct thread_args*)args)->b;
	if (f == -1) {
		for (;;) {
			for (int i = 0; i < (int)b->nc; i++) {
				neuron_fire(b->neurons[i], b);
				sleep(sleep_t);
			}
		}
	}
	if (f == 0) {
		for (;;) {
			neuron_update_range(start, end, b);
			brain_mutate(b, false);
			sleep(sleep_t);
		}
	}
	if (f == 1) {
		for (;;) {
			neuron_accum(b->neurons[rand_int(0, b->nc - 1)], rand_int(1, 10));
			sleep(sleep_t);
		}
	}
	return (void*)NULL;
}
