#include "../src/cbrain.h"
#include "defs.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
	float state_decay = rand_float(0.1, 0.5);
	struct brain* b = brain_init(200, state_decay);
	printf("-------------------\n");
	printf("building connectome\n");
	printf("using state decay %f\n", state_decay);
	printf("-------------------\n");

    int inputs[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int outputs[10] = { 190, 191, 192, 193, 194, 195, 196, 197, 198, 199 };

    for (int n = 0; n < 10; n++) { neuron_set_type(b->neurons[inputs[n]], sensory); }
    for (int m = 0; m < 10; m++) { neuron_set_type(b->neurons[outputs[m]], motor); }
    
    for (int i = 0; i < 190; i++) {
        for(int j = 0; j < 10; j++) {
            int target = rand_int(11, b->nc-1);
            while (target == i) {
                target = rand_int(11, b->nc-1);
            }
            neuron_link(b->neurons[i], b->neurons[target], rand_int(1, THRESHOLD));
        }
    }

    for (;;) {
        int max = 0;
        while (max < 9) {
            int success = 0;
            for (int i = 0; i < 10; i++) {
                neuron_fire(b->neurons[inputs[i]]);
                neuron_update_range(0, 199, b);
                if (b->neurons[outputs[i]]->fired) { success += 1; }
            }
            printf("success: %f\n", (float)success / (float)10);
            if (success > max) { max = success; }
            brain_reset(b);
            tem_m(b);
        }
    }

}