
/*****************************************************************

 Copyright 2008   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/

/*****************************************************************

Nella musica occidentale le note musicali della scala cromatica sono dodici 
(le cosiddette sette note più cinque diesis/bemolli). Nella notazione 
anglosassone le note si indicano con una lettera maiuscola dell’alfabeto: 
A=la, B=si, ... , G=sol. Le note possono essere seguite dal carattere ‘#’ 
per indicare il diesis e da ‘b’ per indicare il bemolle. Sul pianoforte, 
il LA dell’ottava centrale è indicato come A4, trattandosi della quarta ottava. 
Tale nota è utilizzata come riferimento per l’accordatura degli strumenti ed 
ha la frequenza di 440 Hz. Quattro ottave più in basso si trova la prima nota 
del pianoforte, A1, alla frequenza di 55 Hz. La nota più alta è il C9, cioè 
la prima nota della nona ottava.

Si assuma di disporre della funzione    double freq (char nota[], int ottava);
che riceve in ingresso il nome di una nota in notazione anglosassone e 
il numero dell’ottava, e che restituisce la frequenza di tale nota.

Si scriva un programma in linguaggio C che riceva sulla riga di comando 
il nome di un file wave di uscita e una sequenza di note rappresentate 
nella notazione sopra descritta. Il programma deve produrre nel file 
di uscita la melodia rappresentata dalle note.

Ogni nota sia indicata nella forma seguente: 
<nota><ottava>/<durata in millisecondi>. 
Le pause (silenzi) siano indicate con P/<durata>. 

Per esempio:
D:\>make_melody per_elisa.wav E5/200 D#5/200 E5/200 D#5/200 E5/200 B4/200 D5/200 C5/200 A4/200
oppure
D:\>make_melody prova_pause.wav A4/100 P/400 A4/100 P/200 A4/400

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wav.h"


double freq (char note[], int octave);


int main (int argc, char *argv[])
{
   FILE *fpout;
   WAVE wave;
   int i;
   double f;
   long int sample, start, total_length;
   char note[8];
   int octave, duration;
   

   if (argc < 3)
   {
      printf ("make_melody outputfile notes...\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Error opening output file\n");
      exit (EXIT_FAILURE);
   }

   total_length = 0;
   for (i = 2; i < argc; i++)
   {
      if (extract_note (argv[i], note, &octave, &duration) == -1)
         printf ("wrong code for note %s\n", argv[i]);
      else
         total_length += duration;
   }
   
   wave = CreateEmptyCDaudioWave (total_length / 1000.0 * SAMPLINGRATE);

   start = 0;
   for (i = 2; i < argc; i++)
   {
      if (extract_note (argv[i], note, &octave, &duration) == -1)
      {
         printf ("wrong code for note %s\n", argv[i]);
      }
      else
      {
         if (note[0] == 'P')
         {
            for (sample = 0; sample < duration / 1000.0 * SAMPLINGRATE && sample < wave.numofstereosamples; sample++)
            {
               SAMPLE(wave, RIGHT, start + sample) = 
               SAMPLE(wave, LEFT, start + sample) = 0;
            }
         }
         else
         {
            f = freq (note, octave);
            for (sample = 0; sample < duration / 1000.0 * SAMPLINGRATE && sample < wave.numofstereosamples; sample++)
            {
               SAMPLE(wave, RIGHT, start + sample) = 
                  25000 * sin (sample / 44100.0 * 2 * M_PI * f);
               SAMPLE(wave, LEFT, start + sample) = 
                  25000 * sin (sample / 44100.0 * 2 * M_PI * f);
            }
         }
         start += sample;
      }
   }

   /* write the file */
   WriteWave (wave, fpout);
   fclose (fpout);

   ReleaseWaveData (&wave);

   return EXIT_SUCCESS;
}



extract_note (char s[], char note[], int *p_octave, int *p_duration)
{
   int i, j;
   char str_duration[16];
   
   note[0] = s[0];
   if (s[1] == '#' || s[1] == 'b')
   {
      note [1] = s[1];
      note[2] = '\0';
      i = 2;
   }
   else
   {
      note[1] = '\0';
      i = 1;
   }
   
   if (note[0] != 'P')
   {
      *p_octave = s[i] - '0';
      i++;
   }
   
   if (s[i] != '/')
      return -1;
   
   i++;
   j = 0;
   
   while (s[i] != '\0')
   {
      str_duration[j] = s[i];
      i++;
      j++;
   }
   
   str_duration[j] = '\0';
   *p_duration = atoi (str_duration);
   
   return 1;
}



struct basic_note
{
   char name[3];
   int index;
};


double freq (char note[], int octave)
{
   struct basic_note notes[17] = { {"A", 0}, {"A#", 1}, {"Bb", 1}, {"B", 2}, 
                                   {"C", -9}, {"C#", -8}, {"Db", -8}, {"D", -7}, 
                                   {"D#", -6}, {"Eb", -6}, {"E", -5}, 
                                   {"F", -4}, {"F#", -3}, {"Gb", -3}, {"G", -2}, 
                                   {"G#", -1}, {"Ab", -1} };
   int i;

   for (i = 0; i < 17; i++)
   {
      if (strcmp (note, notes[i].name) == 0)
         return 55 * pow (2, (12 * (octave - 1) + notes[i].index) / 12.0);
   }

   printf ("note %s not found!\n", note);
   exit (EXIT_FAILURE);
}





