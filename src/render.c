/*
Copyright (c) 2018-2020 Auxilus raghavsphadke@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "cbrain.h"

int last_max_level = 0;

struct sdlctx* render_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		exit(1);
	} else {
		struct sdlctx* ctx = (struct sdlctx*)malloc(sizeof(struct sdlctx));
		ctx->win = SDL_CreateWindow("cbrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
		ctx->ren = SDL_CreateRenderer(ctx->win, -1, 0);
		return ctx;
	}
}

void render_handle_events(struct sdlctx* ctx)
{
	SDL_PollEvent(&ctx->event);
	switch (ctx->event.type) {
		case SDL_QUIT:
			render_cleanup(ctx);
			break;
	}
}

void render_update(struct sdlctx* ctx, struct entityctx* ec, struct brain* b)
{
	int acc_right = 0;
	int acc_left = 0;
	for (int i = ec->mrstart; i < ec->mrend; i++) {
		acc_right += b->neurons[i]->thisstate;
		b->neurons[i]->thisstate *= 0.7;
	}

	for (int i = ec->mlstart; i < ec->mlend; i++) {
		int st = b->neurons[i]->thisstate;
		acc_left += st;
		b->neurons[i]->thisstate *= 0.7;
	}

	float speed = acc_right + acc_left;
	speed = speed / 150.0;

	ec->rot  += (acc_right > acc_left) ? -5.0 : (acc_left > acc_right) ? 5.0 : 0.0;
	ec->x += cosf(ec->rot / RADTODEG) * speed;
	ec->y += sinf(ec->rot / RADTODEG) * speed;
}

void render_draw(struct sdlctx* ctx, struct entityctx* ec, struct brain* b)
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
	render_draw_brain(ctx, ec, b);
	render_draw_activity_level(ctx, ec, b);
	SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 255);
	SDL_RenderDrawLine(ctx->ren, v4x, v4y, v1x, v1y);
	SDL_RenderDrawLine(ctx->ren, v1x, v1y, v2x, v2y);
	SDL_RenderDrawLine(ctx->ren, v2x, v2y, v3x, v3y);
	SDL_RenderDrawLine(ctx->ren, v3x, v3y, v4x, v4y);
	SDL_RenderPresent(ctx->ren);
}

void render_draw_brain(struct sdlctx* ctx, struct entityctx* ec, struct brain* b)
{
	for (int i = 0; i < b->nc; i++) {
		int x = i % 16;
		int y = i / 16.0;
		SDL_Rect rect;
		rect.x = x * 8;
		rect.y = y * 8;
		rect.w = 8;
		rect.h = 8;
		if (b->neurons[i]->fired) {
			if (b->neurons[i]->f_type == user) {
				SDL_SetRenderDrawColor(ctx->ren, 164, 36, 59, 255);
			} else {
				SDL_SetRenderDrawColor(ctx->ren, 19, 56, 71, 255);
			}
			if (SDL_RenderFillRect(ctx->ren, &rect) != 0) {
				printf("SDL_RenderDrawRect() for %d: %s", i, SDL_GetError());
			}
		}
		else {
			if (b->neurons[i]->thisstate == 0) {
				SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 255);
				if (SDL_RenderFillRect(ctx->ren, &rect) != 0) {
					printf("SDL_RenderDrawRect() for %d: %s", i, SDL_GetError());
				}
			}
			else {
				// int shade = 255 - (255 * b->neurons[i]->thisstate / THRESHOLD);
				// SDL_SetRenderDrawColor(ctx->ren, shade, shade, shade, 255);
				SDL_SetRenderDrawColor(ctx->ren, 150, 150, 150, 255);
				if (SDL_RenderDrawRect(ctx->ren, &rect) != 0) {
					printf("SDL_RenderDrawRect() for %d: %s", i, SDL_GetError());
				}
			}
		}
	}
	SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 255);
}

void render_draw_activity_level(struct sdlctx* ctx, struct entityctx* ec, struct brain* b)
{
	int activity_box_height = 16;
	SDL_Rect rect;
	rect.x = 0;
	rect.y = WIN_HEIGHT - activity_box_height;
	rect.w = WIN_WIDTH;
	rect.h = activity_box_height;
	SDL_SetRenderDrawColor(ctx->ren, 100, 100, 100, 255);
	if (SDL_RenderDrawRect(ctx->ren, &rect) != 0) {
		printf("SDL_RenderDrawRect() for activity level: %s", SDL_GetError());
	}

	int total_fired = 0;
	for (int i = 0; i < b->nc; i++) {
		if (b->neurons[i]->fired) {
			total_fired++;
		}
	}

	if (last_max_level < total_fired) {
		last_max_level = total_fired;
	}

	int wf = WIN_WIDTH * total_fired / b->nc;
	SDL_Rect acrect;
	acrect.x = 0;
	acrect.y = WIN_HEIGHT - activity_box_height;
	acrect.w = wf;
	acrect.h = activity_box_height;
	if (SDL_RenderFillRect(ctx->ren, &acrect) != 0) {
		printf("SDL_RenderDrawRect() for activity level: %s", SDL_GetError());
	}

	SDL_RenderDrawLine(ctx->ren, wf, acrect.y - 4, wf, WIN_HEIGHT);
	SDL_RenderDrawLine(ctx->ren, WIN_WIDTH * last_max_level / b->nc, WIN_HEIGHT - activity_box_height, WIN_WIDTH * last_max_level / b->nc, WIN_HEIGHT);

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
