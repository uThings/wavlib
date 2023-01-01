
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


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave1, wave2, *pwmix, *pwcurr, *pwtmp;
   int i, j, n;

   if (argc < 4)
   {                                                         
      printf ("USAGE: mixer <output filename> "
              "<input filename> <input filename> ...\n");
      printf ("       (at least 2 input waves, please)\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }


   /* open and load the first wave */
   if ((fpin = fopen (argv[2], "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   wave1 = ReadWave (fpin);
   fclose (fpin);

   pwmix = &wave1;
   pwcurr = &wave2;

   for (j = 3; j < argc; j++)
   {
      if ((fpin = fopen (argv[j], "rb")) == NULL)
      {
         printf ("Error opening input file\n");
         exit (EXIT_FAILURE);
      }

      /* read the wave */
      *pwcurr = ReadWave (fpin);
      fclose (fpin);

      /* let the wavemix be the longest one */
      if ((*pwcurr).numofstereosamples > (*pwmix).numofstereosamples)
      {
         pwtmp = pwmix;
         pwmix = pwcurr;
         pwcurr = pwtmp;
      }

      /* mix into *pwmix */
      for (i = 0; i < (*pwcurr).numofstereosamples; i++)
      {
         SAMPLE((*pwmix), RIGHT, i) =
               SAMPLE((*pwmix), RIGHT, i) + SAMPLE((*pwcurr), RIGHT, i);
         SAMPLE((*pwmix), LEFT, i) =
               SAMPLE((*pwmix), LEFT, i) + SAMPLE((*pwcurr), LEFT, i);
      }  

      /* release current waveform */
      ReleaseWaveData (pwcurr);
   }

   WriteWave ((*pwmix), fpout);
   ReleaseWaveData (pwmix);

   return EXIT_SUCCESS;
}

