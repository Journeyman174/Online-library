/* OLARU Gabriel-312CB */

/*-- tarb.h --- arbori trie ---*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#define A_SIZE (68)

#ifndef _ARBORE_TRIE_
#define _ARBORE_TRIE_

typedef struct nod
{
  void* info;                 /* adresa informatie */
  bool frunza;                /* este frunza */
  struct nod *fii[A_SIZE];    /* adresa urmatoarilor noduri fii */
} TNod, *TArb;                /* tipurile nod, arbore  */

typedef struct carte
{
  char * titlu;
  char * autor;
  int rating;
  int nr_pag;
} TCarte;


//functii arbore 

TArb ConstrNod(); /* -> adresa nod (initializat cu NULL */
int InsertA(TArb a, char * word, void *ae );	//insereaza titlu in arbore
TCarte * CautaTitlu(TArb a, char *word);
TArb CautaAutor(TArb a, char *word);
void AfisFA(TArb r, FILE * f_out);
void AfisFAA(TArb r, FILE * f_out);
void AfisFAA3(TArb r, char * word, FILE * f_out, char autor[], int nivel, int *nr);
TArb CautaPTitlu(TArb a, char *word);
void AfisFA3(TArb r, FILE * f_out, int *nr);

TArb delete_book(TArb r, char *titlu, int nivel);
bool Gol(TArb r);
TArb delete_autor(TArb r, char *autor, int nivel);
void DistrT2(TArb *a);/* distruge nodurile interioare la nodul frunza distruge T2 */
void DistrT1(TArb *a);/* distruge toate nodurile arborelui de la adresa a */
                      /* distruge nodurile interioare la nodul frunza distruge T2 */

#endif
