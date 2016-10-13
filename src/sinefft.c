#include "brightyutil.h"
#include "brightyfft.h"
#include <fftw3.h>

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fputs("Please specify a input filename.\n", stderr);
    exit(1);
  }
  int inputSize = bc_fsize(argv[1]);
  if (inputSize < 0) return 1;
  FILE * inputFile;
  inputFile = bc_fopen(argv[1], "rb");

  int i;
  int fft_size = FFT_SIZE;
  uint32_t * fft_window;
  fft_window = (uint32_t *) malloc(fft_size * sizeof(uint32_t));
  double * scaled_fft_window;
  scaled_fft_window = (double *) malloc(fft_size * sizeof(double));
  fread(fft_window, sizeof(uint32_t), (size_t) fft_size, inputFile);
  for (i = 0; i < fft_size; i++) {
    scaled_fft_window[i] = (((double) fft_window[i]) / ((double) UINT32_MAX));
  }
  fftw_plan plan;
  fftw_complex * complexOut;
  complexOut = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * fft_size);

  plan = fftw_plan_dft_r2c_1d(fft_size, scaled_fft_window, complexOut, FFTW_ESTIMATE);

  fftw_execute(plan);


  uint32_t * mag_out;
  mag_out = (uint32_t *) malloc(fft_size * sizeof(uint32_t));
  bc_unpackfft(fft_size, complexOut, mag_out);

  double binfreq;
  double binfreq_increment = ((double) SAMPLE_RATE) / ((double) FFT_SIZE);

  binfreq = 0.0;

  for (i = 0; i < fft_size; i++) {
    printf("%2.2f, %d\n", binfreq, mag_out[i]);
    binfreq += binfreq_increment;
  }

  fclose(inputFile);
  fftw_destroy_plan(plan);
  fftw_free(complexOut);
  free(mag_out);
  free(fft_window);
  free(scaled_fft_window);
  return 0;
}
