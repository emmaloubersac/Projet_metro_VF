#include <stdio.h>
#include <stdlib.h>
#include "truc.h"

Un_truc *creer_truc(Une_coord coord, Ttype type, Tdata data, double uv){
    Un_truc *new_truc = NULL;
    new_truc = (Un_truc*)malloc(sizeof(Un_truc));
    new_truc->coord = coord;
    new_truc->data = data;
    new_truc->type = type;
    new_truc->user_val = uv;
    return new_truc;
} 

void detruire_truc(Un_truc *truc){
    if (truc->type == 0){
        free(truc->data.sta.nom);
        free(truc->data.sta.tab_con);
        free(truc);
    }
    else{
        free(truc);
    }
    return;
}