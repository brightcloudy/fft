#!/bin/bash
cd src
gcc sinefft.c brightyutil.c -Wall -lfftw3 -lm -O3 -o sinefft
