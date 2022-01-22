#include <stdlib.h>
#include <stdio.h>
#include "sequences.h"

/* Tri d'une sequence  : à implementer *

/* Affichage d'une sequence d'entiers.
  En  sortie : un espace entre deux entiers */
void afficher(sequence_t l) {
  cellule_t *c;
  if (l.tete==NULL) {
    printf("\n");}
  else {
    c = l.tete;
    while (c!=NULL) {
      printf(" %d",c->valeur);
      c = c->suivant;}
    printf("\n");}
return;}


/* Ajout en queue d'une sequence d'entiers implémentée par une liste chaînée */
void ajout_en_queue(sequence_t *l, int n) {
  cellule_t *c;
  c = l->tete;
  if (l->tete==NULL){
    l->tete= (cellule_t *) malloc(sizeof(cellule_t));
    c = l->tete;
    c->valeur = n;
    c->suivant=NULL;
  }
  else{
  cellule_t *nv = (cellule_t *) malloc(sizeof(cellule_t));
  nv->valeur = n;
  while(c->suivant!=NULL){
    c = c->suivant;
  }
  c->suivant = nv;
  nv->suivant=NULL;
  }
}

/* Ajout en tete d'une sequences d'entiers implémentée par une liste chaînée */
void ajout_en_tete(sequence_t *l, int n) {
    /* votre code ici */
    cellule_t *tete_ancien = l->tete;
    cellule_t *nv_tete = (cellule_t *) malloc(sizeof(cellule_t)); 
    nv_tete->suivant = tete_ancien;
    nv_tete->valeur = n;
    l->tete = nv_tete;
}

/* insertion dans une sequence triée */
void insertion(sequence_t *l, int n) {
  cellule_t *c;
  cellule_t *nv = creer_cellule(n);
  c = l->tete;
  if ((c==NULL) || c->valeur>= nv->valeur)
  {
    l->tete = nv;
    nv->suivant= c;
    c = nv;
  }
  else{
    while ((c->suivant!=NULL) && (c->suivant->valeur < nv->valeur)){ 
            c = c->suivant; 
        }
    nv->suivant= c->suivant;
    c->suivant = nv;
  }
  
  return;
}



/* Suppression des doublons consecutifs d'une sequence */
void suppression_doublons_consecutifs(sequence_t *l) {
  cellule_t *c;
  cellule_t *tmp;
  c = l->tete;
  while (c!=NULL) {
    if (c==l->tete) {
      tmp = c;
    }
    else{
      if (tmp->valeur==c->valeur) {
        tmp->suivant = c->suivant;
      }
      else{
        tmp=c;
      }
    }
    c= c->suivant;  
  }
return;}



/* Inversion d'une sequence */
void inversion(sequence_t *l) {
  cellule_t *c = l->tete;
  cellule_t *prec = NULL;
  cellule_t *suivant = NULL;

  while (c != NULL) {
      // Store suivant
      suivant = c->suivant;

      // Reverse current c's pointer
      c->suivant = prec;

      // Move pointers one position ahead.
      prec = c;
      c = suivant;
  }
  l->tete = prec;
return;}

