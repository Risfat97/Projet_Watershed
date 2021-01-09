#include <stdio.h>
#include <stdlib.h>
#include "table_de_hachage.h"


maillon_t* creer_maillon(data_t donnee){
    maillon_t *m = (maillon_t*)malloc(sizeof(maillon_t));
    if(m == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour créer un maillon.\n");
        exit(EXIT_FAILURE);
    }
    m->donnee = donnee;
    m->next = NULL;
    return m;
}

liste* creer_liste_vide(){
    liste* l = (liste*)malloc(sizeof(liste));
    if(l == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour créer la liste.\n");
        exit(EXIT_FAILURE);
    }
    l->tete = NULL;
    l->taille = 0;
    return l;
}

void ajouter_dans_liste(liste* l, data_t donnee){
    maillon_t *m = creer_maillon(donnee);
    m->next = l->tete;
    l->tete = m;
    l->taille += 1;
}

uint8_t est_vide(liste* l){
    return l->taille == 0;
}

data_t obtenir_tete_liste(liste* l){
    maillon_t *tmp = l->tete;
    data_t ret = tmp->donnee;

    l->tete = l->tete->next;
    free(tmp);
    l->taille -= 1;
    return ret;
}

void detruire_liste(liste* l){
    while(!est_vide(l))
        obtenir_tete_liste(l);

    free(l);
}


table_hachage creer_table(){
    table_hachage th;
    uint32_t i;
    for(i = 0; i < TAILLE_TABLE_HACHAGE; i++)
        th.tab[i] = creer_liste_vide();
    return th;
}

uint8_t hacher(table_hachage th, data_t donnee){
    return donnee.cle % TAILLE_TABLE_HACHAGE;
}

void ajouter_dans_table(table_hachage th, uint8_t key, data_t donnee){
    ajouter_dans_liste(th.tab[key], donnee);
}

data_t obtenir_donnee(table_hachage th, uint8_t key){
    data_t ret = {-1, -1, 0};
    if(!est_vide(th.tab[key]))
        ret = obtenir_tete_liste(th.tab[key]);
    return ret;
}

data_t extraire_minimum_table(table_hachage th){
    uint32_t i = 0;
    data_t ret = {-1, -1, 0};
    while( i < TAILLE_TABLE_HACHAGE && est_vide(th.tab[i]) )
        i++;
    if(i < TAILLE_TABLE_HACHAGE)
        ret = obtenir_donnee(th, i);
    return ret;
}

void detruire_table(table_hachage th){
    uint32_t i;
    for(i = 0; i < TAILLE_TABLE_HACHAGE; i++)
        detruire_liste(th.tab[i]);
}