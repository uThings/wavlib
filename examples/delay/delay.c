
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


WAVE delay (WAVE wave, int delay_time_ms, int decay_percentage);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave, waved;

   if (argc != 5)
   {
      printf ("USAGE: delay <source file> <destination file> "
              "<delay time (ms)> <decay %%>\n");
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

   waved = delay (wave, atoi (argv[3]), atoi (argv[4]));

   WriteWave (waved, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);
   ReleaseWaveData (&waved);

   return EXIT_SUCCESS;
}


WAVE delay (WAVE wave, int delay_time_ms, int decay_percentage)
{
   WAVE waved;
   int delay_samples;
   int i, tmp, clipcounter;
   double decay;

   delay_samples = SAMPLINGRATE * delay_time_ms / 1000.0;
   decay = decay_percentage / 100.0;

   waved = CreateEmptyCDaudioWave (wave.numofstereosamples + delay_samples);

   /* copy the original wave */
   for (i = 0; i < wave.numofstereosamples; i++)
   {
      SAMPLE (waved, RIGHT, i) = SAMPLE (wave, RIGHT, i);
      SAMPLE (waved, LEFT, i) = SAMPLE (wave, LEFT, i);
   }

   /* silent the tail */
   for ( ; i < wave.numofstereosamples + delay_samples; i++)
   {
      SAMPLE (waved, RIGHT, i) = 0;
      SAMPLE (waved, LEFT, i) = 0;
   }

   /* add the delay */
   clipcounter = 0;
   for (i = delay_samples; i < wave.numofstereosamples + delay_samples; i++)
   {
      tmp = SAMPLE (waved, RIGHT, i) +
            SAMPLE (wave, RIGHT, i - delay_samples) * decay;
      if (tmp < SHRT_MIN)
      {
         clipcounter++;
         tmp = SHRT_MIN;
      }
      else if (tmp > SHRT_MAX)
      {
         clipcounter++;
         tmp = SHRT_MAX;
      }
      SAMPLE (waved, RIGHT, i) = tmp;

      tmp = SAMPLE (waved, LEFT, i) +
            SAMPLE (wave, LEFT, i - delay_samples) * decay;
      if (tmp < SHRT_MIN)
      {
         clipcounter++;
         tmp = SHRT_MIN;
      }
      else if (tmp > SHRT_MAX)
      {
         clipcounter++;
         tmp = SHRT_MAX;
      }
      SAMPLE (waved, LEFT, i) = tmp;
   }

   if (clipcounter > 0)
      printf ("Warning: %d samples clipped\n", clipcounter);

   return waved;
}
