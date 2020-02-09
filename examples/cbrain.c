#include "../src/cbrain.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
	struct brain* b = parse_model_csv("models/conn.txt");
	struct sdlctx* ctx = render_init();
	struct entityctx* ec = render_spawn(300, 347, 347, 395);
	int ctr = 0;
	int ittr = 0;
	for (;;) {
		render_handle_events(ctx, b);
		render_update(ctx, ec, b);
		render_draw(ctx, ec);
		neuron_update_range(0, 499, b);
		usleep(20000);
		ittr += 1;
	}
	return 0;
}
