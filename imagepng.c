#include "imagepng.h"
#include <stdio.h>
#include <stdlib.h>


imagepng initialise(){
    imagepng im; // l'imagepng à retourner
    im.hauteur = im.largeur = 0;
    im.rouge = NULL;
    return im;
}

imagepng allouer_image(unsigned hauteur, unsigned largeur){
    imagepng im;    // l'imagepng à retourner
    im.hauteur = hauteur;
    im.largeur = largeur;

    if(hauteur == 0 || largeur == 0){   
        //Si la largeur ou la hauteur est égale à 0
        im = initialise();
    } else{
        //Allocation mémoire du nombre de lignes du tableau 2d
        im.rouge = (unsigned char**)malloc(hauteur * sizeof(unsigned char*));
        if(!im.rouge){
            //Arrêt du programme avec un message d'erreur, si l'allocation échoue
            fprintf(stderr, "Echec allocation mémoire.\n");
            exit(EXIT_FAILURE);
        }
        for(unsigned i = 0; i < hauteur; i++){
            //Allocation mémoire du nombre de colonnes du tableau 2d et initialisation à 0
            im.rouge[i] = (unsigned char*)calloc(largeur, sizeof(unsigned char));
            if(!im.rouge[i]){
                //Arrêt du programme avec un message d'erreur, si l'allocation échoue
                for(unsigned j = 0; j < i; j++)
                    free(im.rouge[j]); //libération mémoire des cases qui ont pu être allouées
                free(im.rouge);
                fprintf(stderr, "Echec allocation mémoire.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return im; 
}

imagepng lire_image(char* nom_fichier){
    imagepng im;
    unsigned error;
    unsigned char* tab; // tableau unidimensionnel qui contiendra les valeurs de pixel de l'image
    unsigned largeur, hauteur;

    //decodage de l'image avec le nom spécifié en image brute RVBa 32 bits 
    error = lodepng_decode32_file(&tab, &largeur, &hauteur, nom_fichier);
    if(!error){
        /* s'il n'y a pas d'erreur, c'est à dire si error == 0, allocation mémoire
        de l'imagepng avec la hauteur et la largeur de l'image spécifiées. */
        im = allouer_image(hauteur, largeur);

        for(unsigned i = 0; i < hauteur; i++)
            for(unsigned j = 0; j < largeur; j++)
                // copie du niveau de rouge dans le tableau 2d
                im.rouge[i][j] = tab[4 * (i * largeur + j)];

        free(tab);
    } else
        im = initialise();

    return im;
}

void ecrire_image(imagepng im, char* nom_fichier){
    unsigned error;
    unsigned char* tab;  // tableau unidimensionnel qui contiendra les valeurs de pixel de l'image
    int taille = im.largeur * im.hauteur * 4;   // taille du tableau unidimensionnel

    tab = (unsigned char*)malloc(taille);
    if(!tab){
        fprintf(stderr, "Erreur, impossible d'écrire l'image dans le fichier\n");
        return;
    }
 
    for(unsigned i = 0; i < im.hauteur; i++)
        for(unsigned j = 0; j < im.largeur; j++){
            //copie des pixels dans le tableau unidimensionnel
            tab[(i * im.largeur + j) * 4] = im.rouge[i][j]; //niveau de rouge
            tab[(i * im.largeur + j) * 4 + 1] = im.rouge[i][j]; //niveau de vert
            tab[(i * im.largeur + j) * 4 + 2] = im.rouge[i][j]; //niveau de bleu
            tab[(i * im.largeur + j) * 4 + 3] = 255;    //niveau alpha = 255
        }

    //encodage à partir d'une image brute RVBa 32 bits dans le fichier avec le nom spécifié 
    error = lodepng_encode32_file(nom_fichier, tab, im.largeur, im.hauteur);
    if(error) 
        printf("error %u: %s\n", error, lodepng_error_text(error));

    free(tab); 
}

void liberer_image(imagepng im){
    if(im.rouge){
        for(unsigned i = 0; i < im.hauteur; i++)
            free(im.rouge[i]); //liberation mémoire de chaque ligne du tableau 2d
        free(im.rouge); //puis libération mémoire du tableau 2d
        im.rouge = NULL;
    }
}