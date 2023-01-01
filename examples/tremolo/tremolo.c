
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
#include <math.h>
#include "wav.h"


void tremolo (WAVE w, double d, double f, double phi);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE sound;

   if (argc != 6)
   {
      printf ("usage: temolo_wav <input file> <output file> <depth> <frequency> <phase difference>\n");
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
   tremolo (sound, atof (argv[3]), atof (argv[4]), atof (argv[5]));
   WriteWave (sound, fpout);

   ReleaseWaveData (&sound);

   fclose (fpin);
   fclose (fpout);

   return EXIT_SUCCESS;
}



void tremolo (WAVE w, double p, double f, double phi)
{
   int i;
   double kL, kR;

   if (p < 0 || p > 100)
   {
      printf ("invalid depth %f\n", p);
      return;
   }

  for (i = 0; i < w.numofstereosamples; i++)
  {
     /* the modulation coefficient must fall between 1 and 0 */

     kL = 1 - p / 100.0 * (1 + sin (2 * M_PI * f * i / 44100.0)) / 2.0;
     kR = 1 - p / 100.0 * (1 + sin (2 * M_PI * f * i / 44100.0 + phi / 180.0 * M_PI)) / 2.0;

     SAMPLE (w, LEFT, i) = SAMPLE (w, LEFT, i) * kL;
     SAMPLE (w, RIGHT, i) = SAMPLE (w, RIGHT, i) * kR;
  }

   return;
}







