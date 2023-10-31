#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 400000


/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;


int main(void) {

  srand(time(NULL));

  //Crée les 2listes et ajoute une proie
  Animal* liste_proie_tmp=NULL;
  Animal* liste_proie=NULL;
  Animal* liste_predateur_tmp=NULL;
  Animal* liste_predateur=NULL;
  ajouter_animal(10,25,10.0,&liste_proie_tmp);

  //Affiche l'ecosys avant mvmt  
	afficher_ecosys(liste_proie_tmp, liste_predateur_tmp);
  printf("%d %d\n", liste_proie_tmp->x, liste_proie_tmp->y);

  //Bouge l'animal et affiche l'ecosys
	bouger_animaux(liste_proie_tmp, p_ch_dir);
  afficher_ecosys(liste_proie_tmp, liste_predateur_tmp);
  printf("x=%d y=%d\n", liste_proie_tmp->x, liste_proie_tmp->y);


  //Fait reproduire l'animal
  reproduce(&liste_proie_tmp, 1.0); //1ere fois
  afficher_ecosys(liste_proie_tmp, liste_predateur_tmp);
  reproduce(&liste_proie_tmp, 1.0); //2eme fois
  afficher_ecosys(liste_proie_tmp, liste_predateur_tmp); 

  //Cree une liste de NB_PROIES proies
  for (int i=0; i<NB_PROIES; i++){
    int x = rand()%SIZE_X;
    int y = rand()%SIZE_Y;
    ajouter_animal(x,y, 10, &liste_proie); 
  }

  //Cree une liste de NB_PREDATEURS predateurs
  for (int i=0; i<NB_PREDATEURS; i++){
    int x = rand()%SIZE_X;
    int y = rand()%SIZE_Y;
    ajouter_animal(x,y, 10, &liste_predateur); 
  }

  int cpt = 0; //Compteur pour le nombre d'iteration
  //Raffraichis les proies (deplacement, perte d'energie, reproduction)
  while(liste_proie && cpt<200 && liste_predateur){
    cpt++;
    rafraichir_proies(&liste_proie, p_ch_dir, p_reproduce_proie);
    rafraichir_proies(&liste_predateur, p_ch_dir, p_reproduce_predateur);
    afficher_ecosys(liste_proie, liste_predateur);
    usleep(T_WAIT); //Cree une pause de T_WAIT micro-secondes
  } 

  liberer_liste_animaux(liste_predateur);
	liberer_liste_animaux(liste_predateur_tmp);
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_proie_tmp);
  

  return 0;
}

