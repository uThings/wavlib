
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

/* create a 5 seconds sine wave (200 Hz left and 500 Hz right).
   Amplitude: +/- 25000 */

#define LEN 5
#define AMP 25000


WAVE five_secs_sinwave (void);

int main (int argc, char *argv[])
{
   FILE *fpout;
   WAVE wave;

   if (argc != 2)
   {
      printf ("USAGE: waves <destination file>\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   wave = five_secs_sinwave();

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


WAVE five_secs_sinwave (void)
{
   WAVE wave;
   int i;

   wave = CreateEmptyCDaudioWave (LEN * SAMPLINGRATE);
   for (i = 0; i < wave.numofstereosamples; i++)
   {
      /* right channel: 500 Hz */
      SAMPLE(wave, RIGHT, i) = AMP * sin (i / 44100.0 * 2 * M_PI * 500);

      /* left channel: 200 Hz */
      SAMPLE(wave, LEFT, i) = AMP * sin (i / 44100.0 * 2 * M_PI * 200);
   }

   return wave;
}
