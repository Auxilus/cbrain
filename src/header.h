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

#ifndef CBRAIN_HEADER_H
#define CBRAIN_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

typedef unsigned int uint;

struct neuron
{
	uint id;
	uint *links;
	uint *wts;
	uint lc;
	uint lmax;

	uint thisstate;
	uint nextstate;

} neuron;

struct brain
{
	struct neuron** neurons;
	
	uint nc;
	uint nmax;
} brain;

struct nthread
{
	pthread_t tid;
	uint s;
	uint e;
	uint status;
} nthread;

struct neuron* make_neuron(uint);
struct brain* init_brain(int);
void link_neuron(struct neuron*, struct neuron*, uint);
void link_random_neuron(struct neuron*, struct brain*);
void unlink_neuron(struct neuron*, struct neuron*);
void accum_neuron(struct neuron*, uint);
void fire_neuron(struct neuron*, struct brain*);
int update_neuron(struct neuron*, struct brain*);
int update_range(uint, uint, struct brain*);
void show_stat(struct neuron*);

struct nthread* thread_struct_new(uint, uint);
int 		thread_create(struct nthread*, struct brain*);
void*		thread_func(void*);

int checkexist(uint, uint*, int);
int rand_int(int, int);

#endif