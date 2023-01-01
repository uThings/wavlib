
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
   Filtro FIR. Esempio di applicazione: simulazione di riverbero
   mediante convoluzione con la risposta all'impulso registrata 
   nell'ambiente da simulare
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"



WAVE convolution (WAVE win, WAVE wir);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpir, *fpout;
   WAVE win, wout, wir;
   char message[1024];

   if (argc != 4)
   {                                                         
      printf ("USO: convoluzione_risposta_impulso "
              "<file in> <file out> <file risposta all'impulso>\n");
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

   if ((fpir = fopen (argv[3], "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   /* read the input wave */
   win = ReadWave (fpin);
   fclose (fpin);

   /* read the impulse response */
   wir = ReadWave (fpir);
   fclose (fpir);

   wout = convolution (win, wir);

   /* write the file */
   WriteWave (wout, fpout);
   fclose (fpout);

   ReleaseWaveData (&win);
   ReleaseWaveData (&wout);
   ReleaseWaveData (&wir);

   return EXIT_SUCCESS;
}


WAVE convolution (WAVE win, WAVE wir)
{
   int i, j;
   WAVE w;
   int sl, sr, smax;
   double k;

   w = CreateEmptyCDaudioWave
          (win.numofstereosamples + wir.numofstereosamples);

   /* step 1: evaluate the scaling factor */
   smax = 0;

   for (i = 0; i < w.numofstereosamples; i++)
   {
      sl = sr = 0;
      for (j = 0; j < wir.numofstereosamples; j++)
      {
         if (i - j >= 0 && i - j < win.numofstereosamples)
         {
            sl += ((double) SAMPLE (win, LEFT, i - j)) *
                  (SAMPLE (wir, LEFT, j) / 32768.0);
            sr += ((double) SAMPLE (win, RIGHT, i - j)) *
                  (SAMPLE (wir, RIGHT, j) / 32768.0);
         }
      }
      if (smax < abs (sl))
         smax = abs (sl);
      if (smax < abs (sr))
         smax = abs (sr);
   }
   k = 32767.0 / smax;

   /* step 2: apply the convolution */
   for (i = 0; i < w.numofstereosamples; i++)
   {
      sl = sr = 0;
      for (j = 0; j < wir.numofstereosamples; j++)
      {
         if (i - j >= 0 && i - j < win.numofstereosamples)
         {
            sl += ((double) SAMPLE (win, LEFT, i - j)) * k *
                  (SAMPLE (wir, LEFT, j) / 32768.0);
            sr += ((double) SAMPLE (win, RIGHT, i - j)) * k *
                  (SAMPLE (wir, RIGHT, j) / 32768.0);
         }
      }
      SAMPLE (w, LEFT, i) = sl;
      SAMPLE (w, RIGHT, i) = sr;
   }

   return w;
}

