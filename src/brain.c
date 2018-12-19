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

#include "header.h"

#define THRESHOLD 5

struct neuron* make_neuron(uint id)
{
	struct neuron* n = (struct neuron*)malloc(sizeof(struct neuron));
	n->id = id;
	n->lc = 0;
	n->lmax = 10;
	n->links = (uint*)malloc(sizeof(uint) * n->lmax);
	n->wts = (uint*)malloc(sizeof(uint) * n->lmax);
	n->thisstate = 0;
	n->nextstate = 0;
	return n;
}

void link_neuron(struct neuron* src, struct neuron* n, uint wt)
{
	if (!(src->id == n->id)) {
		src->lc += 1;
		if (src->lc >= src->lmax) {
			src->lmax *= 2;
			printf("reallocating memory...");
			src->links = (uint*)realloc(src->links, sizeof(uint) * src->lmax);
			src->wts = (uint*)realloc(src->wts, sizeof(uint) * src->lmax);
		}
		src->links[src->lc] = n->id;
		src->wts[src->lc] = wt;
	}
}

void link_random_neuron(struct neuron* n, struct brain* b)
{
	int des = rand_int(0, b->nmax - 1);
	if (checkexist(des, n->links, n->lc) == -1) {
		link_neuron(n, b->neurons[des], rand_int(10, 30));
	}

}

void unlink_neuron(struct neuron* src, struct neuron* n)
{
	int pos;
	pos = checkexist(n->id, src->links, (int)src->lc);
	if (!(pos == -1)) {
		if (pos < src->lc) {
			src->links[pos] = src->links[pos+1];
			src->wts[pos] = src->wts[pos+1];
			src->lc -= 1;
		}
		else {
			src->lc -= 1;
		}
	}
}

void accum_neuron(struct neuron* n, uint wt)
{
	n->nextstate += wt;
}

int update_neuron(struct neuron* n, struct brain* b)
{
	int fired = 0;
	if (n->thisstate >= THRESHOLD) {
		fire_neuron(n, b);
		fired = 1;
	}
	n->thisstate += n->nextstate;
	n->nextstate = 0;
	return fired;
}

int update_range(uint s, uint e, struct brain* b)
{
	int nf = 0;
	for (int i = s; i <= e; i++) {
		int fired = update_neuron(b->neurons[i], b);
		if (fired == 1) {
			nf += 1;
		}
	}
	return nf;
}

void fire_neuron(struct neuron* n, struct brain* b)
{
	int p;
	p = n->lc;
	for (int i = 0; i < p; i++) {
		b->neurons[n->links[i]]->nextstate += n->wts[i];
		n->thisstate = 0;
		n->nextstate = 0;
	}
	printf("neuron %u fired\n", n->id);
}

struct brain* init_brain(int s)
{
	struct brain* b = (struct brain*)malloc(sizeof(struct brain));
	b->nc = s;
	b->nmax = b->nc;
	b->neurons = (struct neuron**)malloc(sizeof(struct neuron) * b->nmax);
	for (int i = 0; i < s; i++) {
		struct neuron* n = make_neuron(i);
		b->neurons[i] = n;
	}
	return b;
}


void show_stat(struct neuron* n)
{
	printf("ID: %d  thisstate: %d  nextstate: %d\n", n->id, n->thisstate, n->nextstate);
}

int main(int argc, char* argv[])
{
	srand(time(0));
	if (argc < 2) {
		printf("Number of neurons required\n");
		exit(1);
	}
	int neurons_no = atoi(argv[1]);
	sleep(1);
	struct brain* b = init_brain((uint)neurons_no);
	struct nthread* nt1 = thread_struct_new(0, 49);
	struct nthread* nt2 = thread_struct_new(50, 99);
	accum_neuron(b->neurons[rand_int(0, b->nc - 1)], 10);
	thread_create(nt1, b);
	thread_create(nt2, b);

	for (int i = 0; i < 100;i++) {
		int src = rand_int(0, b->nc - 1);
		link_random_neuron(b->neurons[src], b);
	}
	pthread_join(nt1->tid, NULL);
	pthread_join(nt2->tid, NULL);
	return 0;
}
