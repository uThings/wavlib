
/*****************************************************************

 Copyright 2009   PIER LUCA MONTESSORO

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

#define DIM_DIZIONARIO 100
#define FILEDIZIONARIO "dizionario.dat"

struct parola
{
   char parola[32];
   WAVE pronuncia;
};


void copia_pronuncia (WAVE frase, int posizione,
                      struct parola dizionario[], int indice_parola);
int carica_dizionario (struct parola dizionario[], int maxdim);
int cerca_in_dizionario (char s[], struct parola dizionario[], int nparole);



int main (int argc, char *argv[])
{
   FILE *fpout;
   struct parola dizionario[DIM_DIZIONARIO];
   int nparole;   /* dimensione corrente del dizionario */
   int i;
   int indice_parola;
   int lunghezza_frase = 0;  /* numero di campioni totale */
   int posizione;
   WAVE frase;


   if (argc < 3)
   {
      printf ("servono almeno due argomenti (nome file e una parola)\n");
      exit (EXIT_FAILURE);
   }

   if ((fpout = fopen (argv[1], "wb")) == NULL)
   {
      printf ("Errore di apertura del file\n");
      exit (EXIT_FAILURE);
   }

   nparole = carica_dizionario (dizionario, DIM_DIZIONARIO);
   
   for (i = 2; i < argc; i++)
   {
      indice_parola = cerca_in_dizionario (argv[i], dizionario, nparole);

      if (indice_parola == -1)
      {
         printf ("attenzione: la parola %s non e` presente nel dizionario\n", argv[1]);
      }
      else
      {
         lunghezza_frase +=
             dizionario[indice_parola].pronuncia.numofstereosamples;
      }
   }


   /* crea la wave di uscita */
   frase = CreateEmptyCDaudioWave (lunghezza_frase);
   posizione = 0;

   for (i = 2; i < argc; i++)
   {
      indice_parola = cerca_in_dizionario (argv[i], dizionario, nparole);

      if (indice_parola != -1)
      {
         copia_pronuncia (frase, posizione, dizionario, indice_parola);
         posizione += dizionario[indice_parola].pronuncia.numofstereosamples;
      }
   }

   WriteWave (frase, fpout);
   fclose (fpout);

   return EXIT_SUCCESS;
}



int carica_dizionario (struct parola dizionario[], int maxdim)
{
   FILE *fpdiz, *fpwav;
   char parola[32], nomefilewav[32];
   int i;
   
   if ((fpdiz = fopen (FILEDIZIONARIO, "r")) == NULL)
   {
      printf ("Errore di apertura del file %s\n", FILEDIZIONARIO);
      exit (EXIT_FAILURE);
   }

   i = 0;
   while (fscanf (fpdiz, "%s %s", parola, nomefilewav) != EOF)
   {
      strcpy (dizionario[i].parola, parola);
      
      if ((fpwav = fopen (nomefilewav, "rb")) == NULL)
      {
         printf ("Errore di apertura del file %s\n", nomefilewav);
         exit (EXIT_FAILURE);
      }

      dizionario[i].pronuncia = ReadWave (fpwav);
      fclose (fpwav);
      
      i++;
      if (i >= maxdim)
      {
         printf ("il dizionario e` pieno!\n");
         return i;
      }
   }      

   return i;
}



int cerca_in_dizionario (char s[], struct parola dizionario[], int nparole)
{
   int i;

   i = 0;

   while (i < nparole)
   {
      if (strcmp (s, dizionario[i].parola) == 0)
         return i;

      i++;
   }

   return -1;
}



void copia_pronuncia (WAVE frase, int posizione,
                      struct parola dizionario[], int indice_parola)
{
   int i, j;

   for (i = 0, j = posizione;
        i < dizionario[indice_parola].pronuncia.numofstereosamples;
        i++, j++)
   {
      SAMPLE (frase, LEFT, j) = 
         SAMPLE (dizionario[indice_parola].pronuncia, LEFT, i);
      SAMPLE (frase, RIGHT, j) = 
         SAMPLE (dizionario[indice_parola].pronuncia, RIGHT, i);
   }

   return;
}










