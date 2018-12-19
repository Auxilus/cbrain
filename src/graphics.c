/*
Copyright (c) 2018 Auxilus raghavsphadke@gmail.com

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

#include "header.h"

struct ngraph* graphics_init(void)
{
	struct ngraph* ng = (struct ngraph*)malloc(sizeof(struct ngraph));
	SDL_Init(SDL_INIT_VIDEO);
	printf("creating window\n");
	SDL_Window* window = SDL_CreateWindow(
		"An SDL2 window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		printf("failed to create window\n");
		exit(1);
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		printf("failed to create renderer\n");
		exit(1);
	}
	ng->window = window;
	ng->renderer = renderer;
	return ng;
}

void graphics_quit(struct ngraph* ng)
{
	SDL_DestroyRenderer(ng->renderer);
	SDL_DestroyWindow(ng->window);
	SDL_Quit();
}

void* graphics_event_monitor(void* ng)
{
	SDL_Event e;
	int quit = 0;
	while (!quit) {
		while (SDL_PollEvent(&e)) { 
			switch (e.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			default: {}
			}
		}
		SDL_SetRenderDrawColor(((struct ngraph*)ng)->renderer, 225, 225, 225, 255);
		SDL_RenderClear(((struct ngraph*)ng)->renderer);
		SDL_RenderPresent(((struct ngraph*)ng)->renderer);
	}
	graphics_quit((struct ngraph*)ng);
}
