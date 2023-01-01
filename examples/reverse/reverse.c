
/*****************************************************************

 Copyright 2001   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "wav.h"


void reverse (WAVE wave);
void swap_short_int (short int *pa, short int *pb);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;

   if (argc != 3)
   {
      printf ("USAGE: reverse <source file> <destination file>\n");
      exit (EXIT_FAILURE);
   }

   if ((fpin = fopen (argv[1], "rb")) == NULL)
   {
      printf ("errore di apertura del file di ingresso\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[2], "wb")) == NULL)
   {
      printf ("errore di apertura del file di uscita\n");
      exit (EXIT_FAILURE);
   }

   wave = ReadWave (fpin);

   reverse (wave);

   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


void reverse (WAVE wave)
{
   int i, j;

   i = 0;
   j = wave.numofstereosamples - 1;
   while (i < j)
   {
      swap_short_int (&SAMPLE(wave, RIGHT, i), &SAMPLE(wave, RIGHT, j));
      swap_short_int (&SAMPLE(wave, LEFT, i), &SAMPLE(wave, LEFT, j));
      i++;
      j--;
   }

   return;
}


void swap_short_int (short int *pa, short int *pb)
{
   short int tmp;

   tmp = *pa;
   *pa = *pb;
   *pb = tmp;

   return;
}
