/*
Copyright (c) 2018-2020 Auxilus raghavsphadke@gmail.com

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

/* initialize new neuron */
struct neuron* neuron_init(uint id, float decay)
{
	struct neuron* n = (struct neuron*)malloc(sizeof(struct neuron));
	n->id = id;
	n->lc = 0;
	n->lmax = 20;
	n->inc = 0;
	n->inmax = 20;
	n->links = (int*)malloc(4 * n->lmax);
	n->incoming = (int*)malloc(4 * n->inmax);
	n->wts = (int*)malloc(4 * n->lmax);
	n->thisstate = 0.0;
	n->nextstate = 0.0;
	n->state_decay = decay;
	n->fired = 0;
	n->n_fired = 0;
	n->n_type = undefined;
	return n;
}

/* link neuron 'n' to neuron 'src' with given weight 'wt' */
void neuron_link(struct neuron* src, struct neuron* n, int wt)
{
	cbrain_print(1, "linking neuron %d to %d with %d\n", src->id, n->id, wt);

	// link constraints:
	//		neuron shouldn't have a link with itself
	//		source neuron can't be a motor neuron
	//		target neuron can't be a sensory neuron
	if ((src->id != n->id) && (src->n_type != motor) && (n->n_type != sensory)) {

		// reallocate links and wts 
		if ((src->lc + 1) > src->lmax) {
			src->lmax *= 2;
			cbrain_print(2, "reallocating memory for links [id: %u]\n", src->id);
			src->links = (int*)realloc(src->links, sizeof(int) * src->lmax);
			src->wts = (int*)realloc(src->wts, sizeof(int) * src->lmax);
		}

		// reallocate incoming
		if ((n->inc + 1) > n->inmax) {
			n->inmax *= 2;
			cbrain_print(2, "reallocating memory for incoming [id: %u]\n", n->id);
			n->incoming = (int*)realloc(n->incoming, sizeof(int) * n->inmax);
		}

		src->links[src->lc] = n->id;
		src->wts[src->lc] = wt;
		src->lc += 1;
		// hold ref. of src neuron in linked neuron
		n->incoming[n->inc] = src->id;
		n->inc += 1;
	}
}

/* make random links */
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

/* remove a neural link */
void neuron_unlink(struct neuron* src, struct neuron* n)
{
	// make sure that the link exists for neuron 'n' in 'src->links'
	int pos = checkexist(n->id, src->links, (int)src->lc);
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
 * fire the neuron if it does and reset thisstate, nextstate
 */
int neuron_update(struct neuron* n, struct brain* b)
{
	// id of 'n' shouldn't be greater than the max id in brain 'b'
	assert(n->id <= (b->nc - 1));

	// fire neuron if thisstate exceeds THRESHOLD
	if (n->thisstate >= THRESHOLD) {
		n->f_type = self;
		for (int i = 0; i < n->lc; i++) {
			cbrain_print(4, "sending %d from %d to %d\n", n->wts[i], n->id, n->links[i]);
			b->neurons[n->links[i]]->nextstate += n->wts[i];
		}
		n->fired = 1;
		n->thisstate = 0;
		n->nextstate = 0;
		n->n_fired += 1;
	} else {
		n->thisstate += n->nextstate;

		// thisstate decay if next state is zero
		if (n->nextstate == 0 && n->thisstate > 0.0 && n->fired == 0) {
			// make sure thisstate completely decays
			n->thisstate -= ((n->thisstate - n->state_decay) < n->state_decay) ? n->thisstate : n->state_decay;
			cbrain_print(4, "[%d] decay %f\n", n->id, n->thisstate);
		} else {
			n->nextstate = 0;
		}
		n->fired = 0;
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
	n = neuron_init(b->nc, b->state_decay);
	if ((b->nc + 1) > b->nmax) {
		b->nmax *= 2;
		cbrain_print(2, "reallocating memory nc: %u\n", b->nc);
	}
	b->nc += 1;
	b->neurons = (struct neuron**)realloc(b->neurons, sizeof(struct neuron) * b->nmax);
	cbrain_print(1, "adding new neuron to brain with id: %d\n", n->id);
	b->neurons[b->nc] = n;
	
	for (int j = 0; j < 10; j++) {
		uint id = (uint)rand_int(0, b->nc - 1);
		if (!(id == n->id) && (checkexist(id, n->links, n->lc) == -1)) {
			cbrain_print(2, "linking %d to %d with weight\n", id, n->id);
			neuron_link(n, b->neurons[(int)id], rand_int(WEIGHT_MIN, WEIGHT_MAX));
		}
	}
	
}

/* set thisstate of neuron 'n' to THRESHOLD 
 * so that it will be fired when neuron_update is called next time
 */
void neuron_fire(struct neuron* n)
{
	n->f_type = user;
	n->thisstate = THRESHOLD;
}

/* initialize brain with given number of neurons 's' */
struct brain* brain_init(int s, float decay)
{
	struct brain* b = (struct brain*)malloc(sizeof(struct brain));
	b->neurons = (struct neuron**)malloc(sizeof(struct neuron) * s);
	b->nc = s;
	b->nmax = s;
	b->fitness = 0.0;
	b->state_decay = decay;

	// make neurons
	for (int i = 0; i < s; i++) {
		b->neurons[i] = neuron_init(i, decay);
	}
	return b;
}

void brain_reset(struct brain* b)
{
	for (int i = 0; i < b->nc; i++) {
		b->neurons[i]->thisstate = 0;
		b->neurons[i]->nextstate = 0;
		b->neurons[i]->fired = 0;
	}
}

void brain_neuron_type(struct brain* b, type t)
{
	for (int i = 0; i < b->nc; i++) {
		if (b->neurons[i]->n_type == t) {
			cbrain_print(0, "%d\n", b->neurons[i]->id);
		}
	}
}

void neuron_show_stat(struct neuron* n)
{
	cbrain_print(0, "ID: %d  thisstate: %f  nextstate: %f", n->id, n->thisstate, n->nextstate);
	for (int i = 0; i < n->lc; i++) {
		cbrain_print(0, "%u:%u ", n->links[i], n->wts[i]);
	}
	cbrain_print(0, "\n");
	for (int i = 0; i < n->inc; i++) {
		cbrain_print(0, "%u ", n->incoming[i]);
	}
	cbrain_print(0, "\n");
}
