#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
 
/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}


/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(liste_animal);
  Animal* newAnimal= creer_animal(x,y,energie);
  *liste_animal = ajouter_en_tete_animal(*liste_animal, newAnimal);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    Animal *courant = *liste;
    Animal *precedent = NULL;

    while (courant) {
        if (courant == animal) {
            if (precedent) {
                precedent->suivant = courant->suivant;
            } else {
                *liste = courant->suivant;
            }
            free(courant);
            break;
        }
        precedent = courant;
        courant = courant->suivant;
    }
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
  Animal* tmp = liste;
  while (tmp!=NULL) {
    Animal* next = tmp->suivant;
    free(tmp);
    tmp = next;
  }

  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, AjTTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      ecosys[i][j]=' ';
    }
  } 

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; j++) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; i++) {
    printf("|");
    for (j = 0; j < SIZE_Y; j++) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; j++) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la, float p_ch_dir) {
	while(la){
		la->x=(la->x+la->dir[0]+SIZE_X)%SIZE_X;
		la->y=(la->y+la->dir[1]+SIZE_Y)%SIZE_Y;

		if ((float)rand()/RAND_MAX<=p_ch_dir){
			la->dir[0]=(rand()%3)-1; 
			la->dir[1]=(rand()%3)-1;
		}
		la=la->suivant;
	}
}


/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
  Animal* courant = *liste_animal;
  while(courant){
    if((float)rand()/RAND_MAX<=p_reproduce){
      ajouter_animal(courant->x,courant->y,courant->energie/2.0,liste_animal);
      courant->energie/=2.0;
    }
    courant = courant->suivant;
  }
}


/* Part 2. Exercice 6(en fait 5), question 1 */
void rafraichir_proies(Animal **liste_proie, float p_ch_dir, float p_reproduce) {
  Animal* courant = *liste_proie;
  while(courant){
    bouger_animaux(courant,p_ch_dir);
    courant->energie-=1;
    if(courant->energie<0){ 
      enlever_animal(liste_proie,courant);
    } 
    courant = courant->suivant;
  }
  reproduce(liste_proie,p_reproduce);
}

/* Part 2. Exercice 7(en fait 6), question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
  Animal* courant=l;
  while(courant){
    if(courant->x==x &&courant->y==y){
      return courant;
    }
    courant = courant->suivant;
  }
  return NULL;
} 

/* Part 2. Exercice 7(en fait 6), question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie, float p_ch_dir, float p_reproduce) {
   Animal* courant = *liste_predateur;
  while(courant){
    bouger_animaux(courant,p_ch_dir);
    courant->energie-=1;
    Animal* tmp = animal_en_XY(*liste_proie, courant->x, courant->y); 
    
    if(tmp!=NULL){
      courant->energie+=tmp->energie;
      enlever_animal(liste_proie,tmp);
    } 
    
    if(courant->energie<0){  
      enlever_animal(liste_predateur,courant);
    } 
    courant = courant->suivant;
  }
  reproduce(liste_predateur,p_reproduce);
} 



/* Part 2. Exercice 5(en fait ex7), question 2 */

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
   /*A Completer*/
}

