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

#ifndef CBRAIN_HEADER_H
#define CBRAIN_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

/* control constants */
#define DEBUG 0
#define THRESHOLD 20.0
#define MAX_WT_DIFF 5
#define SLEEP_T 0
#define MUTATE_PROB 0.00001
#define WEIGHT_MIN 1
#define WEIGHT_MAX 20
#define WIN_WIDTH 800
#define WIN_HEIGHT 400

#define RADTODEG 57.2957

typedef unsigned int uint;
typedef enum { undefined, sensory, intermediate, motor } type;
typedef enum { user, self } fire_type;

pthread_mutex_t lock;

struct neuron {
	int id;
	int *links;
	int *wts;
	uint lc;
	uint lmax;
	type n_type;
	fire_type f_type;

	float thisstate;
	float nextstate;
	float state_decay;
	uint fired;
	uint n_fired;
} neuron;

struct brain {
	uint nc;
	uint nmax;
	float state_decay;
	float fitness;
	struct neuron** neurons;
} brain;

struct nthread {
	pthread_t tid;
	uint s;
	uint e;
	uint status;
} nthread;

struct thread_bank {
	uint tc;
	uint tmax;

	struct nthread** threads;
} thread_bank;

struct sdlctx {
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Event event;
} sdlctx;

struct vect {
	float x;
	float y;
} vect;

struct entityctx {
	int mlstart;
	int mrstart;
	int mlend;
	int mrend;
	int width;
	int height;
	float rot;
	float x;
	float y;
} entityctx;

/*	src/brain.c	*/
struct brain* brain_init(int, float);
void brain_reset(struct brain*);
void brain_neuron_type(struct brain*, type);
struct neuron* neuron_init(uint, float);
void neuron_link(struct neuron*, struct neuron*, int);
void neuron_link_random(struct brain*);
void neuron_unlink(struct neuron*, struct neuron*);
void neuron_accum(struct neuron*, uint);
void neuron_fire(struct neuron*);
int  neuron_update(struct neuron*, struct brain*);
int  neuron_update_range(uint, uint, struct brain*);
void neuron_set_type(struct neuron*, type);
void neuron_add(struct brain*);
void show_stat(struct neuron*);

/*	src/tem.c  */
void tem_m(struct brain*);

/*	src/thread.c	*/
struct thread_bank* thread_bank_new(uint);
struct nthread* thread_struct_new(uint, uint);
int thread_create(struct nthread*, struct brain*, uint, uint);
void* thread_func(void*);

/*	src/util.c	*/
int checkexist(int, int*, int);
char* get_type_name(type);
int rand_int(int, int);
float rand_float(float, float);
void save_brain(struct brain*, char*);
struct brain* gen_brain(char*);
int file_num_lines(char*);
int cbrain_print(int, const char*, ...)__attribute__((format(printf, 2, 3)));
struct brain* parse_model_csv(char*);

/*	src/render.c	*/
struct sdlctx* render_init(void);
void render_handle_events(struct sdlctx*);
void render_update(struct sdlctx*, struct entityctx*, struct brain*);
void render_draw(struct sdlctx*, struct entityctx*, struct brain*);
void render_draw_activity_level(struct sdlctx*, struct entityctx*, struct brain*);
void render_draw_brain(struct sdlctx*, struct entityctx*, struct brain*);
void render_cleanup(struct sdlctx*);
struct entityctx* render_spawn(int, int, int, int);
#endif
