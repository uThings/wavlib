
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
   RLE silence un-compress in speech
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


WAVE RLE_uncompress (WAVE w);


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
   compressed_wave = ReadWave (fpin);
   fclose (fpin);

   wave = RLE_uncompress (compressed_wave);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);
   ReleaseWaveData (&compressed_wave);

   return EXIT_SUCCESS;
}

WAVE RLE_uncompress (WAVE w)
{
   long int i, j, compressed_len;
   short int sl, sr;
   int counter;
   WAVE uncompressed_wave;

   /* step 1: compute length */

   compressed_len = 0;
   counter = 0;
   i = 0;
   while (i < w.numofstereosamples)
   {
      sl = SAMPLE(w, RIGHT, i);
      sr = SAMPLE(w, LEFT, i);
      compressed_len++;
      i++;

      if (sl == 0 && sr == 0)
      {
         counter++;

         if (counter == 3)  /* new sequence found! */
         {
            sl = SAMPLE(w, RIGHT, i);
            sr = SAMPLE(w, LEFT, i);
            if (sl != sr)
            {
               printf ("wrong compressed format\n");
               exit (EXIT_FAILURE);
            }
            compressed_len += sl - 3; /* 3 samples already written! */
            i++;
            counter = 0;
         }
      }
      else
      {
         counter = 0;
      }
   }


   /* step 2: expand the compressed wave */

   uncompressed_wave = CreateEmptyCDaudioWave (compressed_len);

   counter = 0;
   i = j = 0;
   while (i < w.numofstereosamples)
   {
      sl = SAMPLE(w, RIGHT, i);
      sr = SAMPLE(w, LEFT, i);
      SAMPLE(uncompressed_wave, RIGHT, j) = sr;
      SAMPLE(uncompressed_wave, LEFT, j) = sl;
      i++; j++;

      if (sl == 0 && sr == 0)
      {
         counter++;

         if (counter == 3)  /* new sequence found! */
         {
            int t;

            sl = SAMPLE(w, RIGHT, i);
            i++;

            for (t = 0; t < sl - 3; t++)    /* 3 samples already written! */
            {
               SAMPLE(uncompressed_wave, RIGHT, j) = 0;
               SAMPLE(uncompressed_wave, LEFT, j) = 0;
               j++;
            }
            counter = 0;
         }
      }
      else
      {
         counter = 0;
      }
   }

   return uncompressed_wave;
}

