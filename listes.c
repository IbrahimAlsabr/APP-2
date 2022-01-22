#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

cellule_t* nouvelleCellule (void)
{
    cellule_t *nCel = (cellule_t*)malloc(sizeof(cellule_t));
    nCel->command = 0;
    nCel->entier = 0;
    nCel->autre = NULL;
    nCel->suivant = NULL;
    return nCel;
}

void ajout_en_tete(sequence_t *liste, int n, int type) 
{
    cellule_t *AncienTete = liste->tete;
    cellule_t *nCel = nouvelleCellule(); 
    nCel->suivant = AncienTete;
    ajouterValeur(nCel, n, type);
    nCel->type = type;
    liste->tete = nCel;
}

void ajout_en_queue(sequence_t *seq, int n,int type) { 
  cellule_t *cel;
  cel = seq->tete;
  cellule_t *nCel = nouvelleCellule();
  nCel->type = type;
  nCel->suivant = NULL;
  ajouterValeur(nCel, n, type);
  
  if (cel == NULL)
  {
    seq->tete = nCel;
  }
  else
  {
    while(cel->suivant != NULL)
    {
        cel = cel->suivant;
    }
        cel->suivant = nCel;
  }
}

void detruireCellule (cellule_t* cel)
{
    free(cel);
}

void ajouterValeur (cellule_t * cel, int valeur, int type)
{
    if (type==INTEGER)
		cel->entier = valeur;
    else
    	cel->command = (char)valeur; 
}

int depilerNombre (sequence_t* seq, bool A_Detruire)
{
    int entier = 0;
    if(A_Detruire == true)
    {
        cellule_t * cel_temp = seq->tete;
        if (cel_temp == NULL) return -1; 
        seq->tete= cel_temp->suivant;
        entier = cel_temp->entier;
        detruireCellule(cel_temp);
    }
    else
    {
        entier = seq->tete->entier;
    }
        return entier;
}

void stockageValeur(sequence_t* seq, char charLu)
{
    if (charLu == '+' || charLu == '-' || charLu == '*')
    {
        ajout_en_tete(seq, charLu, OPERATION);
    }

    else if(isdigit(charLu))
    {
        int n = atoi(&charLu);
        ajout_en_tete(seq, n, INTEGER);
    }

    else if(isalpha(charLu) || charLu == '?' || charLu == '!')
    {
        ajout_en_tete(seq, charLu, CMD);
    }
}

/*Cette fonction sert a attribuer ou prendre les pointeur des fin et ouvertures des 
crochets du vers le tableau de pointeurs */

void gererCrocherFin(cellulesChainees *cellc , cellule_t* charLu)
{
    if (charLu->type != CROCHE_FERME && charLu->type != CROCHE_OUVERT)
    {
        printf("ERROR 404 in function gererCrocherFin \n");
        return;
    }

    if(charLu->type == CROCHE_OUVERT)
    {
        charLu->autre = cellc->tab[--cellc->n];
    }

    else if(charLu->type == CROCHE_FERME)
    {
        cellc->tab[cellc->n++]=charLu; 
    }
}

void gerer_crocher(cellulesChainees *cellc , cellule_t* charLu)
{
    cellule_t* tmp ;
    if (charLu->type != CROCHE_FERME && charLu->type != CROCHE_OUVERT)
    {
        printf("ERROR 404 in function gerer_crocher\n");
        return;
    }

    if(charLu->type == CROCHE_OUVERT)
    {
        cellc->tab[cellc->n++] = charLu; 
    }

    else if(charLu->type == CROCHE_FERME)
    {
        tmp = cellc->tab[--cellc->n];
        tmp->autre = charLu;
    }
}

void conversion (char *texte, sequence_t *seq)
{
    int len = strlen(texte);
    cellulesChainees cellc;
    cellc.n = 0;
    cellule_t *cel_temp = NULL; 
    char charLu = 0;

    for (int i = len-1; i >= 0; i--)
    {   
        charLu = texte[i];
        cel_temp = seq->tete;
        if(charLu == '}')
        {
            ajout_en_tete(seq,'}',CROCHE_FERME);
            gererCrocherFin(&cellc,seq->tete);
        }

        else if(charLu == '{')
        {
            ajout_en_tete(seq,'{',CROCHE_OUVERT);
            gererCrocherFin(&cellc,seq->tete);
        }

        else
        {
            stockageValeur(seq,charLu);
        }

        if (cel_temp != NULL && cel_temp->command == '}' && charLu != ' ' && charLu != '\n')
        {
            cel_temp->autre = seq->tete;  
        }
    }
}

cellule_t* depilerCroche(sequence_t* seq)
{
    if (seq->tete==NULL)
    {
        printf("seq tete NULL (fn depilerCroche liste.c)\n");
    }
    cellule_t* c;
    cellule_t* fin;
    c = seq->tete;

    if(c->command != '{')
    {
        printf("Il y a un erreur ...\n");
    }

    if (c->autre == NULL)
    {
        printf("c->autre est NULL (fn depilerCroche liste.c)\n");
    }
    fin = c->autre;

    if (fin->suivant == NULL)
    {
        printf("fin->suivant est NULL (fn depilerCroche liste.c)\n");
    }
    seq->tete = fin->suivant;
    fin->suivant = NULL; 
    return c;
}

void empilerCroche(sequence_t* seq, cellule_t* charLu)
{
    cellule_t* fin = charLu->autre;
    fin->suivant = seq->tete;
    seq->tete = charLu; 
}

void empSansCroches(sequence_t *seq, cellule_t* celCroche)
{
    cellule_t* fin = celCroche->autre;
    cellule_t* debut= celCroche->suivant;
    detruireCellule(celCroche);
    if(debut != fin)
    { 
        cellule_t* final = fin->autre;
        final->suivant = seq->tete;
        seq->tete=debut;
    } 
}

void afficher (sequence_t* seq)
{
    assert (seq); /* Le pointeur doit être valide */
    cellule_t *cel_temp = seq->tete;
    while(cel_temp != NULL)
    {
        if (cel_temp == seq->tete)
            printf("-> ");
        
		if (cel_temp->type == INTEGER){
            printf("%d ",cel_temp->entier);
        }
		else
        {
  			printf("%c ",cel_temp->command);
        }
        cel_temp = cel_temp->suivant;
    }   printf("\n");
}

void copierCellule(cellule_t * dest, cellule_t * src)
{
    dest->type = src->type;
    dest->command = src->command;
    dest->entier = src->entier;
}

/* Cette fonction ne change pas la cel initiale , elle cree une copie en renvoyant une cellule
qui indique a ca fin en pointeur autre*/
cellule_t* copierProg(cellule_t * celCroche)
{
    cellule_t* debut= celCroche->suivant;
    cellule_t* fin = celCroche->autre;
    cellule_t* c = debut;
    cellule_t* nCel;
    cellule_t* prec_nCel = NULL;
    cellule_t* debut_nCel;
    cellule_t* fin_nCel;
    cellulesChainees cellc;
   
    while(c != fin)
    {
        nCel = nouvelleCellule();
        copierCellule(nCel,c);

        if (prec_nCel==NULL)
        {
            debut_nCel = nCel;
        }
        else
        {
            prec_nCel->suivant = nCel;
        }

        if (nCel->type==CROCHE_OUVERT)
        {
            gerer_crocher(&cellc,nCel);
        }
        else if(nCel->type==CROCHE_FERME)
        {
            gerer_crocher(&cellc,nCel);
            nCel->autre = prec_nCel;
        }
        prec_nCel = nCel;
        c = c->suivant;
    }

    fin_nCel = nCel;
    fin_nCel->suivant = NULL;
    debut_nCel->autre = fin_nCel;
    return debut_nCel;
}