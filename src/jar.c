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

struct jar* jar_init(int b_count, int nc)
{
	struct jar* j = (struct jar*)malloc(sizeof(struct jar));
	j->bc = b_count;
	j->bmax = b_count * 2;
	j->brains = (struct brain**)malloc(sizeof(struct brain) * j->bc);
	for (int i = 0; i < b_count; i++) {
		j->brains[i] = brain_init(nc);
	}
	j->fittest = (struct brain*)malloc(sizeof(struct brain));
	j->fittest = brain_init(-1);
	return j;
}

void jar_update(struct jar* j)
{
	for (int i = 0; i < j->bc; i++) {
		neuron_update_range(0, j->brains[i]->nc - 1, j->brains[i]);
		//brain_eval(0);
		brain_mutate(j->brains[i], false);
	}

	for (int k = 0; k < j->bc; k++) {
		if (j->brains[k]->fitness > j->fittest->fitness) {
			j->fittest = j->brains[k];
		}
	}
}

void brain_mutate(struct brain* b, bool force_mutate)
{
	for (int i = 0; i < b->nc; i++) {
		float random = rand_float(0, 1);
		if (random < MUTATE_PROB || force_mutate) {
			int l = rand_int(0, b->neurons[i]->lc - 1);
			int original_wt = b->neurons[i]->wts[l];
			int new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			while (new_wt == original_wt) {
				cbrain_print(2, "mutated weight is same as original weight, generating new weight\n");
				new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			}
			while (abs(new_wt - original_wt) > MAX_WT_DIFF) {
				cbrain_print(2, "mutated weight [%d] exceeds MAX_WT_DIFF [%d], generating new weight\n", new_wt, MAX_WT_DIFF);
				new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			}
			b->neurons[i]->wts[l] = new_wt;
			cbrain_print(0, "mutated %d:%d from weight %d to %d\n", i, b->neurons[i]->links[l], original_wt, new_wt);
		}
	}
}

/* eval only selected neurons for fitness */
void brain_eval_range(struct brain* b, int n_start, int n_end, float max)
{
	int accumulator = 0;
	for (int i = n_start; i <= n_end; i++) {
		if (b->neurons[i]->n_type == motor) {
			accumulator += b->neurons[i]->thisstate;
		}
	}
	float fit = 100 * accumulator / max;
	b->fitness = fit;
}

void brain_eval(struct brain* b, float max)
{
	int accumulator = 0;
	for (int i = 0; i < b->nc; i++) {
		if (b->neurons[i]->n_type == motor) {
			accumulator += b->neurons[i]->thisstate;
		}
	}
	float fit = 100 * accumulator / max;
	b->fitness = fit;
}
