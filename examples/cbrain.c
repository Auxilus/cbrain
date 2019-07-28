#include "../src/cbrain.h"

int main(int argc, char* argv[])
{
	struct brain* b = parse_model_csv("models/conn.txt");
	struct sdlctx* ctx = render_init();
	struct entityctx* ec = render_spawn(300, 347, 347, 395);
	int ctr = 0;
	for (;;) {
		render_handle_events(ctx);
		render_update(ctx, ec, b);
		render_draw(ctx, ec);
		neuron_fire(b->neurons[4], b);
		neuron_fire(b->neurons[5], b);
		neuron_fire(b->neurons[42], b);
		neuron_fire(b->neurons[41], b);
		neuron_fire(b->neurons[45], b);
		neuron_fire(b->neurons[46], b);
		neuron_fire(b->neurons[48], b);
		neuron_fire(b->neurons[76], b);
		neuron_fire(b->neurons[77], b);
		neuron_fire(b->neurons[72], b);
		neuron_fire(b->neurons[73], b);
		neuron_update_range(0, 499, b);
	}
	return 0;
}
