
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


WAVE sawtooth (int Lfreq_Hz, int Lamp_percentage, int Rfreq_Hz, int Rampl_percentage, int len_s);


int main (int argc, char *argv[])
{
   FILE *fpout;
   WAVE wave;

   if (argc != 7)
   {
      printf ("USAGE: sawtooth <out file> <Lfreq[Hz]> <Lampl[%%]> <Rfreq[Hz]> <Rampl[%%]> <len[s]> \n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   wave = sawtooth (atoi (argv[2]), atoi (argv[3]), atoi (argv[4]), atoi (argv[5]), atoi (argv[6]));

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


WAVE sawtooth (int Lfreq_Hz, int Lamp_percentage, int Rfreq_Hz, int Rampl_percentage, int len_s)
{
   WAVE wave;
   int i, Lcounter, Rcounter, Lmaxampl, Rmaxampl, Lampl, Rampl;
   double Lcyclesamples, Lstep, Rcyclesamples, Rstep;

   wave = CreateEmptyCDaudioWave (len_s * SAMPLINGRATE);

   Lcyclesamples = 1.0 / Lfreq_Hz * SAMPLINGRATE;
   Rcyclesamples = 1.0 / Rfreq_Hz * SAMPLINGRATE;

   Lmaxampl = ((1 << 15) - 1) / 100.0 * Lamp_percentage;
   Rmaxampl = ((1 << 15) - 1) / 100.0 * Rampl_percentage;
  
   Lstep = 2 * Lmaxampl / Lcyclesamples;
   Rstep = 2 * Rmaxampl / Rcyclesamples;


   Lampl = -Lmaxampl;
   Rampl = -Rmaxampl;

   Lcounter = Rcounter = 1;

   for (i = 0; i < wave.numofstereosamples; i++)
   {
      /* left channel */
      if (i > Lcounter * Lcyclesamples)
      {
         Lampl = -Lmaxampl;
         SAMPLE(wave, LEFT, i) = Lampl;
         Lcounter++;
      }
      else
      {
         Lampl += Lstep;
         SAMPLE(wave, LEFT, i) = Lampl;
      }

      /* right channel */
      if (i > Rcounter * Rcyclesamples)
      {
         Rampl = -Rmaxampl;
         SAMPLE(wave, RIGHT, i) = Rampl;
         Rcounter++;
      }
      else
      {
         Rampl += Rstep;
         SAMPLE(wave, RIGHT, i) = Rampl;
      }
   }

   return wave;
}

