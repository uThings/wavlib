
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"


long int add_a_word (WAVE wave, long int current_position, char word[]);

int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   char fname[64];
   WAVE wave;
   int i;
   long int tot_len, current_position;

   if (argc < 3)
   {
      printf ("USAGE: say <output file> <sentence>\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }


   /* compute the output wave length */

   tot_len = 0;
   for (i = 2; i < argc; i++)
   {
      strcpy (fname, argv[i]);
      strcat (fname, ".wav");
      if ((fpin = fopen (fname, "rb")) == NULL)
      {
         printf ("Error opening input file %s\n", fname);
         exit (EXIT_FAILURE);
      }
      printf ("Opening input file %s\n", fname);
      wave = ReadWave (fpin);
      tot_len += wave.numofstereosamples;
      fclose (fpin);
      ReleaseWaveData (&wave);
   }

   /* create the output wave */
   current_position = 0;
   wave = CreateEmptyCDaudioWave (tot_len);
   for (i = 2; i < argc; i++)
   {
      current_position = add_a_word (wave, current_position, argv[i]);
   }


   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);
   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}



long int add_a_word (WAVE wave, long int current_position, char word[])
{
   FILE *fpin;
   WAVE current_wave;
   char fname[64];
   long int i;

   strcpy (fname, word);
   strcat (fname, ".wav");
   if ((fpin = fopen (fname, "rb")) == NULL)
   {
      printf ("Error opening input file %s\n", fname);
      exit (EXIT_FAILURE);
   }
   current_wave = ReadWave (fpin);
   printf ("Starting copy of file %s\n", fname);
   for (i = 0; i < current_wave.numofstereosamples; i++)
   {
      SAMPLE (wave, RIGHT, current_position) =
                         SAMPLE (current_wave, RIGHT, i);
      SAMPLE (wave, LEFT, current_position) =
                         SAMPLE (current_wave, LEFT, i);
      current_position++;
   }

   fclose (fpin);
   ReleaseWaveData (&current_wave);

   return current_position;
}
