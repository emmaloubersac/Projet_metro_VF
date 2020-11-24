#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "aqrtopo.h"
#include "liste.h"
#include "ligne.h"
#include "abr.h"
#include "abr_type.h"
#include <math.h>
#define X_max 720
#define Y_max 540
#define ZOOM_X 240
#define ZOOM_Y 180
#define RAYON 2.5


void disque(int cx, int cy, int rayon, SDL_Renderer *renderer);
void ligneHorizontale(int x, int y, int w, SDL_Renderer *renderer);
void extraction_couleur_ligne(char *color,int *r,int *v,int *b);
void dessine_sta_con(int x_dep, int x_arr, int y_dep, int y_arr, int rayon, SDL_Renderer *renderer);
void affiche_sta_con(Un_elem *liste_sta, SDL_Renderer *renderer);

int main(int argc, char **agrv){

    SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

    //lancement SDL

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){     
		SDL_Log("ERREUR : Initialisation SDL > %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	} 
    
    //creation fenetre + rendu

    if (SDL_CreateWindowAndRenderer(X_max, Y_max, 0, &window, &renderer) != 0){
	    SDL_Log("ERREUR : creation fenetre et rendu echoue %s\n", SDL_GetError()); 
	    exit(EXIT_FAILURE);
    }


	//affichage d'un arrière-plan image (carte vierge de Paris)

	SDL_Surface *image = NULL, *image_zoom = NULL;    
	SDL_Texture *texture = NULL;

	image = SDL_LoadBMP("Projet_metro_VF/Arriere-plan paris.bmp");   //on charge l'image dans la surface
	
	if (image == NULL){   //on verifie que l'image a bien ete cree
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Log("ERREUR : ouverture image echoue\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}

	texture = SDL_CreateTextureFromSurface(renderer,image);  
	SDL_FreeSurface(image);   

	if (texture == NULL){   
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Log("ERREUR : creation texture echoue\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}
		

	SDL_Rect rect;   
	if (SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h) != 0){  
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Log("ERREUR : chargement memoire texture impossible\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}

	rect.x = (X_max - rect.w)/2;
	rect.y = (Y_max - rect.h)/2;

	if (SDL_RenderCopy(renderer,texture,NULL,&rect) != 0){ 
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Log("ERREUR : copie texture impossible\n", SDL_GetError()); 
		exit(EXIT_FAILURE);
	}

	SDL_RenderPresent(renderer); 

	//affichage des stations et connexions

	/*Un_elem *liste_sta = lire_stations("station2.csv");
	Un_nabr *abr = construire_abr(liste_sta);
	Une_ligne *liste_ligne = lire_lignes("ligne_metro.csv");
	afficher_lignes(liste_ligne);
	Un_elem *liste_con = lire_connexions("connexion.csv",liste_ligne, abr);
	Une_coord *l_no = NULL;
	l_no = (Une_coord*)malloc(sizeof(Une_coord));
	Une_coord *l_se = NULL;
	l_se = (Une_coord*)malloc(sizeof(Une_coord));

	limites_zone(liste_sta, l_no,l_se);

	Une_coord delta_max;
	delta_max.lat = l_se->lat - l_no->lat;
	delta_max.lon = l_se->lon - l_no->lon;
	printf(" delta_max.lon : %f\n", delta_max.lon);

	while (liste_con != NULL){
		int r = 0, v = 0, b = 0;
		int *p_r, *p_v, *p_b;
		p_r = &r;
		p_v = &v;
		p_b = &b;

		//determination de la couleur dans laquelle on va dessiner les stations et connexions 

		extraction_couleur_ligne(liste_con->truc->data.con.ligne->color,p_r,p_v,p_b);

		if (SDL_SetRenderDrawColor(renderer, r, v, b, SDL_ALPHA_OPAQUE) != 0){   //renderer le rendu ou l'on change la couleur, ensuite on a RRR/VVV/BBB puis l'opacite du trait (ici 100%, le trait nest pas transparent)
			SDL_Log("ERREUR : Impossible de changer couleur rendu\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		//determination des coords des stations

		Un_truc *sta_dep = chercher_sta(liste_sta,liste_con->truc->data.con.sta_dep->data.sta.nom);
		Un_truc *sta_arr = chercher_sta(liste_sta,liste_con->truc->data.con.sta_arr->data.sta.nom);
		int x_dep = floor((sta_dep->coord.lon - l_no->lon)*(X_max/delta_max.lon));
		int y_dep = floor((sta_dep->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

		int x_arr = floor((sta_arr->coord.lon - l_no->lon)*(X_max/delta_max.lon));
		int y_arr = floor((sta_arr->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

		dessine_sta_con(x_dep,x_arr,y_dep,y_arr,RAYON,renderer);

		liste_con = liste_con->suiv;
		SDL_RenderPresent(renderer);

	}*/
	
	Un_elem *liste_sta = lire_stations("station2.csv");
	affiche_sta_con(liste_sta,renderer);
	
    //gestion d'evenement 
	SDL_bool program_launched = SDL_TRUE;

	while(program_launched){
		SDL_Event event; 

		while(SDL_PollEvent(&event)){   
			switch (event.type)    //evenement quitter le programme
			{

			case SDL_MOUSEBUTTONDOWN:   //gestion evenemnt cliquer avec la souris
					printf("Click en : %d / %d\n", event.button.x, event.button.y);
					
					if(event.button.clicks >= 2)
						printf("double-click!\n");
						if (SDL_RenderClear(renderer) != 0){    //efface un rendu (pour pvr en afficher un nouveau apres pas ex) et test en mm temps si l'effacement a bien eu lieu, quitte sinon
							SDL_Log("ERREUR : effacement rendu echoue > %s\n", SDL_GetError()); 
							exit(EXIT_FAILURE);
						}

							Une_coord *l_no = NULL;
							l_no = (Une_coord*)malloc(sizeof(Une_coord));
							Une_coord *l_se = NULL;
							l_se = (Une_coord*)malloc(sizeof(Une_coord));

						limites_zone(liste_sta, l_no,l_se);

						Une_coord delta_max;
						delta_max.lat = l_se->lat - l_no->lat;
						delta_max.lon = l_se->lon - l_no->lon;
						printf(" delta_max.lat : %f\n", delta_max.lon);

						int pos_x = event.motion.x, pos_y = event.motion.y;
						Une_coord new_l_no, new_l_se;
						new_l_no.lon = (pos_x - ZOOM_X) / (X_max/delta_max.lon) + l_no->lon;
						new_l_no.lat = (pos_y + ZOOM_Y) / (Y_max/delta_max.lat) + l_se->lat;
						new_l_se.lon = (pos_x + ZOOM_X) / (X_max/delta_max.lon) + l_no->lon;
						new_l_se.lat = (pos_y - ZOOM_Y) / (Y_max/delta_max.lat) + l_se->lat;
						printf("%f",new_l_se.lon);

						if ((pos_x - ZOOM_X) < 0){
							new_l_no.lon = l_no->lon;
						}
						if ((pos_y - ZOOM_Y) < 0){
							new_l_se.lat = l_se->lat;
						}

						if (((pos_y + ZOOM_Y) / (Y_max/delta_max.lat)) > l_no->lat){
							new_l_no.lat = l_no->lat;
						}

						if (((pos_x + ZOOM_X) / (X_max/delta_max.lon)) > l_se->lon){
							new_l_se.lon = l_se->lon;
						}
						printf("bbbb");
						Un_noeud *aqr = construire_aqr(liste_sta);
						Un_elem *new_liste_sta = chercher_zone(aqr, new_liste_sta, new_l_no,new_l_se);
						printf("aaa");
						affiche_sta_con(new_liste_sta,renderer);

						SDL_RenderPresent(renderer);

						/*int pos_x = event.motion.x, pos_y = event.motion.y;
						Une_coord new_l_no, new_l_se;
						new_l_no.lon = (pos_x - ZOOM_X) / (X_max/delta_max.lon) + l_no->lon;
						new_l_no.lat = (pos_y + ZOOM_Y) / (Y_max/delta_max.lat) + l_no->lat;
						new_l_se.lon = (pos_x + ZOOM_X) / (X_max/delta_max.lon) + l_no->lon;
						new_l_se.lat = (pos_y - ZOOM_Y) / (Y_max/delta_max.lat) + l_no->lat;

						Un_noeud *aqr = construire_aqr(liste_sta);
						Un_elem *new_liste_sta = chercher_zone(aqr, liste_sta, new_l_no,new_l_se);

						while (liste_con != NULL){
							if (chercher_sta(new_liste_sta,liste_con->truc->data.con.sta_dep->data.sta.nom) != NULL){
								int r = 0, v = 0, b = 0;
								int *p_r, *p_v, *p_b;
								p_r = &r;
								p_v = &v;
								p_b = &b;

								extraction_couleur_ligne(liste_con->truc->data.con.ligne->color,p_r,p_v,p_b);

								if (SDL_SetRenderDrawColor(renderer, r, v, b, SDL_ALPHA_OPAQUE) != 0){   //renderer le rendu ou l'on change la couleur, ensuite on a RRR/VVV/BBB puis l'opacite du trait (ici 100%, le trait nest pas transparent)
									SDL_Log("ERREUR : Impossible de changer couleur rendu\n", SDL_GetError());
									exit(EXIT_FAILURE);
								}

								Un_truc *sta_dep = chercher_sta(new_liste_sta,liste_con->truc->data.con.sta_dep->data.sta.nom);
								Un_truc *sta_arr = chercher_sta(new_liste_sta,liste_con->truc->data.con.sta_arr->data.sta.nom);
								int x_dep = 3*floor((sta_dep->coord.lon - l_no->lon)*(X_max/delta_max.lon));
								int y_dep = 3*floor((sta_dep->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

								int x_arr = 3*floor((sta_arr->coord.lon - l_no->lon)*(X_max/delta_max.lon));
								int y_arr = 3*floor((sta_arr->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

								dessine_sta_con(x_dep,x_arr,y_dep,y_arr,RAYON,renderer);

								liste_con = liste_con->suiv;
								SDL_RenderPresent(renderer);
							}
							liste_con = liste_con->suiv;
						}


						pos_x = pos_x / (X_max/delta_max.lon) + l_no->lon;
						pos_y = pos_y / (Y_max/delta_max.lat) + l_no->lat;
						Une_coord coord_sta;
						coord_sta.lat = pos_x;
						coord_sta.lon = pos_y;
*/
						
					break;

					
			case SDL_QUIT:
				program_launched = SDL_FALSE;
				break;
			
			default:
				break;
			}

		}
	}

    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_VideoQuit(); //ne pas oublier de quitter en fct de ce qu'on a ouvert
	SDL_Quit();
    return EXIT_SUCCESS;
}


void extraction_couleur_ligne(char *color,int *r,int *v,int *b){
	char t_r[3] , t_v[3] , t_b[3] ;
	int i = 0, ind_v = 0, ind_b = 0;
	for ( i ; color[i] != ',' ; i++){
		t_r[i] = color[i];
	}
	i++;
	*r = atoi(t_r);
	for ( i ; color[i] != ',' ; i++){
		t_v[ind_v] = color[i];
		ind_v++;
	}
	i++;
	t_v[ind_v] = '\0';
	*v = atoi(t_v);
	for (i; color[i] != '\0' ; i++){
		t_b[ind_b] = color[i];
		ind_b++;
	}
	*b = atoi(t_b);
}

void ligneHorizontale(int x, int y, int w, SDL_Renderer *renderer)
{
  SDL_Rect r;
 
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;
 
  SDL_RenderFillRect(renderer, &r);
}

void disque(int cx, int cy, int rayon, SDL_Renderer *renderer)
{
  int d, y, x;
 
  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;
 
  while (y >= x) {
    ligneHorizontale(cx - x, cy - y, 2 * x + 1, renderer);
    ligneHorizontale(cx - x, cy + y, 2 * x + 1, renderer);
    ligneHorizontale(cx - y, cy - x, 2 * y + 1, renderer);
    ligneHorizontale(cx - y, cy + x, 2 * y + 1, renderer);
 
    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
 
    x++;
  }
}

void dessine_sta_con(int x_dep, int x_arr, int y_dep, int y_arr, int rayon, SDL_Renderer *renderer){
			//dessin des stations

		disque(x_dep,y_dep,RAYON,renderer);

		//dessin des connexions

		if (SDL_RenderDrawLine(renderer, x_dep, y_dep, x_arr, y_arr) != 0 && SDL_RenderDrawLine(renderer, x_dep+1, y_dep+1, x_arr+1, y_arr+1) != 0 && SDL_RenderDrawLine(renderer, x_dep-1, y_dep-1, x_arr-1, y_arr-1) != 0 && SDL_RenderDrawLine(renderer, x_dep+2, y_dep+2, x_arr+2, y_arr+2) != 0){   
			SDL_Log("ERREUR : Impossible de dessiner la ligne\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	
}

void affiche_sta_con(Un_elem *liste_sta, SDL_Renderer *renderer){
	
	Un_nabr *abr = construire_abr(liste_sta);
	Une_ligne *liste_ligne = lire_lignes("ligne_metro.csv");
	afficher_lignes(liste_ligne);
	Un_elem *liste_con = lire_connexions("connexion.csv",liste_ligne, abr);
	Une_coord *l_no = NULL;
	l_no = (Une_coord*)malloc(sizeof(Une_coord));
	Une_coord *l_se = NULL;
	l_se = (Une_coord*)malloc(sizeof(Une_coord));

	limites_zone(liste_sta, l_no,l_se);

	Une_coord delta_max;
	delta_max.lat = l_se->lat - l_no->lat;
	delta_max.lon = l_se->lon - l_no->lon;
	printf(" delta_max.lon : %f\n", delta_max.lon);

	while (liste_con != NULL){
		int r = 0, v = 0, b = 0;
		int *p_r, *p_v, *p_b;
		p_r = &r;
		p_v = &v;
		p_b = &b;

		//determination de la couleur dans laquelle on va dessiner les stations et connexions 

		extraction_couleur_ligne(liste_con->truc->data.con.ligne->color,p_r,p_v,p_b);

		if (SDL_SetRenderDrawColor(renderer, r, v, b, SDL_ALPHA_OPAQUE) != 0){   //renderer le rendu ou l'on change la couleur, ensuite on a RRR/VVV/BBB puis l'opacite du trait (ici 100%, le trait nest pas transparent)
			SDL_Log("ERREUR : Impossible de changer couleur rendu\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}

		//determination des coords des stations

		Un_truc *sta_dep = chercher_sta(liste_sta,liste_con->truc->data.con.sta_dep->data.sta.nom);
		Un_truc *sta_arr = chercher_sta(liste_sta,liste_con->truc->data.con.sta_arr->data.sta.nom);
		int x_dep = floor((sta_dep->coord.lon - l_no->lon)*(X_max/delta_max.lon));
		int y_dep = floor((sta_dep->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

		int x_arr = floor((sta_arr->coord.lon - l_no->lon)*(X_max/delta_max.lon));
		int y_arr = floor((sta_arr->coord.lat - l_no->lat)*(Y_max/delta_max.lat));

		dessine_sta_con(x_dep,x_arr,y_dep,y_arr,RAYON,renderer);

		liste_con = liste_con->suiv;
		SDL_RenderPresent(renderer);

	}
}
