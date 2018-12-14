/*
Copyright (c) 2018 Auxilus

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

#include "brain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THRESHOLD 10
struct neuron* make_neuron(uint id)
{
	struct neuron* n = (struct neuron*)malloc(sizeof(struct neuron));
	n->id = id;
	n->lc = 0;
	n->links = (uint*)malloc(sizeof(uint)*10);
	if (n->links == NULL) {
		printf("Failed to allocate memory for links");
		exit(1);
	}
	n->wts = (uint*)malloc(sizeof(uint)*10);
	if (n->wts == NULL) {
		printf("Failed to allocate memory for links");
		exit(1);
	}
	n->lmax = 10;
	n->thisstate = 0;
	n->nextstate = 0;
	return n;
}

void link_neuron(struct neuron* src, struct neuron* n, uint wt)
{
	if (!(src->id == n->id)) {
		if (src->lc >= src->lmax) {
			printf("reallocating memory...");
			src->links = (uint*)realloc(src->links, sizeof(uint)*10);
			src->wts = (uint*)realloc(src->wts, sizeof(uint)*10);
			src->lmax += 10;
		}
		src->links[src->lc] = n->id;
		src->wts[src->lc] = wt;
		src->lc += 1;
	}
}

void accum_neuron(struct neuron* n, uint wt)
{
	n->nextstate += wt;
}

void update_neuron(struct neuron* n, struct brain* b)
{
	if (n->thisstate >= THRESHOLD) {
		fire_neuron(n, b);
	}
	n->thisstate += n->nextstate;
	n->nextstate = 0;
}

void update_all(struct brain* b)
{
	for (int i = 0; i < b->size; i++) {
		update_neuron(b->neurons[i], b);
	}
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
	b->neurons = (struct neuron**)malloc(sizeof(struct neuron) * s);
	for (int i = 0; i < s; i++) {
		struct neuron* n = make_neuron(i);
		b->neurons[i] = n;
	}
	b->size = s;
	return b;
}

int rand_int(int x, int y)
{
	int num;
	num = rand() % (y + 1 - x) + x;
	return num;
}

int checkexist(uint val, uint* arr, int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == val) {
			return 1;
		}
	}
	return 0;
}

void show_stat(struct neuron* n)
{
	printf("ID: %d  thisstate: %d  nextstate: %d\n", n->id, n->thisstate, n->nextstate);
}

int main()
{
	srand(time(0));
	printf("Creating brain...\t");
	struct brain* b = init_brain(10000000);
	printf(" done\n");
	int a = 0;
	printf("Making random connections...\t");
	for (int i = 0; i < 100;i++) {
		int src = rand_int(0, b->size - 1);
		int des = rand_int(0, b->size - 1);
		if (checkexist(des, b->neurons[src]->links, b->neurons[src]->lc) == 0) {
			link_neuron(b->neurons[src], b->neurons[des], rand_int(1
, 10));
		}
	}
	printf(" done\n");
	while (a == 0) {
		accum_neuron(b->neurons[rand_int(0, b->size - 1)], 20);
		update_all(b);
	}
	return 0;
}
