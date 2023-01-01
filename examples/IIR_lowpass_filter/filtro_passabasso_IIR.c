
/*****************************************************************

 Copyright 2006   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/

/* IIR low-pass filter:  y(n) = (1-k)*x(n) + k*y(n)  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


void IIR_low_pass (WAVE win, WAVE wout, double k);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE win, wout;

   if (argc != 4)
   {
      printf ("USAGE: IIRlp <source file> <destination file> <decay>\n");
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

   win = ReadWave (fpin);
   fclose (fpin);

   wout = CreateEmptyCDaudioWave (win.numofstereosamples);

   IIR_low_pass (win, wout, atof(argv[3]));

   WriteWave (wout, fpout);
   fclose (fpout);

   ReleaseWaveData (&win);
   ReleaseWaveData (&wout);

   return EXIT_SUCCESS;
}


void IIR_low_pass (WAVE win, WAVE wout, double k)
{
   long int i;

   SAMPLE(wout, RIGHT, 0) = SAMPLE(win, RIGHT, 0) * (1-k);
   SAMPLE(wout, LEFT, 0) = SAMPLE(win, LEFT, 0) * (1-k);

   for (i = 1; i < wout.numofstereosamples; i++)
   {
      SAMPLE(wout, RIGHT, i) = SAMPLE(win, RIGHT, i) * (1-k)
                               + SAMPLE(wout, RIGHT, i-1) * k;
      SAMPLE(wout, LEFT, i) = SAMPLE(win, LEFT, i) * (1-k)
                               + SAMPLE(wout, LEFT, i-1) * k;
   }

   return;
}
