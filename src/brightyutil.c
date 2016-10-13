#include "brightyutil.h"
#include <limits.h>

int bc_fsize(char * filename) {
  struct stat ststat;
  int streturn = stat(filename, &ststat);
  if (streturn == -1) {
    fputs("Error in stat call!\n", stderr);
    return streturn;
  } else if (streturn == ENOENT) {
    fputs("Filename does not exist!\n", stderr);
    return streturn;
  }
  streturn = (int) ststat.st_size;
  return streturn;
}

FILE * bc_fopen(char * filename, char * mode) {
  FILE * inputFile = fopen(filename, mode);
  if (inputFile == NULL) {
    fputs("Error opening input file!\n", stderr);
    exit(2);
  }
  return inputFile;
}

void bc_unpackfft(int fft_size, fftw_complex * fft_output, unsigned int * magnitudes) {
  int i;
  for (i = 0; i < fft_size; i++) {
    magnitudes[i] = sqrt(pow(fft_output[i][0], 2) + pow(fft_output[i][1], 2));
  }
}

