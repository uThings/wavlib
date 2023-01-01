
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


void mono (WAVE wave);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;

   if (argc != 3)
   {                                                         
      printf ("USAGE: mono <source file> <destination file>\n");
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

   /* read the wave */
   wave = ReadWave (fpin);
   fclose (fpin);

   mono (wave);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}

 
void mono (WAVE wave)
{
   long int i;
   short int s;

   for (i = 0; i < wave.numofstereosamples; i++)
   {
      s = (SAMPLE(wave, RIGHT, i) + SAMPLE(wave, LEFT, i)) / 2.0;
      SAMPLE(wave, RIGHT, i) = SAMPLE(wave, LEFT, i) = s;
   }
   return;
}


