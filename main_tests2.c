#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> //pour utiliser strncmp

#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20


int main(){
    
    int energie=10;
    srand(time(NULL));

    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    for (int i=0; i<NB_PREDATEURS; i++){
        int x = rand()%SIZE_X;
        int y = rand()%SIZE_Y;

        ajouter_animal(x,y, energie, &liste_predateur); 
    }

    for (int i=0; i<NB_PROIES; i++){
        int x = rand()%SIZE_X;
        int y = rand()%SIZE_Y;

        ajouter_animal(x,y, energie, &liste_proie); 
    }

    // printf("%d\n",compte_animal_rec(liste_predateur));
    // printf("%d\n",compte_animal_rec(liste_proie));

    // afficher_ecosys(liste_proie,liste_predateur);

    enlever_animal(&liste_predateur, liste_predateur->suivant);//enlève le deuxieme predateur de la liste
    enlever_animal(&liste_predateur, liste_predateur);

    enlever_animal(&liste_proie, liste_proie);
    enlever_animal(&liste_proie, liste_proie->suivant);
    enlever_animal(&liste_proie, (liste_proie->suivant)->suivant);

    afficher_ecosys(liste_proie,liste_predateur);


    void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie){
        FILE* f=fopen(nom_fichier, "w");
        if (f==NULL){
            printf("Erreur d'ouverture de %s\n", nom_fichier);
            return;
        }
        fprintf(f,"<proies>\n");
        Animal* courant=liste_proie;
        while (courant){
            fprintf(f, "x=%d y=%d dir=[%d %d] e=%.6f\n", courant->x, courant->y, courant->dir[0], courant->dir[1], courant->energie);
            courant=courant->suivant;
        }
        fprintf(f,"</proies>\n");
        fprintf(f,"<predateurs>\n");
        courant=liste_predateur;
        while (courant){
            fprintf(f, "x=%d y=%d dir=[%d %d] e=%.6f\n", courant->x, courant->y, courant->dir[0], courant->dir[1], courant->energie);
            courant=courant->suivant;
        }
        fprintf(f,"</predateurs>\n");
        fclose(f);
    }


    void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie){
        FILE* f=fopen(nom_fichier, "r");
        if (f==NULL){
            printf("Erreur d'ouverture de %s\n", nom_fichier);
            return;
        }
        char buffer[256];
        int x,y,dir[2];
        float e;
        Animal* courant;
        fgets(buffer,256,f);
        assert(strncmp(buffer,"<proies>",8)==0);
        fgets(buffer,256,f);


        while(strncmp(buffer,"</proies>",9)!=0){
            printf("PROIES %s", buffer);
            sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n", &x, &y, &dir[0], &dir[1], &e);
            courant=creer_animal(x,y,e);
            courant->dir[0]=dir[0];
            courant->dir[1]=dir[1];
            *liste_proie=ajouter_en_tete_animal(*liste_proie, courant);
            fgets(buffer,256,f);
        }
        fgets(buffer,256,f);
        assert(strncmp(buffer,"<predateurs>",12)==0);
        fgets(buffer,256,f);

        while(strncmp(buffer,"</predateurs>",13)!=0){
            printf("PREDATEURS %s", buffer);
            sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n", &x, &y, &dir[0], &dir[1], &e);
            courant=creer_animal(x,y,e);
            courant->dir[0]=dir[0];
            courant->dir[1]=dir[1];
            *liste_predateur=ajouter_en_tete_animal(*liste_predateur, courant);
            fgets(buffer,256,f);
        }

        fclose(f);
    }

	//Sauvegarde l'écosystème actuel dans testfichier.txt
    ecrire_ecosys("testfichier.txt", liste_predateur, liste_proie);


	//Creation de nouvelles listes pour recevoir la copie
	Animal* liste_predateur2=NULL;
	Animal* liste_proie2=NULL;
	//Lis l'écosystème tout juste sauvegardé
	lire_ecosys("testfichier.txt",&liste_predateur2, &liste_proie2);
	//Le réécris dans un autre fichier
	// ecrire_ecosys("testfichier2.txt", liste_predateur, liste_proie);

	//On peut comparer les deux fichiers pour voir si les fonctions ont bien lu / écrit.




    //libération des listes 
    liberer_liste_animaux(liste_predateur);
    liberer_liste_animaux(liste_predateur2);
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_proie2);


    return 0;
}