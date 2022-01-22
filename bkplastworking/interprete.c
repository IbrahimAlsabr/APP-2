#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"

#define DEBUG do{printf("I am here ...\n");}while(0)


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

void somme(sequence_t *p){
    int x1 = depilernum(p,true);
    int x2 = depilernum(p,true);
    x1 = x1 + x2;
    ajout_en_tete(p , x1 , NUM); // le + '0' , c'est pour convertir le int x1  an char , code de '1' est 1 en ascii
}

void diff(sequence_t *p){
    int x1 = depilernum(p,true);
    int x2 = depilernum(p,true);
    x1 = x2 - x1;
    ajout_en_tete(p , x1, NUM);
}

void mult(sequence_t *p){
    int x1 = depilernum(p,true);
    int x2 = depilernum(p,true);
    x1 = x1 * x2;
    ajout_en_tete(p , x1, NUM);
}


void cmdinterro(sequence_t *pile , sequence_t* seq){
    cellule_t* cF = depilercroche(pile);
    cellule_t* cV = depilercroche(pile);
    int entier = depilernum(pile,true);
    if (entier==0){
        empsanscroches(seq,cF);
    }
    else{
        empsanscroches(seq,cV);
    }
}

void vidersuiv(sequence_t* seq , cellule_t* c , bool detruire){
    if (detruire){
    cellule_t* prec;
    prec = seq->tete;
    seq->tete = seq->tete->suivant;
    detruireCellule(prec);
    }
    else{
    c = seq->tete;
    }
}

void echangeprog(sequence_t *pile){
    cellule_t* c1 = depilercroche(pile);
    cellule_t* c2 = depilercroche(pile);
    empilercroche(pile,c1);
    empilercroche(pile,c2);
}

void execprog(sequence_t *pile , sequence_t* seq){
    cellule_t* c = depilercroche(pile);
    empsanscroches(seq,c);
}

bool commandeB(sequence_t *pile,sequence_t* seq){
    int n = pile->tete->entier;
    cellule_t* c = pile->tete->suivant;
    cellule_t* tmp;
    if(n>0){
        tmp = copierprog(c);
        tmp->autre->suivant = seq->tete;
        seq->tete = tmp;
        pile->tete->entier--;
        return false;
    }
    depilernum(pile,true);
    depilercroche(pile); // ###il faut detruire la cellule !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return true;
}


int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
    int entier = 0; //entier a utiliser en cas de besoin
    bool changerc = true;
    

    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");
    // Ajouter par nous
    cellule_t *c = seq->tete;
    sequence_t * calcul = (sequence_t *)malloc(sizeof(sequence_t));
    cellule_t* tmp; // POUR LES CROCHES OUVERTS

    commande = c->command ; //à modifier: premiere commande de la sequence
    int ret = REUSSI;         //utilisée pour les valeurs de retour
    // ###on avait ret == REUSSI , a revoir en cas de problemes 
    while (c!=NULL) { //à modifier: condition de boucle
        changerc = true;
        if(c->type==NUM){
            ajout_en_tete(calcul,c->entier,NUM);
            vidersuiv(seq,c,true);
        }
        else{
            commande = c->command;
            switch (commande) {
                /* Ici on avance tout le temps, à compléter pour gérer d'autres commandes */
                case 'A':
                    ret = avance();
                    if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                    if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                    vidersuiv(seq,c,true);
                    break; /* à ne jamais oublier !!! */

                case 'D':
                    droite();
                    vidersuiv(seq,c,true);
                    break; /* à ne jamais oublier !!! */
                case 'G':
                    gauche();
                    vidersuiv(seq,c,true);
                    break;
                case '+':
                    somme(calcul);
                    vidersuiv(seq,c,true);
                    break;
                case '-':
                    diff(calcul);
                    vidersuiv(seq,c,true);
                    break;
                case '*':
                    mult(calcul);
                    vidersuiv(seq,c,true);
                    break;
                case 'M':
                    entier = depilernum(calcul,true);
                    vidersuiv(seq,c,true);
                    entier = mesure(entier);
                    ajout_en_tete(calcul , entier, NUM);
                    break;
                case 'P':
                    entier = depilernum(calcul,true);
                    pose(entier);
                    vidersuiv(seq,c,true);
                    break;
                case '{':
                    tmp = depilercroche(seq);
                    //c=tmp->autre->suivant; // on a garder le suivant pour pouvoir faire ca
                    empilercroche(calcul,tmp);
                    break;
                case '?':
                    vidersuiv(seq,c,true);
                    cmdinterro(calcul,seq);
                    break;
                case 'X':
                    vidersuiv(seq,c,true);
                    echangeprog(calcul);
                    break;
                case '!':
                    vidersuiv(seq,c,true);
                    execprog(calcul,seq);
                    break;
                case 'B':
                    if(commandeB(calcul,seq)){ vidersuiv(seq,c,true); }
                    break;
                default:
                    eprintf("Caractère inconnu: '%c'\n", commande);
                    break;
            }
        }
        c = seq->tete;
        printf ("Here is calcul : ");
        afficher(calcul);


        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
