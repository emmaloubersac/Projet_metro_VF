#ifndef STATION_H
#define STATION_H
#include "truc.h"

typedef struct _une_station{
    char *nom;   /*le nom*/
    struct _un_truc **tab_con;   /*tableau des connexions*/
    unsigned int nb_con;   /*Nb de connexions*/
    struct _un_truc *con_p_c;   /*Connexions du plus court chemin*/
}Une_station;

#endif