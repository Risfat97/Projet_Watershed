#include <stdio.h>
#include <stdlib.h>
#include "table_de_hachage.h"


maillon_t* creer_maillon(uint32_t ligne, uint32_t colonne, uint8_t cle){
    maillon_t *m = (maillon_t*)malloc(sizeof(maillon_t));
    if(m == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour créer un maillon.\n");
        exit(EXIT_FAILURE);
    }
    m->cle = cle;
    m->colonne = colonne;
    m->ligne = ligne;
    m->next = NULL;
    return m;
}

liste* creer_liste_vide(){
    liste *l = (liste*)malloc(sizeof(liste));
    if(l == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour créer la liste.\n");
        exit(EXIT_FAILURE);
    }
    l->tete = NULL;
    l->taille = 0;
    return l;
}

void ajouter_dans_liste(liste* l, maillon_t* donnee){
    donnee->next = l->tete;
    l->tete = donnee;
    l->taille += 1;
}

uint8_t est_vide(liste* l){
    return l->taille == 0;
}

maillon_t* obtenir_tete_liste(liste* l){
    maillon_t *ret = l->tete;

    l->tete = l->tete->next;
    l->taille -= 1;
    return ret;
}

void detruire_liste(liste* l){
    while(!est_vide(l))
        free(obtenir_tete_liste(l));

    free(l);
}


void initialiser_table(table_hachage th){
    uint8_t i;
    for(i = 0; i < TAILLE_TABLE_HACHAGE; i++)
        th.tab[i] = creer_liste_vide();
}

uint8_t hacher(table_hachage th, maillon_t* donnee){
    return donnee->cle % TAILLE_TABLE_HACHAGE;
}

void ajouter_dans_table(table_hachage th, uint8_t key, maillon_t* donnee){
    ajouter_dans_liste(th.tab[key], donnee);
}

maillon_t* obtenir_donnee(table_hachage th, uint8_t key){
    return obtenir_tete_liste(th.tab[key]);
}

maillon_t* extraire_minimum_table(table_hachage th){
    uint8_t i = 0;
    while( i < TAILLE_TABLE_HACHAGE && est_vide(th.tab[i]) )
        i++;
    if(i < TAILLE_TABLE_HACHAGE)
        return obtenir_donnee(th, i);
    // Si la table est vide
    return NULL;
}

void detruire_table(table_hachage th){
    uint8_t i;
    for(i = 0; i < TAILLE_TABLE_HACHAGE; i++)
        detruire_liste(th.tab[i]);
}