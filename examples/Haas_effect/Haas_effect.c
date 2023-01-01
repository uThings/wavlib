
/*****************************************************************

 Copyright 2013   PIER LUCA MONTESSORO

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


void Haas (WAVE w);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;
   double p;


   if (argc != 3)
   {                                                         
      printf ("USAGE: Haas <source file> <destination file>\n");
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

   Haas (wave);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


void Haas (WAVE w)
{
   long int i;
   short int s;
   int samples_delay;
   double gain;
   
   samples_delay = (int) (20.0 * 44100.0 / 1000.0);
   gain = pow (10, 6.0/20.0);
   
   for (i = 0; i < w.numofstereosamples - samples_delay; i++)
   {
      SAMPLE(w, RIGHT, i + samples_delay) = SAMPLE(w, LEFT, i);
      SAMPLE(w, LEFT, i) = (short int) (SAMPLE(w, LEFT, i) / gain);
   }

   return;
}


