#!/bin/bash
sox -n -t raw -r 48000 -b 32 -e unsigned-integer khzraw synth 30 sine 12345.6789
