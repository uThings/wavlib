
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
   steganografia: occulta un messaggio di testo in un file audio
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


void encode (WAVE w, char s[]);


int main (int argc, char *argv[])
{
   FILE *fpin, *fpout;
   WAVE wave;
   char message[1024];

   if (argc != 4)
   {                                                         
      printf ("USO: steganografia_wav_encoder "
              "<file in> <file out> <messaggio>\n");
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

   encode (wave, argv[3]);

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}


void encode (WAVE w, char s[])
{
   long int i;
   short int bit;
   int j, bit_counter;

   if (strlen(s) * 4 > w.numofstereosamples)
   {
      printf ("attenzione: messaggio troppo lungo, verra` troncato\n");
   }

   j = 0;
   i = 0;
   bit_counter = 0;
   while (j <= strlen(s))
   {
      bit = GETBIT (s[j], bit_counter);
      SETBIT (SAMPLE(w, RIGHT, i), 0, bit);

      bit_counter++;

      bit = GETBIT (s[j], bit_counter);
      SETBIT (SAMPLE(w, LEFT, i), 0, bit);

      bit_counter++;
      bit_counter = bit_counter % 8;
      if (bit_counter == 0)
         j++;

      i++;
      if (i >= w.numofstereosamples)
      {
         printf ("attenzione: messaggio troppo lungo\n");
         return;
      }
   }

   return;
}

