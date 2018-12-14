typedef unsigned int   uint;
typedef unsigned long  ulong;
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
	uint size;
} brain;

struct neuron* make_neuron(uint);
void link_neuron(struct neuron*, struct neuron*, uint);
void accum_neuron(struct neuron*, uint);
void fire_neuron(struct neuron*, struct brain*);
void update_neuron(struct neuron*, struct brain*);
void update_all(struct brain*);
struct brain* init_brain(int);

int rand_int(int, int);
int checkexist(uint, uint*, int);
void show_stat(struct neuron*);
