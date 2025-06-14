#include "../src/cbrain.h"
#include <unistd.h>

int main()
{
    srand((unsigned int)time(NULL));
    struct brain* b = brain_init(2, 0.1f);
    neuron_set_type(b->neurons[0], sensory);
    neuron_set_type(b->neurons[1], motor);
    neuron_link(b->neurons[0], b->neurons[1], 5);

    for (int t = 0; t < 10; t++) {
        if (t == 1) neuron_fire(b->neurons[0]);
        if (t == 3) neuron_fire(b->neurons[1]);
        neuron_update_range(0, 1, b);
        printf("step %u weight=%d\n", b->step, b->neurons[0]->wts[0]);
        usleep(100000);
    }
    return 0;
}

