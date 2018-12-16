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

typedef unsigned int   uint;

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
	
	uint nmax;
} brain;

struct neuron* make_neuron(uint);
struct brain* init_brain(int);
void link_neuron(struct neuron*, struct neuron*, uint);
void accum_neuron(struct neuron*, uint);
void fire_neuron(struct neuron*, struct brain*);
void update_neuron(struct neuron*, struct brain*);
void update_all(struct brain*);

int rand_int(int, int);
int checkexist(uint, uint*, int);
void show_stat(struct neuron*);
