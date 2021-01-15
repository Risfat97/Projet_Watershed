#include "imagepng.h"
#include "table_de_hachage.h"
#include <stdio.h>
#include <stdlib.h>

imagepng allouer_image(uint32_t hauteur, uint32_t largeur){
    imagepng im;    // l'imagepng à retourner
    uint32_t i, j;

    im.hauteur = hauteur;
    im.largeur = largeur;
    //Allocation mémoire du nombre de lignes du tableau 2d
    im.rouge = (uint8_t**)malloc(hauteur * sizeof(uint8_t*));
    if(!im.rouge){
        //Arrêt du programme avec un message d'erreur, si l'allocation échoue
        fprintf(stderr, "Echec allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < hauteur; i++){
        //Allocation mémoire du nombre de colonnes du tableau 2d et initialisation à 0
        im.rouge[i] = (uint8_t*)calloc(largeur, sizeof(uint8_t));
        if(!im.rouge[i]){
            //Arrêt du programme avec un message d'erreur, si l'allocation échoue
            for(j = 0; j < i; j++)
                free(im.rouge[j]); //libération mémoire des cases qui ont pu être allouées
            free(im.rouge);
            fprintf(stderr, "Echec allocation mémoire.\n");
            exit(EXIT_FAILURE);
        }
    }

    return im; 
}

imagepng lire_image(char* nom_fichier){
    imagepng im;
    uint32_t i, j;
    unsigned error;
    unsigned char* tab; // tableau unidimensionnel qui contiendra les valeurs de pixel de l'image
    unsigned largeur, hauteur;

    //decodage de l'image avec le nom spécifié en image brute RVBa 32 bits 
    error = lodepng_decode32_file(&tab, &largeur, &hauteur, nom_fichier);
    if(!error){
        /* s'il n'y a pas d'erreur, c'est à dire si error == 0, allocation mémoire
        de l'imagepng avec la hauteur et la largeur de l'image spécifiées. */
        im = allouer_image(hauteur, largeur);

        for(i = 0; i < hauteur; i++)
            for(j = 0; j < largeur; j++)
                // copie du niveau de rouge dans le tableau 2d
                im.rouge[i][j] = tab[4 * (i * largeur + j)];

        free(tab);
    } else{
        im.hauteur = im.largeur = 0;
        im.rouge = NULL;
    }

    return im;
}

void ecrire_image(imagepng im, char* nom_fichier){
    unsigned error;
    unsigned char* tab;  // tableau unidimensionnel qui contiendra les valeurs de pixel de l'image
    uint32_t i, j, taille = im.largeur * im.hauteur * 4;   // taille du tableau unidimensionnel

    tab = (unsigned char*)malloc(taille);
    if(!tab){
        fprintf(stderr, "Erreur, impossible d'écrire l'image dans le fichier\n");
        return;
    }
 
    for(i = 0; i < im.hauteur; i++)
        for(j = 0; j < im.largeur; j++){
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
        for(uint32_t i = 0; i < im.hauteur; i++)
            free(im.rouge[i]); //liberation mémoire de chaque ligne du tableau 2d
        free(im.rouge); //puis libération mémoire du tableau 2d
        im.rouge = NULL;
    }
}

imagepng calculer_gradient(imagepng im, uint32_t rayon){
    uint32_t i, j, k, l, debut_ligne, fin_ligne, debut_colonne, fin_colonne;
    uint8_t max, min;
    imagepng gradient = allouer_image(im.hauteur, im.largeur);

    //Parcours de chaque pixel de l'image pour calculer son gradient
    for(i = 0; i < im.hauteur; i++)
        for(j = 0; j < im.largeur; j++){
            debut_ligne = i - rayon;
            fin_ligne = i + rayon;
            debut_colonne = j - rayon;
            fin_colonne = j + rayon;
            max = min = im.rouge[i][j];
            //Recherche du max et du min des valeurs entourant le pixel im.rouge[i][j] selon le rayon
            for(k = debut_ligne; k <= fin_ligne; k++)
                for(l = debut_colonne; l <= fin_colonne; l++){
                    if((0 <= k && k < im.hauteur) && (0 <= l && l < im.largeur)){
                        if(max < im.rouge[k][l])
                            max = im.rouge[k][l];
                        else if(min > im.rouge[k][l])
                            min = im.rouge[k][l];
                    }
                }
            gradient.rouge[i][j] = max - min;
        }
        
    return gradient;
}

void calculerLPE(imagepng gradient, imagepng marqueur){
    table_hachage th; // th représente la structure de donnée pour faciliter le calcul de la LPE
    uint32_t i, j, k, l;
    uint8_t cle;
    data_t donnee, donnee_min;

    th = creer_table();
    
    for(i = 0; i < marqueur.hauteur; i++)
        for(j = 0; j < marqueur.largeur; j++)
            if(marqueur.rouge[i][j] != 0){
                donnee.ligne = i;
                donnee.colonne = j;
                donnee.cle = gradient.rouge[i][j];
                cle = hacher(th, donnee);
                ajouter_dans_table(th, cle, donnee);
            }

    do{
        donnee_min = extraire_minimum_table(th);
        if(donnee_min.ligne != -1)
            for(k = donnee_min.ligne-1; k <= donnee_min.ligne+1; k++)
                for(l = donnee_min.colonne-1; l <= donnee_min.colonne+1; l++)
                    if((0 <= k && k < gradient.hauteur) && (0 <= l && l < gradient.largeur)){
                        if(marqueur.rouge[k][l] == 0){
                            marqueur.rouge[k][l] = marqueur.rouge[donnee_min.ligne][donnee_min.colonne];
                            donnee.ligne = k;
                            donnee.colonne = l;
                            donnee.cle = gradient.rouge[k][l];
                            cle = hacher(th, donnee);
                            ajouter_dans_table(th, cle, donnee);
                        }
                    }
    } while(donnee_min.ligne != -1);

    detruire_table(th);  
}

void mis_en_evidence(imagepng im, imagepng im2){
    uint32_t i, j, k, l;
    for(i = 0; i < im.hauteur; i++)
        for(j = 0; j < im.largeur; j++)
            if(im2.rouge[i][j] == 100){
                for(k = i-1; k <= i+1; k++)
                    for(l = j-1; l <= j+1; l++)
                        if((0 <= k && k < im.hauteur) && (0 <= l && l < im.largeur) \
                            && (im2.rouge[k][l] == 200)){
                            im2.rouge[i][j] = 0;
                            k = i + 2; //pour sortir de la boucle de k
                            l = j + 2; //Pour sortir de la boucle de l
                        }
            }
}

void ecrire_image_mis_en_evidence(imagepng im, imagepng im2, char* nom_fichier){
    unsigned error;
    unsigned char* tab;  // tableau unidimensionnel qui contiendra les valeurs de pixel de l'image
    uint32_t taille = im.largeur * im.hauteur * 4;   // taille du tableau unidimensionnel
    uint32_t i, j;

    tab = (unsigned char*)malloc(taille);
    if(!tab){
        fprintf(stderr, "Erreur, impossible d'écrire l'image dans le fichier\n");
        return;
    }
 
    for(i = 0; i < im.hauteur; i++)
        for(j = 0; j < im.largeur; j++){
            //copie des pixels dans le tableau unidimensionnel
            if(im2.rouge[i][j] == 0){
                tab[(i * im.largeur + j) * 4] = 255; //niveau de rouge
                tab[(i * im.largeur + j) * 4 + 1] = 0; //niveau de vert
                tab[(i * im.largeur + j) * 4 + 2] = 0; //niveau de bleu
            }else{ 
                tab[(i * im.largeur + j) * 4] = im.rouge[i][j]; //niveau de rouge
                tab[(i * im.largeur + j) * 4 + 1] = im.rouge[i][j]; //niveau de vert
                tab[(i * im.largeur + j) * 4 + 2] = im.rouge[i][j]; //niveau de bleu
            }
            tab[(i * im.largeur + j) * 4 + 3] = 255;    //niveau alpha = 255   
        }

    //encodage à partir d'une image brute RVBa 32 bits dans le fichier avec le nom spécifié 
    error = lodepng_encode32_file(nom_fichier, tab, im.largeur, im.hauteur);
    if(error) 
        printf("error %u: %s\n", error, lodepng_error_text(error));

    free(tab); 
}