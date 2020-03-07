#define CMPLX 2
#define R 0
#define I 1

float *cmplx_set(float c[CMPLX], float real, float imaginary);
float (*cmplx_copy(float x[][CMPLX], float z[][CMPLX], int m, int n, int interval, int i))[CMPLX];
float (*cmplx_paste(float x[][CMPLX], float z[][CMPLX], int m, int n, int i))[CMPLX];