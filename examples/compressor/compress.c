
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

/*
   dynamic range compression:
   multiply each sample s by (1 - k * |s| / 32767)
   where 0 < k <= 0.5
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


#define MAXSAMPLEVALUE  ((1 << (BITSPERSAMPLE - 1)) - 1)

void compress (WAVE w, double level);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;
   double k;

   if (argc != 4)
   {                                                         
      printf ("USAGE: compress "
              "<source file> <destination file> "
              "<compression level (0 < k <= 0.5)>\n");
      exit (EXIT_FAILURE);
   }

   k = atof (argv[3]);
   if (!(k > 0) || (k > 0.5))
   {
      printf ("Out of range input value (0 < k <= 0.5)\n");
      exit (EXIT_FAILURE);
   }

   if ((fpin = fopen (argv[1], "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[2], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   /* read the wave */
   wave = ReadWave (fpin);
   fclose (fpin);

   compress (wave, k);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

void compress (WAVE w, double level)
{
   long int i;
   short int s;

   for (i = 0; i < w.numofstereosamples; i++)
   {
      s = SAMPLE(w, RIGHT, i);
      SAMPLE(w, RIGHT, i) = 
         s * (1 - level * abs(s) / (double) MAXSAMPLEVALUE);

      s = SAMPLE(w, LEFT, i);
      SAMPLE(w, LEFT, i) =
         s * (1 - level * abs(s) / (double) MAXSAMPLEVALUE);
   }

   return;
}


