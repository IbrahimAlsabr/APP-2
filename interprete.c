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

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

void CalculerSomme(sequence_t *p)
{
    int x1 = depilerNombre(p,true);
    int x2 = depilerNombre(p,true);
    x1 = x1 + x2;
    ajout_en_tete(p , x1 , INTEGER); // le + '0' , c'est pour convertir le int x1  an char , code de '1' est 1 en ascii
}

void CalculerSoutraction(sequence_t *p)
{
    int x1 = depilerNombre(p,true);
    int x2 = depilerNombre(p,true);
    x1 = x2 - x1;
    ajout_en_tete(p , x1, INTEGER);
}

void CalculerMultiplication(sequence_t *p)
{
    int x1 = depilerNombre(p,true);
    int x2 = depilerNombre(p,true);
    x1 = x1 * x2;
    ajout_en_tete(p , x1, INTEGER);
}

void cmdInterro(sequence_t *pile , sequence_t* seq)
{
    cellule_t* n1 = depilerCroche(pile);
    cellule_t* n2 = depilerCroche(pile);
    int entier = depilerNombre(pile, true);
    if (entier == 0)
    {
        empSansCroches(seq, n1);
    }
    else
    {
        empSansCroches(seq, n2);
    }
}

void libererSuivant(sequence_t* seq , cellule_t* c , bool A_Detruire)
{
    if (A_Detruire)
    {
        cellule_t* prec;
        prec = seq->tete;
        seq->tete = seq->tete->suivant;
        detruireCellule(prec);
    }
    else
    {
        c = seq->tete;
    }
}

void echangerProg(sequence_t *pile)
{
    cellule_t* c1 = depilerCroche(pile);
    cellule_t* c2 = depilerCroche(pile);
    empilerCroche(pile,c1);
    empilerCroche(pile,c2);
}

void execProg(sequence_t *pile , sequence_t* seq)
{
    cellule_t* c = depilerCroche(pile);
    empSansCroches(seq,c);
}

bool commandeB(sequence_t *pile,sequence_t* seq)
{
    int n = pile->tete->entier;
    cellule_t* c = pile->tete->suivant;
    cellule_t* tmp;
    if(n > 0)
    {
        tmp = copierProg(c);
        tmp->autre->suivant = seq->tete;
        seq->tete = tmp;
        pile->tete->entier--;
        return false;
    }
    depilerNombre(pile,true);
    depilerCroche(pile); 
    return true;
}

int interprete (sequence_t* seq, bool debug)
{
    char commande;
    int entier = 0; 
    bool changerc = true;

    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:\n");
    afficher(seq);
    printf ("\n");
    //if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    // printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");

    cellule_t *c = seq->tete;
    sequence_t * calcul = (sequence_t *)malloc(sizeof(sequence_t));
    cellule_t* tmp; // POUR LES CROCHES OUVERTS

    commande = c->command ; //à modifier: premiere commande de la sequence
    int ret = REUSSI;         //utilisée pour les valeurs de retour

    while (c != NULL) 
    { 
        changerc = true;
        if(c->type == INTEGER)
        {
            ajout_en_tete(calcul,c->entier,INTEGER);
            libererSuivant(seq, c, true);
        }
        else {
            commande = c->command;
            switch (commande) 
            {
                case 'A':
                    ret = avance();
                    if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                    if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                    libererSuivant(seq,c,true);
                    break; /* à ne jamais oublier !!! */

                case 'D':
                    droite();
                    libererSuivant(seq,c,true);
                    break; 

                case 'G':
                    gauche();
                    libererSuivant(seq,c,true);
                    break;

                case '+':
                    CalculerSomme(calcul);
                    libererSuivant(seq,c,true);
                    break;

                case '-':
                    CalculerSoutraction(calcul);
                    libererSuivant(seq,c,true);
                    break;

                case '*':
                    CalculerMultiplication(calcul);
                    libererSuivant(seq,c,true);
                    break;

                case 'M':
                    entier = depilerNombre(calcul,true);
                    libererSuivant(seq,c,true);
                    entier = mesure(entier);
                    ajout_en_tete(calcul , entier, INTEGER);
                    break;

                case 'P':
                    entier = depilerNombre(calcul,true);
                    pose(entier);
                    libererSuivant(seq,c,true);
                    break;

                case '{':
                    tmp = depilerCroche(seq);
                    //c=tmp->autre->suivant; // on a garder le suivant pour pouvoir faire ca
                    empilerCroche(calcul,tmp);
                    break;

                case '?':
                    libererSuivant(seq,c,true);
                    cmdInterro(calcul,seq);
                    break;

                case 'X':
                    libererSuivant(seq,c,true);
                    echangerProg(calcul);
                    break;

                case '!':
                    libererSuivant(seq,c,true);
                    execProg(calcul,seq);
                    break;

                case 'B':
                    if(commandeB(calcul,seq)){ libererSuivant(seq,c,true); }
                    break;

                default:
                    eprintf("Caractère inconnu: '%c'\n", commande);
                    break;
            }
        }

        c = seq->tete;
        afficher(calcul);

        // /* Affichage pour faciliter le debug */
        // afficherCarte();
        // printf ("Programme:");
        // afficher(seq);
        // printf ("\n");
        // if (debug) stop();
    }
    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */
    return CIBLERATEE;
}
