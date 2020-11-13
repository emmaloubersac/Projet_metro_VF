#ifndef AQRTOPO_H
#define AQRTOPO_H
#include "truc.h"
#include "liste.h"
#include "coord.h"

typedef struct _un_noeud { 
    Un_truc *truc; //Une station ou une connexion 
    Une_coord limite_no; //Limite zone
    Une_coord limite_se; //Limite zone
    struct _un_noeud *no; //Fils pour quart NO 
    struct _un_noeud *so; //Fils pour quart SO 
    struct _un_noeud *ne; //Fils pour quart NE 
    struct _un_noeud *se; //Fils pour quart SE 
}Un_noeud; 

Un_noeud* init_aqr();
Un_noeud* inserer_aqr(Un_noeud *aqr, Une_coord limite_no, Une_coord limite_se, Un_truc *truc);
Un_noeud *construire_aqr(Un_elem *liste);
Un_elem* extraction_liste_limite(Un_elem *liste, Une_coord limite_no, Une_coord limite_se);
/*void detruire_aqr(Un_noeud *aqr);
Un_truc *chercher_aqr(Un_noeud *aqr, Une_coord coord);
Un_elem *chercher_zone(Un_noeud *aqr, Un_elem *liste, Une_coord limite_no, Une_coord limite_se);*/

#endif