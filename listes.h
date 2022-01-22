#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales 
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne 
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

enum{CMD , INTEGER , OPERATION, CROCHE_OUVERT, CROCHE_FERME};


struct cellule {
    char command;
    int entier;
    int type;
    struct cellule *autre;
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

struct tabChaine{
    cellule_t* tab[100];
    int n;
};
typedef struct tabChaine cellulesChainees ;

cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t* l);

void ajout_en_tete(sequence_t *liste, int n, int type);

void ajout_en_queue(sequence_t *seq, int n,int type);

void ajouterValeur (cellule_t * cel, int valeur, int type); 

int depilerNombre (sequence_t* seq, bool A_Detruire);

void stockageValeur(sequence_t* seq, char charLu);

void gererCrocherFin(cellulesChainees *cellc , cellule_t* charLu);

void gerer_crocher(cellulesChainees *cellc , cellule_t* charLu);

void conversion (char *texte, sequence_t *seq);

cellule_t* depilerCroche(sequence_t* seq);

void empilerCroche(sequence_t* seq, cellule_t* charLu);

void empSansCroches(sequence_t *seq, cellule_t* celCroche);

void afficher (sequence_t* seq);

void copierCellule(cellule_t * dest, cellule_t * src);  

cellule_t* copierProg(cellule_t * celCroche);

#endif
