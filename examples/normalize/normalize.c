
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
#include "wav.h"


void normalize (WAVE w, double percentage);
short int max_abs (short int a, short int b);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;
   double p;

#ifdef DEBUG

   if ((fpin = fopen ("testin.wav", "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen ("testout.wav", "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   p = 0.5;

#else

   if (argc != 4)
   {                                                         
      printf ("USAGE: normalize "
              "<source file> <destination file> <percentage>\n");
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

#endif

   /* read the wave */
   wave = ReadWave (fpin);
   fclose (fpin);
   p = atof (argv[3]);

   normalize (wave, p);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

short int max_abs (short int a, short int b)
{
   if (a < 0) a = -a;
   if (b < 0) b = -b;

   return (a > b) ? a : b;
}

void normalize (WAVE w, double percentage)
{
   long int i;
   short int s;
   short int max;
   double k;

   if (w.numofstereosamples == 0)
      return;

   max = max_abs (SAMPLE(w, RIGHT, 0), SAMPLE(w, LEFT, 0));

   for (i = 1; i < w.numofstereosamples; i++)
   {
      max = max_abs (max, SAMPLE(w, RIGHT, i));
      max = max_abs (max, SAMPLE(w, LEFT, i));
   }

   k = ((1 << (w.fmtheader.BitsPerSample - 1)) - 1) *
       (percentage / 100.0) / max;

   for (i = 0; i < w.numofstereosamples; i++)
   {
      SAMPLE(w, RIGHT, i) = k * SAMPLE(w, RIGHT, i);
      SAMPLE(w, LEFT, i) = k * SAMPLE(w, LEFT, i);
   }

   return;
}


