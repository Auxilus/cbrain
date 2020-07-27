#include "cbrain.h"

struct sdlctx* render_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		exit(1);
	} else {
		struct sdlctx* ctx = (struct sdlctx*)malloc(sizeof(struct sdlctx));
		ctx->win = SDL_CreateWindow("cbrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
		printf("%s\n", SDL_GetError());
		ctx->ren = SDL_CreateRenderer(ctx->win, -1, 0);
		printf("%s\n", SDL_GetError());
		printf("%p %p\n", ctx->win, ctx->ren);
		return ctx;
	}
}

SDL_Event render_get_event()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	return event;
}
void render_handle_events(struct sdlctx* ctx, struct brain* b)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			render_cleanup(ctx);
			break;
		case SDL_KEYDOWN:
			if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "Q") == 0) {
				render_cleanup(ctx);
			}
			if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "T") == 0) {
				neuron_fire(b->neurons[8], b);
				neuron_fire(b->neurons[9], b);
				printf("stimulating temperature sensor\n");
			}
			if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "F") == 0) {
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
				printf("giving food\n");
			}
			if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "M") == 0) {
				//brain_mutate(b, true);
				printf("mutating brain\n");
			}

			break;
	}
}

void render_update(struct sdlctx* ctx, struct entityctx* ec, struct brain* b)
{
	// check for collision
	int collision = 0;
	printf("x: %f y: %f\n", ec->x, ec->y);
	if (ec->x <= 0) { ec->x = 0; collision = 1; }
	if (ec->y <= 0) { ec->y = 0; collision = 1; }
	if (ec->x >= (600 - ec->height)) { ec->x = 600-ec->height; collision = 1; }
	if (ec->y >= (400 - ec->height)) { ec->y = 400-ec->height; collision = 1; }
	if (collision) {
		neuron_fire(b->neurons[111], b);
		neuron_fire(b->neurons[112], b);
		neuron_fire(b->neurons[43], b);
		neuron_fire(b->neurons[44], b);
		neuron_fire(b->neurons[127], b);
		neuron_fire(b->neurons[128], b);
		neuron_fire(b->neurons[151], b);
		neuron_fire(b->neurons[152], b);
		neuron_fire(b->neurons[153], b);
		neuron_fire(b->neurons[154], b);
	}
	int acc_right = 0;
	int acc_left = 0;
	float dx = 0;
	float dy = 0;
	for (int i = ec->mrstart; i < ec->mrend; i++) {
		acc_right += b->neurons[i]->thisstate;
		cbrain_print(2, "reducing thisstate for %d from %f to %f\n", b->neurons[i]->id, b->neurons[i]->thisstate, b->neurons[i]->thisstate*0.7);
		b->neurons[i]->thisstate *= 0.7;
	}

	for (int i = ec->mlstart; i < ec->mlend; i++) {
		int st = b->neurons[i]->thisstate;
		acc_left += st;
		b->neurons[i]->thisstate *= 0.7;
	}

	float speed = acc_right + acc_left;
	speed = speed / 200.0;

	if (acc_right > acc_left) {
		ec->rot -= 5.0;
	} else if (acc_left > acc_right) {
		ec->rot += 5.0;
	}

	dx = cosf(ec->rot / RADTODEG) * speed;
	dy = sinf(ec->rot / RADTODEG) * speed;

	ec->x += dx;
	ec->y += dy;
}

void render_draw(struct sdlctx* ctx, struct entityctx* ec)
{
	float v1x = 0.0f;
	float v2x = 0.0f;
	float v3x = 0.0f;
	float v4x = 0.0f;
	float v1y = 0.0f;
	float v2y = 0.0f;
	float v3y = 0.0f;
	float v4y = 0.0f;

	if (ec->rot != 0) {
		float ca = cosf(ec->rot * 3.14159 / 180);
		float sa = sinf(ec->rot * 3.14159 / 180);

		float cx = ec->x + 20;
		float cy = ec->y + 10;

		float x = ec->x - cx;
		float y = ec->y - cy;
		float w = ec->x + ec->width - cx;
		float h = ec->y + ec->height - cy;

		v1x = x*ca - y*sa + cx;
		v1y = x*sa + y*ca + cy;

		v2x = w*ca - y*sa + cx;
		v2y = w*sa + y*ca + cy;

		v3x = w*ca - h*sa + cx;
		v3y = w*sa + h*ca + cy;

		v4x = x*ca - h*sa + cx;
		v4y = x*sa + h*ca + cy;
	} else {
		v1x = ec->x - 20;
		v1y = ec->y - 10;
		v2x = ec->x + ec->width - 20;
		v2y = ec->y - 10;
		v3x = ec->x + ec->width - 20;
		v3y = ec->y + ec->height - 10;
		v4x = ec->x - 20;
		v4y = ec->y + ec->height - 10;
	}

	SDL_SetRenderDrawColor(ctx->ren, 255, 255, 255, 255);
	SDL_RenderClear(ctx->ren);
	SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 255);
	SDL_RenderDrawLine(ctx->ren, v4x, v4y, v1x, v1y);
	SDL_RenderDrawLine(ctx->ren, v1x, v1y, v2x, v2y);
	SDL_RenderDrawLine(ctx->ren, v2x, v2y, v3x, v3y);
	SDL_RenderDrawLine(ctx->ren, v3x, v3y, v4x, v4y);
	SDL_RenderPresent(ctx->ren);
}

void render_cleanup(struct sdlctx* ctx)
{
	printf("Quitting...\n");
	SDL_DestroyWindow(ctx->win);
	SDL_DestroyRenderer(ctx->ren);
	SDL_Quit();
	exit(1);
}

struct entityctx* render_spawn(int mls, int mrs, int mle, int mre)
{
	struct entityctx* ec = (struct entityctx*)malloc(sizeof(struct entityctx));
	ec->x = 300;
	ec->y = 200;
	ec->rot = 0.0f;
	ec->width = 40;
	ec->height = 20;
	ec->mlstart = mls;
	ec->mrstart = mrs;
	ec->mlend = mle;
	ec->mrend = mre;
	return ec;
}
