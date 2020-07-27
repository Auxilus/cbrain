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

/* Initialize new neuron */
struct neuron* neuron_init(uint id)
{
	struct neuron* n = (struct neuron*)malloc(sizeof(struct neuron));
	n->id = id;
	n->lc = 0;
	n->lmax = 100;
	n->links = (int*)malloc(400); /* sizeof(int) * n->lmax */
	n->wts = (int*)malloc(400);   /* |----4----| * |-100-| */
	n->thisstate = 0.0;
	n->nextstate = 0.0;
	n->fired     = 0;
	n->n_fired   = 0;
	n->n_type   = undefined;
	return n;
}

/* Link n to src with given weight */
void neuron_link(struct neuron* src, struct neuron* n, uint wt)
{
	printf("linking neuron %d to %d with %d\n", src->id, n->id, wt);
	if ((src->id != n->id) && (src->n_type != motor) && (n->n_type != sensory)) {
		if ((src->lc + 1) > src->lmax) {
			src->lmax *= 2;
			cbrain_print(2, "reallocating memory... nid: %u\n", src->id);
			src->links = (int*)realloc(src->links, sizeof(int) * src->lmax);
			src->wts = (int*)realloc(src->wts, sizeof(int) * src->lmax);
		}
		src->links[src->lc] = n->id;
		src->wts[src->lc] = wt;
		src->lc += 1;
	}
}

void neuron_link_random(struct brain* b)
{
	for (int i = 0; i < (int)b->nc; i++) {
		for (int j = 0; j < 10; j++) {
			uint id = (uint)rand_int(0, b->nc - 1);
			if (!(id == b->neurons[i]->id) && (checkexist(id, b->neurons[i]->links, b->neurons[i]->lc) == -1)) {
				neuron_link(b->neurons[i], b->neurons[(int)id], rand_int(WEIGHT_MIN, WEIGHT_MAX));
			}
		}
	}
}

void neuron_unlink(struct neuron* src, struct neuron* n)
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

/* accumulate passed weight to neuron's nextstate */
void neuron_accum(struct neuron* n, uint wt)
{
	cbrain_print(2, "accumulating neuron %d with weight %d\n", n->id, wt);
	pthread_mutex_lock(&lock);
	n->nextstate += wt;
	pthread_mutex_unlock(&lock);
}

/* check if neuron's thisstate exceeds the threshold,
 * fire the neuron if it does and reset thisstate, nextstate accordingly
 */
int neuron_update(struct neuron* n, struct brain* b)
{
	//if (DEBUG == 1) { show_stat(n); }
	assert(n->id <= (b->nc - 1));
	if (n->thisstate >= THRESHOLD) {
		cbrain_print(3, "firing neuron %d\n", n->id);
		for (int i = 0; i < n->lc; i++) {
			assert(n->links[i] <= (b->nc - 1));
			assert(n->wts[i]);
			cbrain_print(4, "sending weight %d to %d\n", n->wts[i], n->links[i]);
			float past = b->neurons[n->links[i]]->nextstate;
			b->neurons[n->links[i]]->nextstate += n->wts[i];
			if (b->neurons[n->links[i]]->nextstate > 1000) {
				cbrain_print(2, "faulty fire from %d to %d [%d] (is %f was %f)\n", n->id, n->links[i], n->wts[i], b->neurons[n->links[i]]->nextstate, past);
				exit(-1);
			}
		}
		n->thisstate = 0;
		n->nextstate = 0;
		n->fired = 1;
		n->n_fired += 1;
	} else {
		n->thisstate += n->nextstate;
		n->nextstate = 0;
		n->fired     = 0;
	}
	return n->fired;
}

int neuron_update_range(uint s, uint e, struct brain* b)
{
	int nf = 0;
	for (int i = s; i <= e; i++) {
		int fired = neuron_update(b->neurons[i], b);
		if (fired == 1) {
			nf += 1;
		}
	}
	return nf;
}

void neuron_set_type(struct neuron* n, type t)
{
	n->n_type = t;
}

void neuron_add(struct brain* b)
{
	struct neuron* n;
	n = neuron_init(b->nc);
	if ((b->nc + 1) > b->nmax) {
		b->nmax *= 2;
		cbrain_print(2, "reallocating memory... nc: %u\n", b->nc);
	}
	b->nc += 1;
	b->neurons = (struct neuron**)realloc(b->neurons, sizeof(struct neuron) * b->nmax);
	cbrain_print(1, "adding new neuron to brain with id: %d\n", n->id);
	b->neurons[b->nc] = n;
	
	for (int j = 0; j < 10; j++) {
		uint id = (uint)rand_int(0, b->nc - 1);
		if (!(id == n->id) && (checkexist(id, n->links, n->lc) == -1)) {
			cbrain_print(1, "linking %d to %d with weight\n", id, n->id);
			neuron_link(n, b->neurons[(int)id], rand_int(WEIGHT_MIN, WEIGHT_MAX));
		}
	}
	
}

/* send respective weights to links */
void neuron_fire(struct neuron* n, struct brain* b)
{
	n->thisstate = THRESHOLD;
}

struct brain* brain_init(int s)
{
	struct brain* b = (struct brain*)malloc(sizeof(struct brain));
	b->nc = s;
	b->nmax = b->nc;
	b->fitness = 0.0;
	b->neurons = (struct neuron**)malloc(sizeof(struct neuron) * b->nmax);
	for (int i = 0; i < s; i++) {
		struct neuron* n = neuron_init(i);
		b->neurons[i] = n;
	}
	return b;
}

void brain_neuron_type(struct brain* b, type t)
{
	printf("[%s]\n", get_type_name(t));
	for (int i = 0; i < b->nc; i++) {
		if (b->neurons[i]->n_type == t) {
			printf("%d\n", b->neurons[i]->id);
		}
	}
}

void show_stat(struct neuron* n)
{
	printf("ID: %d  thisstate: %f  nextstate: %f", n->id, n->thisstate, n->nextstate);
	for (int i = 0; i < n->lc; i++) {
		printf("%u:%u ", n->links[i], n->wts[i]);
	}
	printf("\n");
}
