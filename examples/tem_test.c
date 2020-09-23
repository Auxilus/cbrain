#include "../src/cbrain.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
	float state_decay = rand_float(0.1, 0.5);
	struct brain* b = brain_init(8, state_decay);

	printf("-------------------\n");
	printf("building connectome\n");
	printf("using state decay %f\n", state_decay);
	printf("-------------------\n");

    neuron_set_type(b->neurons[0], sensory);
    neuron_set_type(b->neurons[1], sensory);
    neuron_set_type(b->neurons[2], intermediate);
    neuron_set_type(b->neurons[3], intermediate);
    neuron_set_type(b->neurons[4], intermediate);
    neuron_set_type(b->neurons[5], intermediate);
    neuron_set_type(b->neurons[6], intermediate);
    neuron_set_type(b->neurons[5], motor);

    neuron_link(b->neurons[0], b->neurons[3], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[1], b->neurons[2], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[1], b->neurons[3], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[2], b->neurons[4], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[3], b->neurons[6], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[4], b->neurons[5], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[4], b->neurons[7], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[6], b->neurons[7], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[5], b->neurons[6], rand_float(-THRESHOLD, THRESHOLD));
    neuron_link(b->neurons[5], b->neurons[2], rand_float(-THRESHOLD, THRESHOLD));

    int i1[4] = { 0, 0, 1, 1 };
    int i2[4] = { 0, 1, 0, 1 };
    int op[4] = { 0, 1, 1, 1 };

}