/* OLARU Gabriel-312CB */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <stdbool.h>
#include "tarb.h"


int if_tilda( char * word)
{
	int i = 0;
	for(i = 0 ; word[i] != '\0' ; i++)  //citesc caracter pana la sfarsitul sirului ('\0')
		{
			if(word[i] == '~' ) return 1; // are '~' autocomplete		
		}		
	return 0; // nu are '~'
}

//*************************
//tema3 - Program principal
//*************************  

int main(int argc, char* argv[])
{
	FILE * f_in, *f_out; //argumente 1 si 2 linie comanda
    char* line = NULL;
    char *numefis_in, *numefis_out;
	size_t len = 0;
    numefis_in = argv[1];
    numefis_out = argv[2];

    char *cmd1 = "add_book";
    char *cmd2 = "search_book";
    char *cmd3 = "list_author";
    char *cmd4 = "search_by_author";
    char *cmd5 = "delete_book";

	int nivel = 0, nr = 0;
	char str[40];
	
  //deschid fis de intrare si fis de iesire 

    f_in = fopen(numefis_in, "rt");
    if(f_in == NULL)
    	return 1;
	 f_out = fopen(numefis_out, "w+");
    if(f_out == NULL)
    	return 1;

    TArb T1, T2 ;
	T1 = ConstrNod();	//construiesc root nod T1 - carti
	if(!T1)
	{
		free(T1);
		return -1;
	}
	T2 = ConstrNod();	//construiesc root nod T2 - autor
	if(!T2)
	{
		free(T2); 
		return -1;
	}

    while (getline(&line,&len,f_in) != -1 && strlen(line) != 1)
    	{
    		if(line[strlen(line) - 1] == '\n')
   			line[strlen(line) - 1] = '\0' ; // se adauga \0 la sfarsit pentru parsare corecta

  		char * cmd = strtok(line," ");

//cautare comenzi din fisier f.in

//comanda add_book

 			if (strcmp(cmd, cmd1) == 0)
			{
				char * titlus = strtok(NULL, ":");
				char * autors = strtok(NULL, ":");
				char * ratings = strtok(NULL, ":");
				char * nr_pags = strtok(NULL, "");	//nr. de pagini
				int rating = atoi(ratings);
				int nr_pag = atoi(nr_pags);

				if( CautaTitlu(T1,titlus) == NULL ) 
				{
					TCarte *carte = malloc(sizeof(TCarte));
					if(carte == NULL) 
					{
						DistrT1(&T1);
						DistrT2(&T2);
						free(line);	    //eliberez buffer citire din fisier intrare
    					fclose(f_in);	//inchid fisierele de intare si iesire
						fclose(f_out);
						return -1;
					}
					carte->titlu = malloc(sizeof(char)*50);
					if(carte->titlu == NULL)
					{	//esec = eliberare memorie
						free(carte->titlu);
						free(carte->autor);
						free(carte);
						DistrT1(&T1);
						DistrT2(&T2);
						free(line);	     //eliberez buffer citire din fisier intrare
						fclose(f_in);	 //inchid fisierele de intare si iesire
						fclose(f_out);
						return -1;
					}
					carte->autor = malloc(sizeof(char)*40);
					if(!carte->autor)
					{//esec = eliberare memorie
						free(carte->titlu);
						free(carte->autor);	
						free(carte);				

						DistrT1(&T1);
						DistrT2(&T2);
						fclose(f_in);	//inchid fisierele de intare si iesire
						fclose(f_out);
						return -1;
						}

					strcpy(carte->titlu, titlus);
					strcpy(carte->autor, autors);

					carte->rating = rating;
					carte->nr_pag = nr_pag;

					if( InsertA(T1, titlus, carte) ==0 ) 	//daca nu a gasit titlu insereaza in arbore T1
				  						  				//daca e titlu nu fac nimic
					{ //esec = eliberare memorie
						free(carte->titlu);
						free(carte->autor);
						free(carte);
						DistrT1(&T1);
						DistrT2(&T2);

						return -1;
					}
					TArb T4 = CautaAutor(T2, autors);	//T4 pointer arbore carti daca ex. autor in T2
													//sau NULL DACA N-A GASIT

					if( T4 == NULL) 	//T4 nod de carti autor NULL deci autorul nu exista 
					{	
						//nu exista autor in T2,se adauga
						TArb T3 = ConstrNod(); //nod de carte
						if(T3 == NULL) 
						{//esec = eliberare memorie

							free(carte->titlu);
							free(carte->autor);
							free(carte);
							DistrT1(&T3);

							DistrT1(&T1);
							DistrT2(&T2);	
							return -1;
						}

						if(InsertA(T2, autors, T3) == 0)	//adauga autor si arbore carti proprii
						{//esec = eliberare memorie
							free(carte->titlu);
							free(carte->autor);
							free(carte);
							DistrT1(&T3);

							DistrT1(&T1);
							DistrT2(&T2);
							return -1;
						}

						if (InsertA(T3, titlus, carte) == 0)	//adauga carte in arborele carti proprii
						{//esec = eliberare memorie
							free(carte->titlu);
							free(carte->autor);
							free(carte);
							DistrT1(&T3);

							DistrT1(&T1);
							DistrT2(&T2);
							return -1;
						}

					}
					else if(CautaTitlu(T4, titlus) == NULL)	//daca n-a gsit carte o introduc in arborele de carti ale autorului
							if (InsertA(T4, titlus, carte)==0)
							{//esec = eliberare memorie
								free(carte->titlu);
								free(carte->autor);
								free(carte);

								DistrT1(&T4);
								DistrT1(&T1);
								DistrT2(&T2);
								return -1;
							}

				}

			}	//sfarsit add_book

//comanda search_book

			if (strcmp(cmd, cmd2) == 0)
			{
				char * word = strtok(NULL,"");

				if(if_tilda(word) == 0 )
				{
					// search_book <titlu>
				TCarte * carte = CautaTitlu(T1,word);
				if( carte == NULL )
					fprintf(f_out,"Cartea %s nu exista in recomandarile tale.\n",word);	
				else
					fprintf(f_out,"Informatii recomandare: %s, %s, %d, %d\n",carte->titlu, carte->autor, carte->rating, carte->nr_pag);
				}
				else
				{
					// search_book <prefix_titlu>

					word[strlen(word)-1] = '\0';
					len = strlen(word);
					TArb T5 = CautaPTitlu(T1, word);
					nr = 0;
					if( T5 == NULL )
						fprintf(f_out,"Nicio carte gasita.\n");	
					else
					
						AfisFA3(T5, f_out, &nr);
					
				}
			}	//sfarsit c-da search_book

//comanda list_author 

			if (strcmp(cmd, cmd3) == 0)
			{

				char * word = strtok(NULL,"");
				if(if_tilda(word) == 0 )
					{
						// list_author  <titlu>
						TArb T4 = CautaAutor(T2, word);	//T4 pointer arbore carti daca ex. autor in T2
													    //sau NULL DACA N-A GASIT
						if( T4 == NULL) 
							//nu exista autor in T2
							fprintf(f_out,"Autorul %s nu face parte din recomandarile tale.\n", word);	
							
						else 
							AfisFA(T4, f_out);
					}
					else
					{
						// list_author  <prefix_titlu>

						word[strlen(word)-1] = '\0';
						len = strlen(word);
						TArb T5 = CautaPTitlu(T2, word);
						nr = 0;
						if( T5 == NULL )
							fprintf(f_out,"Niciun autor gasit.\n");	
						else
							AfisFAA3(T5, word, f_out, str, nivel, &nr);
					}
			}	//sfarsit c-da list_autor

//comanda search_by_author 

			if (strcmp(cmd, cmd4) == 0)
			{
				char * autors = strtok(NULL,":");
				if( if_tilda(autors) == 0 ) 
				{	//nu are tilda la autor
					char * titlus = strtok(NULL,"");
					if(if_tilda(titlus) == 0 )
					{

						TArb T4 = CautaAutor(T2, autors);	//T4 pointer arbore carti daca ex. autor in T2
													    //sau NULL DACA N-A GASIT
						if( T4 == NULL) 
							//nu exista autor in T2
							fprintf(f_out,"Autorul %s nu face parte din recomandarile tale.\n", autors);	
						
						else
						{
							TCarte * carte = CautaTitlu(T4,titlus);
							if( carte == NULL )
								fprintf(f_out,"Cartea %s nu exista in recomandarile tale.\n", titlus);	
							else
								fprintf(f_out,"Informatii recomandare: %s, %s, %d, %d\n",carte->titlu, carte->autor, carte->rating, carte->nr_pag);
						}
					}
					else	//are tilda la titlu
					{
						// search_by_author   <prefix_titlu>
						titlus[strlen(titlus)-1]='\0';
						len = strlen(titlus);
						TArb T5 = CautaAutor(T2,autors);;
						if( T5 == NULL )
							fprintf(f_out,"Autorul %s nu face parte din recomandarile tale.\n", autors);	
						else
						{
							TArb T6 = CautaPTitlu(T5, titlus);
							nr = 0;

							if( T6 == NULL )
								fprintf(f_out,"Nicio carte gasita.\n");	
							else
								AfisFA3(T6, f_out, &nr);
						}
					}
				}
				else	//serach_by_autor are tilda la autor
				{
					autors[strlen(autors)-1] = '\0';
					len = strlen(autors);
					TArb T5 = CautaPTitlu(T2, autors);
					nr = 0;
					if(T5 == NULL)
						fprintf(f_out,"Niciun autor gasit.\n");	
					else
						AfisFAA3(T5, autors, f_out, str, nivel, &nr);
				}
			}	//sfarsit c-da search_by_autor

//comanda delete_book 

			if (strcmp(cmd, cmd5) == 0)
			{

				char * word = strtok(NULL,"");
				TCarte * carte = CautaTitlu(T1,word);
				if( carte == NULL )
					fprintf(f_out,"Cartea %s nu exista in recomandarile tale.\n", word);	
				else
				{
					TArb T4 = CautaAutor(T2,carte->autor);	//T4 pointer arbore carti daca ex. autor in T2 sau NULL DACA N-A GASIT													    
					nr = 0;
					TArb T3 = delete_book(T4, word, nr);
					nr = 0;
					if (T3 == NULL) //era singura carte,ster si autorul din T2
					{
						free(T3);
						delete_autor(T2, carte->autor, nr);
					}

					delete_book(T1, word, nr);
					free(carte->titlu);
					free(carte->autor);
					free(carte);
					carte = NULL;
				}
			}
		// sfarsit c-da delete_book							

    	} // inchidere while line
		
	free(line);	//eliberez buffer citire din fisier intrare

	DistrT1(&T1);
	DistrT2(&T2);
	
    fclose(f_in);	//inchid fisierele de intare si iesire
	fclose(f_out);

    return 0;
}
