
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


WAVE make_sequence
   (WAVE grancassa, WAVE rullante, WAVE charleston,
    int time_slot, char *seq_str);

void insert_wave (int starting_position, WAVE wave_out, WAVE wave_in);


int main (int argc, char *argv[])
{
   FILE *fpg, *fpr, *fpc, *fpout;
   WAVE grancassa, rullante, charleston, sequence;
   int time_slot;

   if (argc != 4)
   {                                                         
      printf ("USAGE: drum <time_slot> <sequence> <output filename>\n");
      exit (EXIT_FAILURE);
   }

   time_slot = atoi (argv[1]);

   if ((fpout = fopen (argv[3], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }


   /* open and load the drum waves */

   if ((fpg = fopen ("grancassa.wav", "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   if ((fpr = fopen ("rullante.wav", "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   /* open and load the drum waves */
   if ((fpc = fopen ("charleston.wav", "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   grancassa = ReadWave (fpg);
   fclose (fpg);
   rullante = ReadWave (fpr);
   fclose (fpr);
   charleston = ReadWave (fpc);
   fclose (fpc);


   sequence = make_sequence
                 (grancassa, rullante, charleston, time_slot, argv[2]);


   WriteWave (sequence, fpout);
   fclose (fpout);
   ReleaseWaveData (&sequence);
   ReleaseWaveData (&grancassa);
   ReleaseWaveData (&rullante);
   ReleaseWaveData (&charleston);

   return EXIT_SUCCESS;
}


WAVE make_sequence
   (WAVE grancassa, WAVE rullante, WAVE charleston,
    int time_slot, char *seq_str)
{
   WAVE wseq;
   int seq_num_of_samples, i, position;

   seq_num_of_samples = strlen (seq_str) * time_slot * SAMPLINGRATE / 1000.0;

   wseq = CreateEmptyCDaudioWave (seq_num_of_samples);

   for (i = 0; i < wseq.numofstereosamples; i++)
   {
      /* inizialization: silence */
      SAMPLE(wseq, RIGHT, i) = SAMPLE(wseq, LEFT, i) = 0;
   }

   i = 0;
   position = 0;

   while (seq_str[i] != '\0')
   {
      switch (seq_str[i])
      {
         case 'G': insert_wave (position, wseq, grancassa);
                   break;

         case 'R': insert_wave (position, wseq, rullante);
                   break;

         case 'C': insert_wave (position, wseq, charleston);
                   break;

         default:  printf ("unknown sequence code\n");
      }
      position += time_slot * SAMPLINGRATE / 1000.0;
      i++;
   }

   return wseq;
}


void insert_wave (int starting_position, WAVE wave_out, WAVE wave_in)
{
   int i, j;

   for (i = 0, j = starting_position;
        i < wave_in.numofstereosamples && j < wave_out.numofstereosamples;
        i++, j++)
   {
      SAMPLE(wave_out, RIGHT, j) += SAMPLE(wave_in, RIGHT, i);
      SAMPLE(wave_out, LEFT, j) += SAMPLE(wave_in, LEFT, i);
   }

   return;
}

