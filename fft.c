#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define N 16384

#define CMPLX 2
#define R 0
#define I 1

float *cmplx_set(float c[CMPLX], float real, float imaginary);
float (*cmplx_copy(float x[][CMPLX], float z[][CMPLX], int m, int n, int interval, int i))[CMPLX];
float (*cmplx_paste(float x[][CMPLX], float z[][CMPLX], int m, int n, int i))[CMPLX];

typedef struct
{
	float (*x)[CMPLX];

	int m, n, t;
} transform_args;

transform_args *transform_arguments_set(transform_args *args, float x[][CMPLX], int m, int n, int t);
void *transform(void *argp);
float (*transform_fourier(float x[][CMPLX], int m, int n, int h, int i))[CMPLX];
float (*transform_separate(float x[][CMPLX], int m, int n, int h))[CMPLX];
float *transform_normalize(float x[][CMPLX], float y[], int n, int i);

/*-------------------------------test functions-------------------------------*/
void transform_generate(float x[][CMPLX], int n, int m, int i);
void transform_input(float x[][CMPLX], int n, int i, float f, float a, float o, bool reset);
void transform_display(float y[], int n, float threshold, int i);
/*----------------------------------------------------------------------------*/

int main()
{
	float x[N][CMPLX], y[N/2];

	transform_generate(x, N, 3, 0);

	clock_t t0, t;

	t0 = clock();

	transform_args args;

	transform_arguments_set(&args, x, 0, N, 3);

	transform(&args);

	t = clock();

	printf("\nt: %f s\n", (float)(t - t0)/CLOCKS_PER_SEC);
	
	transform_normalize(x, y, N/2, 0);

	transform_display(y, N/2, 0.1, 0);
}

float *cmplx_set(float cmplx[CMPLX], float real, float imaginary)
{
	cmplx[R] = real;
	cmplx[I] = imaginary;

	return cmplx;
}

float (*cmplx_copy(float x[][CMPLX], float z[][CMPLX], int m, int n, int interval, int i))[CMPLX]
{
	int j = m+i*interval;

	cmplx_set(z[i], x[j][R], x[j][I]);

	if (j+interval < n) cmplx_copy(x, z, m, n, interval, i+1);

	return z;
}

float (*cmplx_paste(float x[][CMPLX], float z[][CMPLX], int m, int n, int i))[CMPLX]
{
	cmplx_set(x[m+i], z[i][R], z[i][I]);

	if (m+i+1 < n) cmplx_paste(x, z, m, n, i+1);

	return x;
}

transform_args *transform_arguments_set(transform_args *args, float x[][CMPLX], int m, int n, int t)
{
	args->x = x;
	args->m = m;
	args->n = n;
	args->t = t;

	return args;
}

void *transform(void *argp)
{
	transform_args *args = argp;

	int h = (args->n-args->m)/2;

	if (args->n-args->m >= 2)
	{
		transform_separate(args->x, args->m, args->n, h);

		transform_args argsEven, argsOdd;

		transform_arguments_set(&argsEven, args->x, args->m, args->m+h, args->t-1);
		transform_arguments_set(&argsOdd, args->x, args->m+h, args->n, args->t-1);

		if (args->t > 0)
		{
			pthread_t t1, t2;

			pthread_create(&t1, NULL, transform, &argsEven);
			pthread_create(&t2, NULL, transform, &argsOdd);

			pthread_join(t1, NULL);
			pthread_join(t2, NULL);
		}
		else
		{
			transform(&argsEven);
			transform(&argsOdd);
		}

		transform_fourier(args->x, args->m, args->n, h, 0);
	}

	return NULL;
}

float (*transform_fourier(float x[][CMPLX], int m, int n, int h, int i))[CMPLX]
{
	int even = m+i, odd = m+i+h;

	float v[CMPLX], w[CMPLX];

	cmplx_set(v, cos(-2*M_PI*i/(n-m)), sin(-2*M_PI*i/(n-m)));
	cmplx_set(w, v[R]*x[odd][R]-v[I]*x[odd][I], v[I]*x[odd][R]+v[R]*x[odd][I]);

	cmplx_set(x[odd], x[even][R]-w[R], x[even][I]-w[I]);
	cmplx_set(x[even], x[even][R]+w[R], x[even][I]+w[I]);

	if (i+1 < h) transform_fourier(x, m, n, h, i+1);

	return x;
}

float (*transform_separate(float x[][CMPLX], int m, int n, int h))[CMPLX]
{
	float even[h][CMPLX], odd[h][CMPLX];

	cmplx_copy(x, even, m, n, 2, 0);
	cmplx_copy(x, odd, m+1, n, 2, 0);
	
	cmplx_paste(x, even, m, m+h, 0);
	cmplx_paste(x, odd, m+h, n, 0);

	return x;
}

float *transform_normalize(float x[][CMPLX], float y[], int n, int i)
{
	y[i] = sqrt(pow(x[i][R], 2)+pow(x[i][I], 2))/(N/2);

	if (i+1 < n) transform_normalize(x, y, n, i+1);

	return y;
}

/*-------------------------------test functions-------------------------------*/
void transform_generate(float x[][CMPLX], int n, int m, int i)
{
	float minF = 1, maxF = n/2-1, minA = 1, maxA = 4, noise = 0;
	float frequency, amplitude, offset;

	if (i == 0)
	{
		srand(time(0));

		transform_input(x, n, 0, 0, noise, 0, true);
	}

	frequency = minF+(float)(rand()%(int)(maxF-minF));
	amplitude = minA+((float)rand()/(float)(RAND_MAX))*(maxA-minA);
	offset = ((float)rand()/(float)(RAND_MAX))*(2*M_PI);

	printf("wave %d: frequency=%f, amplitude=%f, offset=%f\n", i+1, frequency, amplitude, offset);

	transform_input(x, n, 0, frequency, amplitude, offset, false);

	if (i+1 < m) transform_generate(x, n, m, i+1);
}

void transform_input(float x[][CMPLX], int n, int i, float f, float a, float o, bool reset)
{
	if (reset == false)
	{
		x[i][R] += a*sin(o+f*2*M_PI*i/n);
	}
	else cmplx_set(x[i], a*(1-2*(float)rand()/((float)RAND_MAX)), 0);

	if (i+1 < n) transform_input(x, n, i+1, f, a, o, reset);
}

void transform_display(float y[], int n, float threshold, int i)
{
	if (y[i] > threshold) printf("y[%d]: %f\n", i, y[i]);

	if (i+1 < n) transform_display(y, n, threshold, i+1);
}
/*----------------------------------------------------------------------------*/