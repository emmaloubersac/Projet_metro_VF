#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "liste.h"
#include "ligne.h"
#include "truc.h"
#include "ligne.h"


int main(){

    //test et affichage des lignes
    Une_ligne *ligne = NULL;
    ligne = lire_lignes("ligne_metro.csv");
    afficher_lignes(ligne);
    


    //test et affichage de station
    Un_elem *liste_sta;
    liste_sta = lire_stations("station2.csv");
    affiche_liste_truc_sta(liste_sta);



    //test et affichage des connexions et abr
    Un_elem *liste_con;
    Un_nabr *abr = construire_abr(liste_sta);
    liste_con = lire_connexions("connexion.csv", ligne,abr);
    affiche_liste_truc_con(liste_con);
    
    detruire_abr(abr);
    detruire_liste_et_truc(liste_sta);
    detruire_liste_et_truc(liste_con);
    detruire_lignes(ligne);
    return 0;
}