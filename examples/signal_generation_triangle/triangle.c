
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


WAVE triangle (double freq_Hz, double amp_percentage, double duty_cycle_percentage, double len_s);


int main (int argc, char *argv[])
{
   FILE *fpout;
   WAVE wave;

   if (argc != 6)
   {
      printf ("USAGE: sawtooth <out file> <freq[Hz]> <ampl[%%]> <duty-cycle[%%]> <len[s]> \n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   wave = triangle (atof (argv[2]), atof (argv[3]), atof (argv[4]), atof (argv[5]));

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

#define UP 1
#define DOWN 2

WAVE triangle (double freq_Hz, double amp_percentage, double duty_cycle_percentage, double len_s)
{
   WAVE wave;
   int i, maxampl, status, counter;
   double up_samples, down_samples, up_step, down_step, ampl;

   wave = CreateEmptyCDaudioWave (len_s * SAMPLINGRATE);

   up_samples = SAMPLINGRATE * (duty_cycle_percentage / 100.0) / freq_Hz;
   down_samples = SAMPLINGRATE * (1 - duty_cycle_percentage / 100.0) / freq_Hz;

   maxampl = ((1 << 15) - 1) / 100.0 * amp_percentage;
  
   up_step = 2 * maxampl / up_samples;
   down_step = 2 * maxampl / down_samples;

   ampl = 0;
   status = UP;
   counter = up_samples / 2.0;

   for (i = 0; i < wave.numofstereosamples; i++)
   {
      if (status == UP)
      {
         if (counter > up_samples)
         {
            status = DOWN;
            ampl = maxampl;
            SAMPLE(wave, RIGHT, i) = SAMPLE(wave, LEFT, i) = ampl;
            counter = 1;
         }
         else
         {
            ampl += up_step;
            SAMPLE(wave, RIGHT, i) = SAMPLE(wave, LEFT, i) = ampl;
            counter++;
         }
      }
      else /* status == DOWN */
      {
         if (counter > down_samples)
         {
            status = UP;
            ampl = -maxampl;
            SAMPLE(wave, RIGHT, i) = SAMPLE(wave, LEFT, i) = ampl;
            counter = 1;
         }
         else
         {
            ampl -= down_step;
            SAMPLE(wave, RIGHT, i) = SAMPLE(wave, LEFT, i) = ampl;
            counter++;
         }
      }
   }

   return wave;
}

