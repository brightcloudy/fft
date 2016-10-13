#include "brightyutil.h"
#include "brightyfft.h"
#include <fftw3.h>
#include <wand/MagickWand.h>

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
  MagickWandGenesis();
  MagickWand * mw;
  mw = NewMagickWand();
  MagickWandTerminus();
  
  // window for audio samples from file raw
  uint32_t * fft_window;
  fft_window = (uint32_t *) malloc(FFT_SIZE * sizeof(uint32_t));
  // scaled for fft maths to be within -1.0, 1.0
  double * scaled_fft_window;
  scaled_fft_window = (double *) malloc(FFT_SIZE * sizeof(double));
  // need malloc for complex output of DFT
  fftw_complex * complexOut;
  complexOut = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
  // and also for plain jane magnitudes
  unsigned int * mag_out;
  mag_out = (unsigned int *) malloc(FFT_SIZE * sizeof(unsigned int));

  // fftw plan is made once and executed many
  fftw_plan plan;
  plan = fftw_plan_dft_r2c_1d(FFT_SIZE, scaled_fft_window, complexOut, FFTW_ESTIMATE);

  int frame_count;
  frame_count = ((inputSize / sizeof(uint32_t)) / FFT_SIZE);

  int frame_i;

  for (frame_i = 0; frame_i < frame_count; frame_i++) {
    fread(fft_window, sizeof(uint32_t), (size_t) FFT_SIZE, inputFile);

    for (i = 0; i < FFT_SIZE; i++) {
      scaled_fft_window[i] = (((double) fft_window[i]) / ((double) UINT32_MAX));
    }

    fftw_execute(plan);

    bc_unpackfft(FFT_SIZE, complexOut, mag_out);

    double binfreq;
    double binfreq_increment = ((double) FMAX) / ((double) FFT_NUMBINS);

    binfreq = 0.0;

    double loudest_bin;
    unsigned int max_mag_seen = 0;

    for (i = 1; i < FFT_SIZE; i++) {
      if (mag_out[i] >= max_mag_seen) {
        loudest_bin = binfreq;
        max_mag_seen = mag_out[i];
      }
      binfreq += binfreq_increment;
    }
    printf("%f is loudest\n", loudest_bin);
  }
  fclose(inputFile);
  fftw_destroy_plan(plan);
  fftw_free(complexOut);
  free(mag_out);
  free(fft_window);
  free(scaled_fft_window);
  return 0;
}
