#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "transform.h"

#define N 16384

int main()
{
	float x[N][CMPLX], y[N/2];

	transform_test_generate(x, N, 3, 0);

	int threads = 8;

	clock_t t0, t;

	t0 = clock();

	transform_args args = {x, 0, N, floor(log2(threads))};

	transform(&args);

	t = clock();

	printf("\ntime: %f s\n", (float)(t-t0)/CLOCKS_PER_SEC);
	
	transform_normalize(x, y, N/2, 0);

	transform_test_display(y, N/2, 0.1, 0);

	return 0;
}