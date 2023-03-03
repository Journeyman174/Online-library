/* OLARU Gabriel-312CB */
/*--- tarb.c --- operatii cu arbori trie --*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "tarb.h"

//intoarce indexul caracterului in alfabetul dat
int getindex (char c)
{
   int index = 0;
   char * alfabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";
   char * found;

    found = strchr (alfabet, c);
    index = found - alfabet;
	
    if (!found) return -1;
    return index;
}


TArb ConstrNod()	/* -> adresa nod (initializat cu NULL */
{
	int i;
	TArb aux = (TArb)malloc (sizeof(TNod));  /* incearca alocare spatiu */
	if (!aux) 
	{
		free(aux);
		return NULL;                   /* spatiu insuficient */
	}
	aux->info = NULL;
	for(i = 0; i < A_SIZE; i++)	aux->fii[i] = NULL;  /* actualizeaza campurile nodului */
	aux->frunza = false;
	return aux;                              /* intoarce adresa nodului */
}

//functia insereaza in arbore 
//si atribuie ->info pointer fie la struct carte fie la un arbore de carti T3

int InsertA(TArb a, char * word, void *ae )	//insereaza titlu in arbore
{
	int i, index = 0;
	int len = strlen(word);
	TArb aux = a;
	for(i = 0; i < len; i++)
	{
		index = getindex(word[i]);
		if(!aux->fii[index]) 
		{
			aux->fii[index] = ConstrNod();

			if(!aux->fii[index]) return 0;

		}

		aux = aux->fii[index];	
	}
	aux->info = ae;			//info != NULL inseamna ca e frunza
	aux->frunza = true;
	return 1;
}

//functia cauta un pointer , titlu in arborele de carti
// pointer arbore carte- a gasit 
//NULL- n-a gasit

TArb  CautaPTitlu(TArb a, char *word)
{
	int i, index = 0;
	int len = strlen(word);
	TArb aux = a;
	for(i = 0; i < len; i++)
	{
		index = getindex(word[i]);
		if(aux->fii[index] == NULL ) 	//aux->fii este este litera curenta
										//aux nodul parinte initial este ROOT
				 return NULL ;	//n-a gasit
		aux = aux->fii[index];
	}
	if (aux != NULL)
		return ((TArb)(aux)); //a gasit intorc pointer
	else	
		return NULL;
}

//functia cauta un titlu, nod cu prefix  in arborele de carti T1
// pointer carte- a gasit 
//NULL- n-a gasit

TCarte * CautaTitlu(TArb a, char *word)
{
	int i, index = 0;
	int len = strlen(word);
	TArb aux = a;
	for(i = 0; i < len; i++)
	{
		index = getindex(word[i]);
		if(aux->fii[index] == NULL ) 	//aux->fii este este litera curenta  //aux nodul parinte initial este ROOT							
		
			return NULL ;	//n-a gasit

		aux = aux->fii[index];

	}
	if (aux->info != NULL && aux->frunza == true)
		return ((TCarte*)(aux->info)); //a gasit intorc pointer
	else	
		return NULL;
}

//functia cauta un autor in arborele T2
//adresa arborelui de carti  - a gasit 
//NULL daca  - n-a gasit

TArb CautaAutor(TArb a, char *word)
{
	int i, index = 0;
	int len = strlen(word);
	TArb aux = a;
	for(i = 0; i < len; i++)
	{
		index = getindex(word[i]);
		if(aux->fii[index] == NULL) 
			return NULL; //n-a gasit
	
		aux = aux->fii[index];
	}

	if (aux->info != NULL)
		return ((TArb)(aux->info));	//a gasit
	else	
		return NULL;
}

//functie afisare titlu carte tot arbore 
void AfisFA(TArb r, FILE * f_out)
{
	int i;
	if(!r)
		return;
	else
	{
		for(i = 0; i < A_SIZE; i++)
		{
			if(r->fii[i] != NULL)
			{
				if((r->fii[i])->info != NULL)
				{
					TCarte * carte = ((TCarte *)(r->fii[i])->info);
					fprintf(f_out,"%s\n", carte->titlu);
				}
				AfisFA(r->fii[i], f_out);
			}
		}
	}
}

//functie afisare 3 titluri de carti
void AfisFA3(TArb r, FILE * f_out, int *nr)
{
	int i;
	if(!r)
		return;
	else
	{
		for(i = 0; i < A_SIZE; i++)
		{
			if(r->fii[i] != NULL)
			{
				if((r->fii[i])->info != NULL)
				{
					if( (*nr) < 3)
					{
						TCarte * carte = ((TCarte *)(r->fii[i])->info);
							fprintf(f_out,"%s\n", carte->titlu);
					}
				(*nr)++;
				}
					AfisFA3(r->fii[i], f_out, nr);
			}
		}
	}
}

//functie afisare nume autor prefix(auto-complete)
void AfisFAA3(TArb r, char * word,  FILE * f_out, char autor[], int nivel, int *nr)
{
	int i;
	char * alfabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";

	if(r->frunza == true)
	{
		if((*nr) < 3)
		{
			autor[nivel] = '\0';
			fprintf(f_out,"%s", word);
			fprintf(f_out,"%s\n", autor);
		}
		(*nr)++;

	}
	
	for(i = 0; i < A_SIZE; i++)
	{
		if(r->fii[i] != NULL)
		{
			autor[nivel] = alfabet[i];
			AfisFAA3(r->fii[i], word, f_out, autor, nivel + 1, nr);
		}
	}
	
}
//functie afisare arbore autor si arbore carti proprii 
void AfisFAA(TArb r, FILE * f_out)
{
	int i;
	
	if(!r)
		return;
	else
	{
		for(i = 0; i < 68; i++)
		{
			if(r->fii[i] != NULL)
			{
				if((r->fii[i])->info != NULL)
					AfisFA( ((TArb)((r->fii[i])->info)), f_out );
				
				AfisFAA(r->fii[i], f_out);
			}
		}
	}
}

//intoarce TRUE daca nodul este GOL : nu are fii, else FALSE

bool Gol(TArb r)
{
	int i;
	for (i = 0; i < A_SIZE; i++)
			if(r->fii[i]) return false;
	return true;
}


//functie recursiva de stergere cheie = titlu carte, din T1 si T2
//daca autorul nu mai are nici o carte se sterge si autorul di T2

TArb delete_book(TArb r, char *titlu, int nivel)
{
//daca root este gol
	if(!r) return NULL;
//daca este ultimul caracter din titlu
	if(nivel == strlen(titlu))
	{	//se sterge semnul de sfarsit de titlu
		if(r->frunza == true)
			r->frunza = false;
// daca nodul nu este prefix pentru alt titlu
		if(Gol(r))
		{
			free(r);
			r = NULL;
		}
		return r;
	}
	//daca nu este ultimul caracter din titlu, apelez recursiv pentru fii 
	int index = getindex(titlu[nivel]);
	
	r->fii[index] = delete_book(r->fii[index], titlu, nivel + 1);
// daca nu are fii(doar cei stersi) si nu este sfirsit de titlu
	if( Gol(r) && r->frunza == false)
	{
		free(r);
		r = NULL;
	}
	return r;
}

TArb delete_autor(TArb r, char *autor, int nivel)
{
//daca root este gol
	if(!r) return NULL;
//daca este ultimul caracter din titlu
	if(nivel == strlen(autor))
	{	//se sterge semnul de sfarsit de titlu
		if(r->frunza == true)
			r->frunza = false;
// daca nodul nu este prefix pentru alt titlu
		if(Gol(r))
		{
			free(r);
			r = NULL;
		}
		return r;
	}
//daca nu este ultimul caracter din titlu, apelez recursiv pentru fii 
	int index = getindex(autor[nivel]);
	
	r->fii[index] = delete_book(r->fii[index], autor, nivel + 1);
// daca nu are fii(doar cei stersi) si nu este sfirsit de titlu
	if( Gol(r) && r->frunza == false)
	{
		free(r);
		r = NULL;
	}
	return r;
}


void deleteC(TArb r)
{
	free(((TCarte *)r->info)->titlu);
	free(((TCarte *)r->info)->autor);
	free(r->info);	
}

void DistrT1(TArb *a)
{
	int i;
	TArb r = *a;
	if (!r) return;

	if(r->frunza == true)
	{
		TCarte * aux = (TCarte *)(r->info);
		if(aux != NULL)
		{
			free(aux->titlu);
			free(aux->autor);
			free(aux);
			aux = NULL;
		}
	}
	for(i = 0; i < A_SIZE; i++)
		DistrT1(&(r->fii[i]));     /* distruge subarborele fii */

	free (r);             /* distruge nodul radacina */
	r = NULL;
}

void DistrTC1(TArb *a)
{
	int i;
	TArb r = *a;
	if (!r) return;
	r->info = NULL;
	for(i = 0; i < A_SIZE; i++)
	{
		DistrTC1(&(r->fii[i]));     /* distruge subarborele fii */
	}
	free (r);             /* distruge nodul radacina */
	r = NULL;
}

void DistrT2(TArb *a)
{
	int i;
	TArb r = *a;
	if (!r) return;

	if(r->frunza == true)
	{
		TArb p = (TArb)(r->info);
		DistrTC1(&p);
	}

	for(i = 0; i < A_SIZE; i++)
	{
		DistrT2(&(r->fii[i]));     /* distruge subarborele fii */
	}

	free (r);             /* distruge nodul radacina */
	r = NULL;

}
