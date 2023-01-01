
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


#define SILENCE_LEVEL   0.001
#define SILENCE_LENGHT  3


struct track_position
{
   int starting_sample;
   int ending_sample;
};

struct track_position find_next_track (WAVE w, int search_starting_sample);


int main (int argc, char *argv[])
{
   FILE *fpin;
   WAVE w;
   int i;
   struct track_position pos;

   if (argc != 2)
   {                                                         
      printf ("USAGE: track_finder <input filename>");
      exit (EXIT_FAILURE);
   }

   if ((fpin = fopen (argv[1], "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   w = ReadWave (fpin);
   fclose (fpin);

   i = 0;
   do
   {
      pos = find_next_track (w, i);
      if (pos.starting_sample < w.numofstereosamples)
      {
         printf ("%d %d\n", pos.starting_sample, pos.ending_sample);
         i = pos.ending_sample + 1;
      }
   } while (pos.starting_sample < w.numofstereosamples);

   ReleaseWaveData (&w);

   return EXIT_SUCCESS;
}

struct track_position find_next_track (WAVE w, int search_starting_sample)
{
   int i, silence_counter;
   struct track_position pos;

   i = search_starting_sample;
   silence_counter = 0;

   /* look for first non_silence sample */
   while (i < w.numofstereosamples &&
          abs (SAMPLE(w, LEFT, i)) <= SILENCE_LEVEL &&
          abs (SAMPLE(w, RIGHT, i)) <= SILENCE_LEVEL)
   {
      i++;
   }

   pos.starting_sample = i;

   while (i < w.numofstereosamples &&
          silence_counter < SILENCE_LENGHT * 44100)
   {
      if (abs (SAMPLE(w, LEFT, i)) <= SILENCE_LEVEL &&
          abs (SAMPLE(w, RIGHT, i)) <= SILENCE_LEVEL)
      {
         silence_counter++;
      }
      else
      {
         silence_counter = 0;
         pos.ending_sample = i;
      }

      i++;
   }

   return pos;
}
