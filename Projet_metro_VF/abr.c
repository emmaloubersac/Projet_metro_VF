
#include "abr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


Un_nabr *creer_nabr(Un_truc *truc){
	Un_nabr *abr=malloc(sizeof(Un_nabr));
	abr->truc=truc;
	abr->g=NULL;
	abr->d=NULL;
	return abr;
}

void inserer_abr(Un_nabr *n, Un_nabr *abr){
	if (strcmp(abr->truc->data.sta.nom,n->truc->data.sta.nom)>0){
		if (abr->g==NULL)
			abr->g=n;
		else
			inserer_abr(n,abr->g);
	}
	else{
		if (abr->d==NULL)
			abr->d=n;
		else
			inserer_abr(n,abr->d);
	}
}

Un_nabr *construire_abr(Un_elem *liste_sta){
	Un_nabr *racine=creer_nabr(liste_sta->truc);
	liste_sta=liste_sta->suiv;
	while (liste_sta->suiv!=NULL){
		printf("%s\n",liste_sta->truc->data.sta.nom);
		Un_nabr *abr=creer_nabr(liste_sta->truc);
		inserer_abr(abr,racine);
		liste_sta=liste_sta->suiv;
	}
	return racine;
}

void detruire_abr(Un_nabr *abr){
	if (abr->d!=NULL)
		detruire_abr(abr->d);
	if (abr->g!=NULL)
		detruire_abr(abr->g);
	free(abr);
}

Un_truc *chercher_station(Un_nabr *abr,char *nom){
	if (strcmp(abr->truc->data.sta.nom,nom)==0)
		return abr->truc;
	if (abr->d==NULL && abr->g==NULL){
		printf("n'a pas trouvé\n");
		return 0;
	}
	if (strcmp(abr->truc->data.sta.nom,nom)<0)
		return chercher_station(abr->d,nom);
	else
		return chercher_station(abr->g,nom);
}

void test_abr(Un_nabr *abr){
	if (abr->g!=NULL)
		test_abr(abr->g);
	printf("%s:%f,%f\n",abr->truc->data.sta.nom,abr->truc->coord.lon,abr->truc->coord.lat);
	if (abr->d!=NULL)
		test_abr(abr->d);
}


	
	
	
	
	
	
	
	
	
