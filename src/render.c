#include "cbrain.h"

struct sdlctx* render_init()
{
	struct sdlctx* ctx = (struct sdlctx*)malloc(sizeof(struct sdlctx));
	ctx->win = SDL_CreateWindow("cbrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	ctx->ren = SDL_CreateRenderer(ctx->win, -1, 0);
	return ctx;
}

void render_handle_events(struct sdlctx* ctx)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			render_cleanup(ctx);
			break;
	}
}

void render_update(struct sdlctx* ctx)
{
	int a = 0;
}

void render_draw(struct sdlctx* ctx)
{
	SDL_SetRenderDrawColor(ctx->ren, 255, 255, 255, 255);
	SDL_RenderClear(ctx->ren);
	SDL_RenderPresent(ctx->ren);
}

void render_cleanup(struct sdlctx* ctx)
{
	SDL_DestroyWindow(ctx->win);
	SDL_DestroyRenderer(ctx->ren);
	SDL_Quit();
}
