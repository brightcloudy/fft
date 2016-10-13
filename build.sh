#!/bin/bash
cd src
gcc sinefft.c brightyutil.c -I/usr/include/ImageMagick-6/ -Wall -lfftw3 -lm `pkg-config --cflags --libs MagickWand` -O3 -o sinefft
