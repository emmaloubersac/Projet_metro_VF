#ifndef ABR_H
#define ABR_H

#include "liste.h"
#include "truc.h"
#include "abr_type.h"


Un_nabr *creer_nabr(Un_truc *truc);
void inserer_abr(Un_nabr *abr, Un_nabr *n);
Un_nabr *construire_abr(Un_elem *liste_sta);
void detruire_abr(Un_nabr *abr);
Un_truc *chercher_station(Un_nabr *abr,char *nom);
void test_abr(Un_nabr *abr);

#endif
