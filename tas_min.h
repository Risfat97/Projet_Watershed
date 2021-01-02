#ifndef __TAS_MIN_H__
#define __TAS_MIN_H__

#include <stdint.h>

// Structure représentant les éléments à stocker dans le tas
typedef struct {
    uint64_t ligne, colonne;
    uint8_t cle;
} data_t;

// Structure de tas implémenté via un tableau
typedef struct{
    uint64_t taille_max;
    uint64_t nbElements;
    data_t *tab;
} tas_min;

/**
 * Crée un tas vide avec une taille initiale de 10 et le retourne
 */
tas_min creer_tas_min();

/**
 * Vérifie si le tas est vide
 */
uint8_t est_vide(tas_min tas);

/**
 * Renvoie le minimum dans le tas 
 */
data_t minimum_tas(tas_min tas);

/**
 * Insére la donnée spécifiée dans le tas.
 */
void inserer_dans_tas(data_t donnee, tas_min* tas);

/**
 * Supprime et retourne l'element minimum dans le tas 
 */
data_t extraire_minimum_tas(tas_min* tas);

/** 
 * Agrandit le tas avec la taille actuelle du tas multiplié par 1.5 
 */
void agrandir_tas(tas_min* tas);

/**
 * Libère tout l'espace occupé par le tas 
 */
void liberer_tas(tas_min tas);

#endif