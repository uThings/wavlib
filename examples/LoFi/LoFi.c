
/*****************************************************************

 Copyright 2015   PIER LUCA MONTESSORO

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
#include <math.h>
#include "wav.h"


void LoFi (WAVE w, int quantization_reduction_factor, int frequency_reduction_factor);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE sound;

   if (argc != 5)
   {
      printf ("usage: LoFi <input file> <output file> <quantization reduction factor> <frequency reduction_factor)>\n");
      exit (EXIT_FAILURE);
   }

   if ((fpin = fopen (argv[1], "rb")) == NULL)
   {
      printf ("errore di apertura del file %s\n", argv[1]);
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[2], "wb")) == NULL)
   {
      printf ("errore di apertura del file %s\n", argv[2]);
      exit (EXIT_FAILURE);
   }

   sound = ReadWave (fpin);
   LoFi (sound, atoi (argv[3]), atoi (argv[4]));
   WriteWave (sound, fpout);

   ReleaseWaveData (&sound);

   fclose (fpin);
   fclose (fpout);

   return EXIT_SUCCESS;
}



void LoFi (WAVE w, int quantization_reduction_factor, int frequency_reduction_factor)
{
   int i, j, sumL, sumR, outL, outR, sample_counter;
   double averageL, averageR;

   if (quantization_reduction_factor < 1 || quantization_reduction_factor > (1 << 15))
   {
      printf ("invalid quantization reduction factor\n");
      exit (EXIT_FAILURE);
   }

   if (frequency_reduction_factor != 1 && frequency_reduction_factor != 2 && 
       frequency_reduction_factor != 4 && frequency_reduction_factor != 8 && 
       frequency_reduction_factor != 16)
   {
      printf ("invalid frequency reduction factor\n");
      exit (EXIT_FAILURE);
   }

  for (i = 0; i < w.numofstereosamples; i += frequency_reduction_factor)
  {
      sumL = sumR = 0;
      for (sample_counter = 0, j = i;
           sample_counter < frequency_reduction_factor && j < w.numofstereosamples;
           sample_counter++, j++)
      {
         sumL += SAMPLE (w, LEFT, j);
         sumR += SAMPLE (w, RIGHT, j);
      }

      averageL = sumL / (double) sample_counter;
      averageR = sumR / (double) sample_counter;
      outL = quantization_reduction_factor * (int) (averageL / (double) quantization_reduction_factor);
      outR = quantization_reduction_factor * (int) (averageR / (double) quantization_reduction_factor);

      for (sample_counter = 0, j = i;
           sample_counter < frequency_reduction_factor && j < w.numofstereosamples;
           sample_counter++, j++)
      {
         SAMPLE (w, LEFT, j) = outL;
         SAMPLE (w, RIGHT, j) = outR;
      }
  }

   return;
}

