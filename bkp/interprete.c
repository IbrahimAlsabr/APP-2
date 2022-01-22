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

int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
    int entier = 0; //entier a utiliser en cas de besoin
    printf ("%d\n",sizeof(cellule_t*));
    

    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");
    // Ajouter par nous
    cellule_t *c = seq->tete;
    cellule_t *tmp; // ulilser dans le cas '{'
    cellule_t * prec = c;
    sequence_t * calcul = (sequence_t *)malloc(sizeof(sequence_t));
    //calcul->tete = (cellule_t*)malloc(sizeof(sequence_t));
    //calcul->tete->suivant=NULL;
    //cellule_t * cel_cal = calcul->tete; // cellule pour les calcules

    commande = c->command ; //à modifier: premiere commande de la sequence
    int ret = REUSSI;         //utilisée pour les valeurs de retour
    // ###on avait ret == REUSSI , a revoir en cas de problemes 
    while (c!=NULL) { //à modifier: condition de boucle
        if(c->type==NUM){
            ajout_en_tete(calcul,c->entier,NUM);
        }
        else{
            commande = c->command;
            switch (commande) {
                /* Ici on avance tout le temps, à compléter pour gérer d'autres commandes */
                case 'A':
                    ret = avance();
                    if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                    if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                    break; /* à ne jamais oublier !!! */

                case 'D':
                    droite();
                    break; /* à ne jamais oublier !!! */
                case 'G':
                    gauche();
                    break;
                case '+':
                    somme(calcul);
                    break;
                case '-':
                    diff(calcul);
                    break;
                case '*':
                    mult(calcul);
                    break;
                case 'M':
                    entier = depilernum(calcul,true);
                    entier = mesure(entier);
                    ajout_en_tete(calcul , entier, NUM);
                    break;
                case 'P':
                    entier = depilernum(calcul,true);
                    pose(entier);
                    break;
                case '{':
                    //tmp = depilercellule(seq);
                    //empilercellule(calcul,tmp);
                    break;
                case '?':
                    break;
                default:
                    eprintf("Caractère inconnu: '%c'\n", commande);
                    break;
            }
        }
        prec = c;
        c = c->suivant;
        printf("%c\n",commande);
        seq->tete = c;
        detruireCellule(prec);
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
