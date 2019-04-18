#include "header.h"

struct jar* jar_init(int b_count)
{
	struct jar* j = (struct jar*)malloc(sizeof(struct jar));
	j->bc = b_count;
	j->bmax = b_count * 2;
	j->brains = (struct brain**)malloc(sizeof(struct brain) * j->bc);
	for (int i = 0; i < b_count; i++) {
		j->brains[i] = brain_init(rand_int(2, 1000));
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
		brain_mutate(j->brains[i]);
	}

	for (int k = 0; k < j->bc; k++) {
		if (j->brains[k]->fitness > j->fittest->fitness) {
			j->fittest = j->brains[k];
		}
	}
}

void brain_mutate(struct brain* b)
{
	for (int i = 0; i < b->nc; i++) {
		float random = rand_float(0, 1);
		if (random < 0.00000001) {
			int l = rand_int(0, b->neurons[i]->lc - 1);
			b->neurons[i]->wts[l] = rand_int(1, 20);
			cbrain_print(0, "mutated %d:%d with weight %d\n", i, b->neurons[i]->links[l], b->neurons[i]->wts[l]);
		}
	}
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
