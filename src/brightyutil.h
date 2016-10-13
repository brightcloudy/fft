#ifndef __BRIGHTYUTIL__
#define __BRIGHTYUTIL__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <fftw3.h>

int bc_fsize(char * filename);
FILE * bc_fopen(char * filename, char * mode);
void bc_unpackfft(int fft_size, fftw_complex * fft_output, unsigned int * magnitudes);
#endif
