#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#include "cmplx.h"

typedef struct
{
	float (*x)[CMPLX];
	int m, n, t;
} transform_args;

void *transform(void *argp);
float (*transform_fourier(float x[][CMPLX], int m, int n, int h, int i))[CMPLX];
float (*transform_separate(float x[][CMPLX], int m, int n, int h))[CMPLX];
float *transform_normalize(float x[][CMPLX], float y[], int n, int i);

void transform_test_generate(float x[][CMPLX], int n, int m, int i);
void transform_test_input(float x[][CMPLX], int n, int i, float f, float a, float o, bool reset);
void transform_test_display(float y[], int n, float threshold, int i);