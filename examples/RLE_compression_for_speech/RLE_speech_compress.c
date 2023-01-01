
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
   RLE silence compress in speech
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


#define MAX_SHORT_VAL 0x7FFF

WAVE RLE_compress (WAVE w);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave, compressed_wave;

   if (argc != 3)
   {                                                         
      printf ("USAGE: compress "
              "<source file> <destination file>\n");
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

   compressed_wave = RLE_compress (wave);

   /* write the file */
   WriteWave (compressed_wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);
   ReleaseWaveData (&compressed_wave);

   return EXIT_SUCCESS;
}

WAVE RLE_compress (WAVE w)
{
   long int i, j, compressed_len;
   short int sl, sr;
   int counter;
   int inside_seq;
   WAVE temp_wave;
   WAVE compressed_wave;

   /* step 1: copy the samples in temp_wave while RLE compressing */

   temp_wave = CreateEmptyCDaudioWave (w.numofstereosamples);

   j = 0;
   inside_seq = 0;
   counter = 0;

   for (i = 0; i < w.numofstereosamples; i++)
   {
      sl = SAMPLE(w, RIGHT, i);
      sr = SAMPLE(w, LEFT, i);

      if (!inside_seq)
      {
         /* copy the samples */
         SAMPLE(temp_wave, RIGHT, j) = sr;
         SAMPLE(temp_wave, LEFT, j) = sl;
         j++;
      }

      if (sl == 0 && sr == 0)
      {
         counter++;

         if (counter == 3)  /* new sequence found! */
         {
            inside_seq = 1;
         }
         else if (counter == MAX_SHORT_VAL-1)
         {
            /* max seq length reached */
            SAMPLE(temp_wave, RIGHT, j) = counter;
            SAMPLE(temp_wave, LEFT, j) = counter;

            j++;
            counter = 0;
            inside_seq = 0;
         }
      }
      else
      {
         if (inside_seq)
         {
            /* seq terminated */
            SAMPLE(temp_wave, RIGHT, j) = counter;
            SAMPLE(temp_wave, LEFT, j) = counter;
            j++;
            SAMPLE(temp_wave, RIGHT, j) = sr;
            SAMPLE(temp_wave, LEFT, j) = sl;
            j++;
            inside_seq = 0;
         }
         counter = 0;
      }
   }

   /* seq at the end of the file? */
   if (inside_seq)
   {
      /* seq terminated */
      SAMPLE(temp_wave, RIGHT, j) = counter;
      SAMPLE(temp_wave, LEFT, j) = counter;
      j++;
   }


   /* step 2: copy j samples in a new wave */

   compressed_len = j;
   compressed_wave = CreateEmptyCDaudioWave (compressed_len);
   for (i = 0; i < compressed_len; i++)
   {
      SAMPLE(compressed_wave, RIGHT, i) = SAMPLE(temp_wave, RIGHT, i);
      SAMPLE(compressed_wave, LEFT, i) = SAMPLE(temp_wave, LEFT, i);
   }

   ReleaseWaveData (&temp_wave);

   return compressed_wave;
}

