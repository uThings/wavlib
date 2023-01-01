
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


void stereoenhancer (WAVE wave, int depth);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;
   int depth;

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

#else

   if (argc != 4)
   {                                                         
      printf ("USAGE: stereoenhancer "
              "<source file> <destination file> <depth>\n");
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

   depth = atoi (argv[3]);

   /* read the wave */
   wave = ReadWave (fpin);
   fclose (fpin);

   stereoenhancer (wave, depth);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

 
void stereoenhancer (WAVE wave, int depth)
{
   double k;
   long int i;
   short int sl, sr;

   k = depth / 100.0;

   for (i = 0; i < wave.numofstereosamples; i++)
   {
      sl = SAMPLE(wave, LEFT, i);
      sr = SAMPLE(wave, RIGHT, i);

      SAMPLE(wave, RIGHT, i) -= k * sl;
      SAMPLE(wave, LEFT, i) -= k * sr;
   }
   return;
}


