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

enum{CMD , NUM , OPR, CROCHEO , CROCHEF};


struct cellule {
    char command;
    /* vous pouvez rajouter d'autres champs ici */
    int entier;
    int type;// Creer par nous
    struct cellule *autre;
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t* l);

void ajout_en_queue(sequence_t *l, int n,int type);

void conversion (char *texte, sequence_t *seq);

cellule_t* depilercroche(sequence_t* seq);

void empilercroche(sequence_t* seq, cellule_t* c);

void empsanscroches(sequence_t *seq, cellule_t* celcroche);

void afficher (sequence_t* seq);

void ajout_en_tete(sequence_t *l, int n, int type);

int depilernum (sequence_t* seq, bool detruire);

cellule_t * copierprog(cellule_t * celcroche);

#endif
