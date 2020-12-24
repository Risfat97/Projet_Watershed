#ifndef __IMAGEPNG_H__
#define __IMAGEPNG_H__

#include "lodepng.h"

//Définition de la structure permettant de stocker les données relatives à l'image en noir et blanc
typedef struct{
    unsigned hauteur;   // hauteur de l'image
    unsigned largeur;   // largeur de l'image
    unsigned char** rouge;  // canal rouge de l'image
} imagepng;

/**
 * Initialise et retourne une imagepng vide avec une hauteur et une largeur à 0,
 * et le tableau 2d à NULL.
 */
imagepng initialise();

/**
 * Initialise une imagepng avec la hauteur et la largeur spécifiées,
 * le tableau 2d de dimension hauteur x largeur remplis de 0 puis 
 * retourne cette image.
 */
imagepng allouer_image(unsigned hauteur, unsigned largeur);

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

#endif