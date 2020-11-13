#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ligne.h"
#include "liste.h"
#define N 100
#define N_L 400


Une_ligne *lire_lignes(char *nom_fichier){
    FILE *fichier = NULL;
    char chaine[N] = "";
    char **tab_fichier = NULL;
    tab_fichier = (char**)malloc(N_L*sizeof(char*));

    if (tab_fichier == NULL){
        printf("erreur alloc1\n");
    }
    for (int i = 0; i < N_L; i++){
        tab_fichier[i] = (char*)malloc(N*sizeof(char));
        if (tab_fichier[i] == NULL){
            printf("erreur alloc2\n");
        }
    }

    int i = 0;

    fichier = fopen(nom_fichier, "r");   //mettre le fichier qu'on souhaite ouvrir a la place de station_test
    if (fichier  == NULL){
        printf("erreur ouverture fichier\n");
        return 0;
    }
    while (fgets(chaine,N,fichier) != NULL){
        strcpy(tab_fichier[i], chaine);
        i++;
    }
    tab_fichier[i][0] = '\0';

    fclose(fichier);

    int ind_ligne = 0;   //ligne du tableau du fichier
    Une_ligne *tete_liste = NULL;    //tete de liste a retourner
    tete_liste = (Une_ligne*)malloc(sizeof(Une_ligne));   
    if (tete_liste == NULL){
        printf("erreur alloc lire_lignes");
    }
    tete_liste->suiv = NULL;

    Une_ligne *temp = tete_liste;     //liste auxiliaire

    while( tab_fichier[ind_ligne][0] != '\0' ){
        temp->code = (char*)malloc(5*sizeof(char));    //on alloue la place necessaire
        temp->color = (char*)malloc(10*sizeof(char));

        int i = 0;
        char tab[5] = "";
        for ( i ; tab_fichier[ind_ligne][i] != ';' ; i++ ){
            tab[i] = tab_fichier[ind_ligne][i];
        }
        
        tab[i] = '\0';
        i++;
        strcpy(temp->code, tab);

        char temp_vit[5] = "";
        int k = 0;
        for(i; tab_fichier[ind_ligne][i] != ';' ; i++ ){
            temp_vit[k] = tab_fichier[ind_ligne][i];
            k++;
        }
        k++;
        i ++;
        temp_vit[k] = '\0';
        temp->vitesse = (float)atof(temp_vit); 
         

        char temp_interv[5] = "";
        int j = 0;
        for(i; tab_fichier[ind_ligne][i] != ';' ; i++ ){
            temp_interv[j] = tab_fichier[ind_ligne][i];
            j++;
        }
        j++;
        i++;
        temp->intervalle = (float)atof(temp_interv);
         

        char temp_couleur[10];
        int l = 0;
        for(i; tab_fichier[ind_ligne][i] != '\0' ; i++ ){
            temp_couleur[l] = tab_fichier[ind_ligne][i];
            l++;
        }
        l++;
        temp_couleur[l] = '\0';
        strcpy(temp->color, temp_couleur);
        temp->suiv = (Une_ligne*)malloc(sizeof(Une_ligne));
        temp = temp->suiv;
        temp->suiv = NULL;
        ind_ligne++;
        if (ind_ligne == 16)break;
    }
    
    return tete_liste;
}


void afficher_lignes(Une_ligne *lligne){
    if (lligne == NULL){
        printf("liste NULL");
    }
    Une_ligne *temp = lligne;
    
    while(temp->suiv!= NULL){
        printf("ligne : %s\t",temp->code);
        printf("ref color : %s\t",temp->color);
        printf("vitesse moyenne : %.2f\t",temp->vitesse);
        printf("interval entre 2 rames : %.2f\n",temp->intervalle);
        temp = temp->suiv;
    }
}


void detruire_lignes(Une_ligne *lligne){
    Une_ligne *tmp ;
    while(lligne!=NULL) 
    {
        tmp=lligne->suiv;
        free (lligne);
        lligne=tmp;
    }
    free(tmp);
}


Une_ligne *chercher_ligne(Une_ligne *lligne, char *code){
    Une_ligne *temp = lligne;
    afficher_lignes(lligne);
    while(temp->suiv != NULL){
        if ( strcmp(temp->code,code) == 0 ){
            return temp;
        }
        temp = temp->suiv;
    }
}
