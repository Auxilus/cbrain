#include "cbrain.h"

struct sdlctx* sdl_init()
{
	struct sdlctx* ctx = (struct sdlctx*)malloc(sizeof(struct sdlctx));
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return NULL;
	}
	ctx->window = SDL_CreateWindow("cbrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);
	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);
	ctx->surface = SDL_GetWindowSurface(ctx->window);
	SDL_FillRect(ctx->surface, NULL, SDL_MapRGB(ctx->surface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(ctx->window);
	SDL_Delay(2000);
	return ctx;
}
