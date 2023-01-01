
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


void fadeout (WAVE wave, int n);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;

   if (argc != 4)
   {
      printf ("USAGE: waves <source file> <destination file> <seconds>\n");
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

   /* call the fading out function */
   fadeout (wave, atoi(argv[3]));

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


void fadeout (WAVE wave, int n)
{
   long int i, fadeoutbegin, samplestofade;
   double volume;

   samplestofade = n * wave.fmtheader.SampleRate;

   if (samplestofade > wave.numofstereosamples)
   {
      printf ("Fade out cannot be longer than wave\n");
      exit (EXIT_FAILURE);
   }

   fadeoutbegin = wave.numofstereosamples - samplestofade;

   for (i = fadeoutbegin; i < wave.numofstereosamples; i++)
   {
      volume = (double) (wave.numofstereosamples - i) /
               (double) samplestofade;
      SAMPLE(wave, RIGHT, i) = (double) SAMPLE(wave, RIGHT, i) * volume;
      SAMPLE(wave, LEFT, i) = (double) SAMPLE(wave, LEFT, i) * volume;
   }

   return;
}
