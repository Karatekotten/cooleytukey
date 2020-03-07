#include "cmplx.h"

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