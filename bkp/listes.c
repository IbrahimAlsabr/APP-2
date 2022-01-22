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

#define DEBUG do{printf("I am here ...\n");}while(0)

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

typedef struct structchaine{
    cellule_t *tab[100];
    int n;
} cellchaine ;

void addvalue (cellule_t * cel,int value,int type){
    if (type==NUM)
		cel->entier = value;
    else
    	cel->command = (char)value; // here we are treating n as if it's a char , n should be between 0 and 255
}

/* Ajout en tete d'une sequences d'entiers implémentée par une liste chaînée */
void ajout_en_tete(sequence_t *l, int n, int type) {
    /* votre code ici */
    cellule_t *tete_ancien = l->tete;
    cellule_t *nv_tete = (cellule_t *) malloc(sizeof(cellule_t)); 
    nv_tete->suivant = tete_ancien;
    addvalue(nv_tete,n,type);
    nv_tete->type = type;
    l->tete = nv_tete;
}

int depilernum (sequence_t* seq, bool detruire){
    int entier = 0;
    if(detruire == true){
    cellule_t * t = seq->tete;
    if (t==NULL){  return -1; }
    seq->tete= t->suivant;
    entier = t->entier;
    detruireCellule(t);
    }
    else
    {
        entier = seq->tete->entier;
    }
    return entier;
}

void ajout_en_queue(sequence_t *l, int n,int type) { // changing this func , change header
  cellule_t *c;
  c = l->tete;
  cellule_t *nv = (cellule_t *) malloc(sizeof(cellule_t));
  nv->type = type;
  nv->suivant=NULL;
  addvalue(nv,n,type);
  
  if (c==NULL){
    l->tete= nv;
  }
  else{
  while(c->suivant!=NULL){
    c = c->suivant;
  }
  c->suivant = nv;
  }
}




cellule_t* nouvelleCellule (void)
{
    /* À compléter (utiliser malloc) */
    cellule_t *nv = (cellule_t*)malloc(sizeof(cellule_t));
    nv->command = 0;
    nv->entier = 0;
    nv->autre = NULL;
    nv->suivant = NULL;
    return nv;
}



void detruireCellule (cellule_t* cel)
{
    free(cel);
}

void stockagevaleur(sequence_t* seq,char c){
    if (c=='+' || c=='-' || c=='*')
        {
            ajout_en_tete(seq,c,OPR);
        }
        else if(isdigit(c)){
            int n = atoi(&c);
            ajout_en_tete(seq,n,NUM);
        }
        else if(isalpha(c) || c=='?'){
            ajout_en_tete(seq,c,CMD);
        }
}

void conversion (char *texte, sequence_t *seq)
{
    int len = strlen(texte);
    cellchaine *cellc = (cellchaine *) malloc(sizeof(cellc));
    //sequence_t *seqtemp;
    cellc->n = 0;
    //seqtemp->tete = seq->tete;
    cellule_t *cel;
    cellule_t *ctete; // current tete
    char c = 0;
    for (int i = len-1; i >= 0; i--)
    {   
        c = texte[i];
        ctete = seq->tete;
        if(c=='}'){
            ajout_en_tete(seq,'}',CROCHEF);
            cellc->tab[cellc->n++] = seq->tete;
            seq->tete->autre = ctete; // on stock la valeur de la liste précédente
        }
        else if(c=='{'){
            ctete = seq->tete;
            ajout_en_tete(seq,'{',CROCHEO);
            cel = cellc->tab[--cellc->n];
            seq->tete->autre = ctete;
            seq->tete->suivant = cel->suivant;
        }
        else{
            stockagevaleur(seq,c);
        }
    }
}

/* La fonction retourne la cellule en tete en mettant c_>suivant == NULL*/
cellule_t* depilercellule(sequence_t* seq){
    cellule_t* c;
    c = seq->tete;
    seq->tete = seq->tete->suivant;
    return c;
}

void empilercellule(sequence_t* seq, cellule_t* c){
    c->suivant = seq->tete;
    seq->tete = c;
}


void afficher (sequence_t* seq)
{
    assert (seq); /* Le pointeur doit être valide */
    cellule_t *t = seq->tete;
    while(t!=NULL)
    {
        if (t==seq->tete)
            printf("-> ");
        
		if (t->type==NUM){
            printf("%d ",t->entier);
            t=t->suivant;
        }
        else if (t->command=='{') {
            printf("%c ",t->command);
            t = t->autre;
        }
		else{
  			printf("%c ",t->command);
            t=t->suivant;
        }
    }
}

// ### A voir avec le prof comment stocker les valeurs entiers ,
// faut-il créer une nouvelle type de sequences ?
