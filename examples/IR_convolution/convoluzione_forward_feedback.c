
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
   WAVE w;
   int i, j;
   int sr, sl;
   double k;

   w = CreateEmptyCDaudioWave
          (win.numofstereosamples + wir.numofstereosamples);
   k = 1.0;

   TRY_AGAIN:

   for (i = 0; i < w.numofstereosamples; i++)
   {
      SAMPLE (w, LEFT, i) = 0;
      SAMPLE (w, RIGHT, i) = 0;
   }

   for (i = 0; i < win.numofstereosamples; i++)
   {
      for (j = 0; j < wir.numofstereosamples; j++)
      {
         sr = k * SAMPLE (wir, RIGHT, j) / 32767.0 * SAMPLE (win, RIGHT, i);
         sl = k * SAMPLE (wir, LEFT, j) / 32767.0 * SAMPLE (win, LEFT, i);
         sr += SAMPLE (w, RIGHT, i + j);
         sl += SAMPLE (w, LEFT, i + j);
         if (sl > 32767 || sl < -32767 || sr > 32767 || sr < -32767)
         {
            /* overflow */
            k = k * 0.9;
            printf ("overflow; k = %f\n", k);

            goto TRY_AGAIN;
         }
         SAMPLE (w, RIGHT, i + j) = sr;
         SAMPLE (w, LEFT, i + j) = sl;
      }
   }

   return w;
}







