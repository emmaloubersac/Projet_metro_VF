
#include "liste.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100
#define N_L 400

Un_elem *inserer_deb_liste(Un_elem *liste, Un_truc *truc){
	Un_elem *res=malloc(sizeof(Un_elem));
	res->truc=truc;
	res->suiv=liste;
	return res;
}	


Un_elem *inserer_liste_trie(Un_elem *liste, Un_truc *truc){
	Un_elem *res=malloc(sizeof(Un_elem));
	res->truc=truc;
	if (truc->user_val<liste->truc->user_val){
		res->suiv=liste;
		return res;
    }
	Un_elem *temp=liste->suiv;
	Un_elem *prec=liste;
	while(temp!=NULL){
		if (truc->user_val<temp->truc->user_val){
			res->suiv=prec->suiv;
			prec->suiv=res;
            
			return liste;
		}
		prec=temp;
		temp=temp->suiv;
	}
    res->suiv = NULL;
	prec->suiv=res;
	return liste;
}


void ecrire_liste( FILE *flux, Un_elem *liste){
    Un_elem *temp = liste;
    while (temp != NULL){
        if (temp->truc->type == STA){
            printf("%s\n", temp->truc->data.sta.nom);
        }
        temp = temp->suiv;
    }
}

void detruire_liste(Un_elem *liste){
    Un_elem *tmp ;
    while(liste!=NULL) 
    {
        tmp=liste->suiv;
        free (liste);
        liste=tmp;
    }
}

void detruire_liste_et_truc(Un_elem *liste){
    Un_elem *temp = liste;
    while(temp->suiv != NULL){
        detruire_truc(liste->truc);
        temp = temp->suiv;
    }
    detruire_liste(liste);
}

Un_elem* lire_stations( char *nom_fichier){

    FILE *fichier = NULL;
    char chaine[N] = "";
    char **tab = NULL;
    tab = (char**)malloc(N_L*sizeof(char*));

    if (tab == NULL){
            printf("erreur alloc1\n");
    }
    for (int i = 0; i < N_L; i++){
        tab[i] = (char*)malloc(N*sizeof(char));
        if (tab[i] == NULL){
            printf("erreur alloc2\n");
        }
    }

    int i = 0;

    fichier = fopen(nom_fichier, "r");
    if (fichier  == NULL){
        printf("erreur ouverture fichier\n");
        return 0;
    }
    while (fgets(chaine,N,fichier) != NULL){
        strcpy(tab[i], chaine);
        i++;
    }
    tab[i][0] = '\0';
    
    int ind_lig = 0;   
                      //ind_l correspond a l'indice de la ligne du texte en argument (soit l'indice de la ligne du texte)
    
    Un_elem *liste = NULL;    //pointeur sur la tete de liste de retour
    liste = (Un_elem*)malloc(sizeof(Un_elem));
    liste->suiv = NULL;
    Un_elem * l_temp = liste;

    fclose(fichier);

    while (tab[ind_lig][0] != '\0' ){ //tant qu'on est pas sorti du fichier text
        Un_truc *p_truc;
        p_truc = (Un_truc*)malloc(sizeof(Un_truc));
        if (p_truc == NULL){
            printf("erreur alloc p_truc dans lire station");
        }
        p_truc->data.sta.nom = (char*)malloc(100*sizeof(char));

        
        l_temp->suiv = (Un_elem*)malloc(sizeof(Un_elem*));
        if (l_temp->suiv == NULL){
            printf("erreur alloc l_temp->suivant");
        }

        int i;   //compteur pour la ligne du texte

        char temp[100] = "";

        for ( i = 0; tab[ind_lig][i] != ';' ; i ++){   //debut de la ligne du texte, on copie dans le nom de la station du truc
            temp[i] = tab[ind_lig][i];
        }
        i++;
        //cas d'arret le ;
        temp[i] = '\0';   //on ajoute le marqueur de fin de chaine
        strcpy(p_truc->data.sta.nom, temp);

        char temp_lon[100] ="";
        

        if (tab[ind_lig][i] != ';' ){   //on s'occupe de la latitude
            
            int k = 0;
            for (int j = i; tab[ind_lig][j] != ';' ; j ++){
                temp_lon[k] = tab[ind_lig][j]; 
                k++;
                i = j;
            }
            i++;
            k++;
            temp_lon[k] = '\0';
            p_truc->coord.lat = (float)atof(temp_lon);

        }
        if (tab[ind_lig][i] == ';'){   // et finalement la longitude
            char temp_lon[100] = "";
            int k = 0;
            i++;
            for (int j = i; tab[ind_lig][j] != '\0' ; j ++){
                temp_lon[k] = tab[ind_lig][j]; 
                k++;
            }
            k++;
            temp_lon[k] = '\0';
            p_truc->coord.lon = (float)atof(temp_lon);
        }
        l_temp->truc = p_truc;   // on ajoute a temp est on passe a l'element suivant de la liste
        l_temp = l_temp->suiv;
        l_temp->suiv = NULL;
        ind_lig++;
    }
    return liste;
}

Un_elem *lire_connexions(char *nom_fichier, Une_ligne *liste_ligne, Un_nabr *abr_sta){
	FILE *fichier = NULL;
	char chaine[300] = "";
    char **tab = NULL;
    tab = (char**)malloc(1000*sizeof(char*));

    if (tab == NULL){
            printf("erreur alloc1\n");
    }

    for (int i = 0; i < 1000; i++){
        tab[i] = (char*)malloc(1000*sizeof(char));
        if (tab[i] == NULL){
            printf("erreur alloc2\n");
        }
    }
    fichier = fopen(nom_fichier, "r");
    if (fichier  == NULL){
        printf("erreur ouverture fichier\n");
        return 0;
    }
    int i=0;
    while (fgets(chaine,1000,fichier) != NULL){
    	strcpy(tab[i], chaine);
       	i++;
    }
    tab[i][0] = '\0';
    Un_elem *liste = NULL;
    
    for (i = 0; tab[i][0] != '\0' ; i++){
    	printf("%d\n",i);
        Un_truc *connexion1 = malloc(sizeof(Un_truc));
        Un_truc *connexion2 = malloc(sizeof(Un_truc));
        connexion1->type=1;
        connexion2->type=1;
        char temp[4][100];
        int j=0;
        int a=0;
        int b=0;
        while (tab[i][j]!='\n'){
        	if (tab[i][j]==';'){
        		temp[a][b]='\0';
        		a++;
        		b=0;
        	}
        	else{
        		temp[a][b]=tab[i][j];
        		b++;
        	}
        	j++;
        }
        temp[a][b]='\0';
		printf("%s   %s\n",temp[1],temp[2]);
        connexion1->data.con.sta_dep=chercher_station(abr_sta,temp[1]);
        connexion1->data.con.sta_arr=chercher_station(abr_sta,temp[2]);
        connexion1->data.con.ligne = chercher_ligne(liste_ligne,temp[0]);
        liste=inserer_deb_liste(liste,connexion1);
        connexion1->data.con.sta_dep->data.sta.tab_con[connexion1->data.con.sta_dep->data.sta.nb_con]=connexion1;
        connexion1->data.con.sta_dep->data.sta.nb_con++;
        
        connexion2->data.con.sta_dep=chercher_station(abr_sta,temp[2]);
        connexion2->data.con.sta_arr=chercher_station(abr_sta,temp[1]);
        connexion2->data.con.ligne = chercher_ligne(liste_ligne,temp[0]);
        liste=inserer_deb_liste(liste,connexion2);
        connexion2->data.con.sta_dep->data.sta.tab_con[connexion2->data.con.sta_dep->data.sta.nb_con]=connexion2;
        connexion2->data.con.sta_dep->data.sta.nb_con++;
    }
    return liste;
}

void limites_zone(Un_elem *liste, Une_coord *limite_no, Une_coord *limite_se){

    Un_elem *l_temp = liste;
    limite_no->lat = l_temp->truc->coord.lat;
    limite_no->lon = l_temp->truc->coord.lon;

    limite_se->lat = l_temp->truc->coord.lat;
    limite_se->lon = l_temp->truc->coord.lon;


    while (l_temp->suiv != NULL){
        if (l_temp->truc->coord.lat > limite_no->lat ){
            limite_no->lat = l_temp->truc->coord.lat;
        }
        if (l_temp->truc->coord.lon < limite_no->lon ){
            limite_no->lon = l_temp->truc->coord.lon;
        }
        if (l_temp->truc->coord.lat < limite_se->lat ){
            limite_se->lat = l_temp->truc->coord.lat;
        }
        if (l_temp->truc->coord.lon > limite_se->lon ){
            limite_se->lon = l_temp->truc->coord.lon;
        }
        l_temp = l_temp->suiv;
    }
}

void affiche_liste_truc_sta(Un_elem *liste){
    Un_elem *temp = liste;
    while(temp != NULL){
        printf("%s\t", temp->truc->data.sta.nom);
        printf("%f\t", temp->truc->coord.lat);
        printf("%f\n", temp->truc->coord.lon);
        temp = temp->suiv;
    }
}

void affiche_liste_truc_con(Un_elem *liste){
    Un_elem *temp = liste;
    while(temp != NULL){
        printf("sta_dep : %s\t", temp->truc->data.con.sta_dep->data.sta.nom);
        printf("sta_arr : %s\t", temp->truc->data.con.sta_arr->data.sta.nom);
        printf("ligne : %s\t", temp->truc->data.con.ligne->code);
        temp = temp->suiv;
    }
}

void affiche_truc(Un_truc *truc){
    printf("%s\t", truc->data.sta.nom);
    printf("%f\t", truc->coord.lat);
    printf("%f\n", truc->coord.lon);
}


