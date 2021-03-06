#ifndef __IMAGEPNG_H__
#define __IMAGEPNG_H__

#include "lodepng.h"
#include <stdint.h>

//Définition de la structure permettant de stocker les données relatives à l'image en noir et blanc
typedef struct{
    uint32_t hauteur;   // hauteur de l'image
    uint32_t largeur;   // largeur de l'image
    uint8_t **rouge;  // canal rouge de l'image
} imagepng;


/**
 * Initialise une imagepng avec la hauteur et la largeur spécifiées,
 * le tableau 2d de dimension hauteur x largeur remplis de 0 puis 
 * retourne cette image.
 */
imagepng allouer_image(uint32_t hauteur, uint32_t largeur);

/**
 * Lit une image PNG dont le nom est le nom spécifié, la stocke dans une structure imagepng
 * puis retourne cette image.
 */
imagepng lire_image(char* nom_fichier);

/**
 * Écrit l'imagepng spécifié dans le fichier dont le nom est le nom spécifié.
 */
void ecrire_image(imagepng im, char* nom_fichier);

/**
 * Libère de la mémoire l'imagepng spécifié
 */
void liberer_image(imagepng im);

/**
 * Calcule et envoie l'image de gradient de l'image spécifiée à l'aide 
 * d'un rayon spécifié.
 */
imagepng calculer_gradient(imagepng im, uint32_t rayon);

/**
 * Calcule et renvoie la ligne de partage des eaux calculée sur l'imagepng
 * gradient et l'imagepng marqueur.
 */
void calculerLPE(imagepng gradient, imagepng marqueur);

void mis_en_evidence(imagepng im, imagepng im2);

void ecrire_image_mis_en_evidence(imagepng im, imagepng im2, char* nom_fichier);

#endif