
/****************************************************************************

Si vuole costruire una melodia basandosi sul suono registrato di 12 
campanellini (uno per nota musicale). 


La melodia è descritta da una sequenza di stringhe che identificano le note 
(considerando solo i “bemolle”, non i “diesis”) come nel seguente esempio:

do do mibemolle mibemolle sol sol la sibemolle fa fa fa re re re do do do do

Il suono di ogni nota è contenuto in un file wave con lo stesso nome della nota.
La durata di ogni nota della melodia è costante e vale 300 ms. La durata delle 
registrazioni dei campanellini è variabile e può essere superiore o inferiore 
a 300 ms.

Si scriva un programma in linguaggio C che riceva sulla riga di comando il 
nome di un file wave di uscita e la melodia nel formato sopra descritto. 
Il programma deve scrivere nel file la melodia in formato audio.

****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"


#define INTERVALLO 0.3


struct suoni
{
   char nome_nota[16];
   WAVE suono_nota;
};


int carica_suoni (struct suoni campanelli[]);
WAVE crea_melodia (char s[], struct suoni campanelli[]);
int cerca_nota (char nome_nota[], struct suoni campanelli[], int numero_note);
void copia_nota (WAVE wout, WAVE win, int posizione);



int main (int argc, char *argv[])
{
   WAVE w;
   char s[128];
   struct suoni campanelli[12];
   int numero_note, lunghezza_melodia, i, indice_nota;
   FILE *f;

   if (argc < 2)
   {
      printf ("argomenti richiesti: nome del file di uscita e sequenza note\n");
      exit (EXIT_FAILURE);
   }

   numero_note = carica_suoni (campanelli);

   /* calcola la lunghezza dell'ultima nota per l'eventuale coda */
   indice_nota = cerca_nota (argv[argc-1], campanelli, numero_note);
   if (indice_nota == -1)
   {
      printf ("nota non trovata: %s\n", argv[i+2]);
      exit (EXIT_FAILURE);
   }
   lunghezza_melodia = INTERVALLO * 44100 * (argc-3) + campanelli[indice_nota].suono_nota.numofstereosamples;

   w = CreateEmptyCDaudioWave (lunghezza_melodia);
   for (i = 0; i < w.numofstereosamples; i++)
      SAMPLE (w, LEFT, i) = SAMPLE (w, LEFT, i) = SAMPLE (w, RIGHT, i) = 0;

   for (i = 0; i < argc - 2; i++)
   {
      indice_nota = cerca_nota (argv[i+2], campanelli, numero_note);
      if (indice_nota == -1)
      {
         printf ("nota non trovata: %s\n", argv[i+2]);
         exit (EXIT_FAILURE);
      }
      printf ("inserisco %s\n", campanelli[indice_nota].nome_nota);
      copia_nota (w, campanelli[indice_nota].suono_nota, 44100 * i * INTERVALLO);
   }

   if ((f = fopen (argv[1], "wb")) == NULL)
   {
      printf ("errore di apertura del file di uscita\n");
      exit (EXIT_FAILURE);
   }
   WriteWave (w, f);
   ReleaseWaveData (&w);

   fclose (f);

   return EXIT_SUCCESS;
}


void copia_nota (WAVE wout, WAVE win, int posizione)
{
   int i_out, i_in;

   i_out = posizione;
   i_in = 0;
   while (i_in < win.numofstereosamples && i_out < wout.numofstereosamples)
   {
      SAMPLE (wout, LEFT, i_out) += SAMPLE (win, LEFT, i_in);
      SAMPLE (wout, RIGHT, i_out) += SAMPLE (win, RIGHT, i_in);
      i_out++;
      i_in++;
   }

   return;
}
   

int cerca_nota (char nome_nota[], struct suoni campanelli[], int numero_note)
{
   int i;
   for (i = 0; i < numero_note; i++)
   {
      if (strcmp (campanelli[i].nome_nota, nome_nota) == 0)
         return i;
   }
   return -1;
}


int carica_suoni (struct suoni campanelli[])
{
   char nomi_note[12][32] = { "do", "rebemolle", "re", "mibemolle", "mi", "fa",  
                            "solbemolle", "sol", "labemolle", "la", "sibemolle", "si" };
   int numero_note = 12;
   int i;
   char nomefile[64];
   FILE *fp;

   for (i = 0; i < numero_note; i++)
   {
      strcpy (nomefile, nomi_note[i]);
      strcat (nomefile, ".wav");

      strcpy (campanelli[i].nome_nota, nomi_note[i]);

      if ((fp = fopen (nomefile, "rb")) == NULL)
      {
         printf ("errore di apertura del file %s\n", nomefile);
         exit (EXIT_FAILURE);
      }
      campanelli[i].suono_nota = ReadWave (fp);
      fclose (fp);
   }

   return numero_note;
}
