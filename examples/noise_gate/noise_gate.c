
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
   Noise gate
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


#define MAX_SHORT_VAL 0x7FFF

void noise_gate (WAVE w, double min_duration_ms, double noise_threshold_dB);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;

   if (argc != 5)
   {                                                         
      printf ("USAGE: noise_gate "
              "<source file> <destination file>"
              "<min noise duration (ms)> <noise threshold (dB)>\n");
      exit (EXIT_FAILURE);
   }

   if (atof(argv[4]) > 0)
   {
      printf ("Noise threshold (dB) must be a negative value\n");
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

   noise_gate (wave, atof(argv[3]), atof(argv[4]));

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

void noise_gate (WAVE w, double min_duration_ms, double noise_threshold_dB)
{
   long int i, j, start_noise_index;
   int min_duration, noise_threshold;
   int inside_noise;

   min_duration = 44100 * min_duration_ms / 1000.0;
   noise_threshold = ((1 << 15) - 1) * pow (10, (noise_threshold_dB / 20));

   inside_noise = 0;

   for (i = 0; i < w.numofstereosamples; i++)
   {
      if (abs (SAMPLE (w, LEFT, i)) < noise_threshold && 
          abs (SAMPLE (w, RIGHT, i)) < noise_threshold)
      {
         if (!inside_noise) 
         {
            start_noise_index = i;
            inside_noise = 1;
         }
      }
      else
      {
         if (inside_noise)
         {
            if (i - start_noise_index >= min_duration)
            {
               printf ("noise found between samples %d and %d\n", start_noise_index, i);
 
               for (j = start_noise_index; j < i; j++)
                   SAMPLE (w, LEFT, j) = SAMPLE (w, RIGHT, j) = 0;
            }

            inside_noise = 0;
         }
      }
   }

   /* tail...*/
   if (inside_noise)
   {
      if (i - start_noise_index >= min_duration)
      {
         printf ("noise found between samples %d and %d\n", start_noise_index, i);

         for (j = start_noise_index; j < i; j++)
             SAMPLE (w, LEFT, j) = SAMPLE (w, RIGHT, j) = 0;
      }
   }

   return;
}

