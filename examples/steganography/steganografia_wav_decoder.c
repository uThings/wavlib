
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
   steganografia: estrae un messaggio di testo occultato in un file audio
*/
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


#define SETBIT(data, pos, value) \
           (data) = (((data) & ~(1 << (pos))) | (((value) & 1) << pos))

#define GETBIT(data, pos) \
           (((data) >> (pos)) & 1)


void decode (WAVE w, char s[]);


int main (int argc, char *argv[])
{
   FILE *fpin;
   WAVE wave;
   char message[1024];

   if (argc != 2)
   {                                                         
      printf ("USO: steganografia_wav_decode <file in>\n");
      exit (EXIT_FAILURE);
   }

   if ((fpin = fopen (argv[1], "rb")) == NULL)
   {
      printf ("Error opening input file\n");
      exit (EXIT_FAILURE);
   }

   wave = ReadWave (fpin);
   fclose (fpin);

   decode (wave, message);
   printf ("%s\n", message);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


void decode (WAVE w, char s[])
{
   long int i;
   short int bit;
   int j, bit_counter;

   j = 0;
   i = 0;
   bit_counter = 0;
   s[j] = 0;
   while (i < w.numofstereosamples)
   {
      bit = GETBIT (SAMPLE(w, RIGHT, i), 0);
      SETBIT (s[j], bit_counter, bit);

      bit_counter++;

      bit = GETBIT (SAMPLE(w, LEFT, i), 0);
      SETBIT (s[j], bit_counter, bit);

      bit_counter++;
      bit_counter = bit_counter % 8;
      if (bit_counter == 0)
      {
         if (s[j] == '\0')
            return;
         j++;
         s[j] = 0;
      }

      i++;
   }

   s[j] = '\0';
   printf ("attenzione: messaggio non terminato\n");

   return;
}

