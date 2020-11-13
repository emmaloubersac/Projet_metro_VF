
#ifndef LISTE_H
#define LISTE_H
#include "truc.h"
#include "abr.h"
#include "abr_type.h"
#include <stdio.h>


typedef struct _un_elem { 
    Un_truc *truc; //Une station ou une connexion 
    struct _un_elem *suiv;
}Un_elem; 



Un_elem *inserer_liste_trie(Un_elem *liste, Un_truc *truc);
void ecrire_liste( FILE *flux, Un_elem *liste);
void detruire_liste(Un_elem *liste);
void detruire_liste_et_truc(Un_elem *liste); 
Un_elem *lire_stations( char *nom_fichier); 
void limites_zone(Un_elem *liste, Une_coord *limite_no, Une_coord *limite_se);
void affiche_liste_truc_sta(Un_elem *liste);
void affiche_truc(Un_truc *truc);
Un_elem *lire_connexions(char *nom_fichier, Une_ligne *liste_ligne, Un_nabr *abr_sta);
Un_elem *inserer_deb_liste(Un_elem *liste, Un_truc *truc);
void affiche_liste_truc_con(Un_elem *liste);

#endif
