#!/bin/sh
gcc -o$1 -DWAVSHOWALL $1.c wav.c -lm

