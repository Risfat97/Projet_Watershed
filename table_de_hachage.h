#ifndef __TABLE_HACHAGE__
#define __TABLE_HACHAGE__

#include <stdint.h>

#define TAILLE_TABLE_HACHAGE 256

//Structure représentant les données à stocker dans le maillon
typedef struct{
    int32_t ligne, colonne;
    uint8_t cle;
} data_t;

//Structure représentant un maillon
typedef struct maillon_s{
    data_t donnee;
    struct maillon_s *next;
} maillon_t;

//Définition de la structure de liste
typedef struct liste_t{
    maillon_t *tete;
    uint32_t taille;
} liste;

//Définition de la structure de la table de hachage
typedef struct {
    liste* tab[TAILLE_TABLE_HACHAGE];
} table_hachage;

/*****************************Prototype des fonctions de liste*****************************/

/**
 * Crée un maillon avec les éléments spécifiés
 */
maillon_t* creer_maillon(data_t donnee);

/**
 * Crée une liste vide et la retourne
 */
liste* creer_liste_vide();

/**
 * Ajoute la donnée spécifiée dans la liste.
 */
void ajouter_dans_liste(liste* l, data_t donnee);

/**
 * Vérifie si la liste est vide
 */
uint8_t est_vide(liste* l);

/**
 * Supprime et retourne la donnée en tête de liste 
 */
data_t obtenir_tete_liste(liste* l);

/**
 * Libère tout l'espace occupé par la liste 
 */
void detruire_liste(liste* l);


/*****************************Prototype des fonctions de map*****************************/

/**
 * Initialise la table de hachage en créant une liste vide pour chaque case du tableau
 */
table_hachage creer_table();

/**
 * Calcule la clé associée à la donnée spécifié pour son stockage
 */
uint8_t hacher(table_hachage th, data_t donnee);

/**
 * Ajoute la donnée spécifiée dans la table de hachage en tête de la liste
 * située à l'indice spécifié par la clé.
 */
void ajouter_dans_table(table_hachage th, uint8_t key, data_t donnee);

/**
 * Supprime et retourne la donnée en tête de liste se trouvant dans la table
 * de hachage à l'indice spécifié par la clé.
 */
data_t obtenir_donnee(table_hachage th, uint8_t key);

/**
 * Supprime et retourne l'element minimum dans la table 
 */
data_t extraire_minimum_table(table_hachage th);

/**
 * Libère tout l'espace occupé par la table de hachage
 */
void detruire_table(table_hachage th);


#endif