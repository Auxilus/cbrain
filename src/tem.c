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

void tem_t(struct brain* b, int* motor, int* sensory, int* inputs, int* outputs)
{

}

void tem_m(struct brain* b)
{
	for (int i = 0; i < b->nc; i++) {
		float random = rand_float(0, 1);
		if (random < MUTATE_PROB) {
			int l = rand_int(0, b->neurons[i]->lc - 1);
			int original_wt = b->neurons[i]->wts[l];
			int new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			while (new_wt == original_wt) {
				cbrain_print(2, "mutated weight is same as original weight, generating new weight\n");
				new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			}
			while (abs(new_wt - original_wt) > MAX_WT_DIFF) {
				cbrain_print(2, "mutated weight [%d] exceeds MAX_WT_DIFF [%d], generating new weight\n", new_wt, MAX_WT_DIFF);
				new_wt = rand_int(WEIGHT_MIN, WEIGHT_MAX);
			}
			b->neurons[i]->wts[l] = new_wt;
			cbrain_print(0, "mutated %d:%d from weight %d to %d\n", i, b->neurons[i]->links[l], original_wt, new_wt);
		}
	}
}