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
    cellule_t* tab[100];
    int n;
} cellchaine ;

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
    cellule_t *nv_tete = nouvelleCellule(); 
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
  cellule_t *nv = nouvelleCellule();
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
        else if(isalpha(c) || c=='?' || c=='!'){
            ajout_en_tete(seq,c,CMD);
        }
}

/*Cette fonction sert a attribuer ou prendre les pointeur des fin et ouvertures des 
crochets du vers le tableau de pointeurs */
void gerer_crocher_inv(cellchaine *cellc , cellule_t* c){
    if (c->type!=CROCHEF && c->type!=CROCHEO){
        printf("Erreur dans la la fontion gerer_crocher_inv / liste.c\n");
        return;
    }
    if(c->type==CROCHEO){
        c->autre=cellc->tab[--cellc->n];
    }
    else if(c->type==CROCHEF){
        cellc->tab[cellc->n++]=c; 
    }
}

void gerer_crocher(cellchaine *cellc , cellule_t* c){
    cellule_t* tmp ;
    if (c->type!=CROCHEF && c->type!=CROCHEO){
        printf("Erreur dans la la fontion gerer_crocher / liste.c\n");
        return;
    }
    if(c->type==CROCHEO){
        cellc->tab[cellc->n++]=c; 
    }
    else if(c->type==CROCHEF){
        tmp=cellc->tab[--cellc->n];
        tmp->autre=c;
    }
}

void conversion (char *texte, sequence_t *seq)
{
    int len = strlen(texte);
    cellchaine cellc;
    cellc.n = 0;
    cellule_t *ctete=NULL; // current tete
    char c = 0;
    for (int i = len-1; i >= 0; i--)
    {   
        c = texte[i];
        ctete = seq->tete;
        if(c=='}'){
            ajout_en_tete(seq,'}',CROCHEF);
            gerer_crocher_inv(&cellc,seq->tete);
        }
        else if(c=='{'){
            ajout_en_tete(seq,'{',CROCHEO);
            gerer_crocher_inv(&cellc,seq->tete);

        }
        else{
            stockagevaleur(seq,c);
    
        }
        if (ctete!= NULL && ctete->command=='}' && c!=' ' && c!='\n')
        {
            ctete->autre = seq->tete; // pour stocker le valeur precedent dans le autre de 
        }
        
    }
}

/* La fonction retourne la cellule en tete en mettant c_>suivant == NULL*/
cellule_t* depilercroche(sequence_t* seq){
    if (seq->tete==NULL){printf("seq tete NULL (fn depilercroche liste.c)\n");}
    cellule_t* c;
    cellule_t* fin;
    c = seq->tete;
    if(c->command!='{'){
        printf("Il y a un erreur ...\n");
    }
    if (c->autre==NULL){printf("c->autre est NULL (fn depilercroche liste.c)\n");}
    fin = c->autre;
    if (fin->suivant==NULL){printf("fin->suivant est NULL (fn depilercroche liste.c)\n");}
    seq->tete = fin->suivant;
    fin->suivant = NULL; //je vais pas faire ca pour pouvoir changer c en dehors
    return c;
}

void empilercroche(sequence_t* seq, cellule_t* c){
    cellule_t* fin = c->autre;
    fin->suivant = seq->tete;
    seq->tete = c; // the error is here !!
}

void empsanscroches(sequence_t *seq, cellule_t* celcroche){
    cellule_t* fin = celcroche->autre;
    cellule_t* debut= celcroche->suivant;
    detruireCellule(celcroche);
    if(debut!=fin){ 
    cellule_t* final = fin->autre;
    final->suivant = seq->tete;
    seq->tete=debut;
    } // fin de programme si crochet et vide '{}'
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
        }
		else
        {
  			printf("%c ",t->command);
        }
        t=t->suivant;
    }
}

void copiecel(cellule_t * dest,cellule_t * src){
    dest->type = src->type;
    dest->command = src->command;
    dest->entier = src->entier;
}

/* Cette fonction ne change pas la cel initiale , elle cree une copie en renvoyant une cellule
qui indique a ca fin en pointeur autre*/
cellule_t * copierprog(cellule_t * celcroche){
    cellule_t* debut= celcroche->suivant;
    cellule_t* fin = celcroche->autre;
    cellule_t* c = debut;
    cellule_t* nv;
    cellule_t* prec_nv = NULL;
    cellule_t* debut_nv;
    cellule_t* fin_nv;
    cellchaine cellc;
    while(c!=fin){
        nv = nouvelleCellule();
        copiecel(nv,c);
        if (prec_nv==NULL){
            debut_nv = nv;
        }
        else{
            prec_nv->suivant = nv;
        }
        if (nv->type==CROCHEO)
        {
            gerer_crocher(&cellc,nv);
        }
        else if(nv->type==CROCHEF){
            gerer_crocher(&cellc,nv);
            nv->autre = prec_nv;
        }
        prec_nv = nv;
        c = c->suivant;
    }
    fin_nv = nv;
    fin_nv->suivant = NULL;
    debut_nv->autre = fin_nv;
    return debut_nv;
}