#include <stdio.h>
#include <stdlib.h>
#include "tas_min.h"

#define Parent(i) ((i-1)/2)
#define FilsGauche(i) 2*i+1
#define FilsDroit(i) 2*i+2 

tas_min creer_tas_min(){
    tas_min tas;
    tas.tab = (data_t*)calloc(10, sizeof(data_t));

    if(tas.tab == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour créer le tas.\n");
        exit(EXIT_FAILURE);
    }

    tas.taille_max = 10;
    tas.nbElements = 0;
    return tas;
}

uint8_t est_vide(tas_min tas){
    return tas.nbElements == 0;
}

data_t minimum_tas(tas_min tas){
    return tas.tab[0];
}

void inserer_dans_tas(data_t donnee, tas_min* tas){
    if(tas->nbElements >= tas->taille_max)
        agrandir_tas(tas);

    tas->tab[tas->nbElements++] = donnee;

    /* Réorganisation du tas */
    uint64_t indice = tas->nbElements - 1;
    data_t tmp;
    while(indice > 0 && tas->tab[Parent(indice)].cle > tas->tab[indice].cle){
        tmp = tas->tab[indice];
        tas->tab[indice] = tas->tab[Parent(indice)];
        tas->tab[Parent(indice)] = tmp;
        indice = Parent(indice);
    }
}

data_t extraire_minimum_tas(tas_min* tas){
    data_t tmp, result = minimum_tas(*tas);
    tas->tab[0] = tas->tab[tas->nbElements-1];
    tas->nbElements--;
    /* Réorganisation du tas */
    uint64_t indice = 1;
    while(indice < tas->nbElements){
        if((indice + 1) < tas->nbElements && tas->tab[indice].cle > tas->tab[indice + 1].cle)
            indice++;
        if(tas->tab[Parent(indice)].cle > tas->tab[indice].cle){
            tmp = tas->tab[indice];
            tas->tab[indice] = tas->tab[Parent(indice)];
            tas->tab[Parent(indice)] = tmp;
        }
        indice = FilsGauche(indice);
    }
    return result;
}

void agrandir_tas(tas_min* tas){
    uint64_t newSize = 1.5 * tas->taille_max;
    tas->tab = (data_t*) realloc(tas->tab, newSize * sizeof(data_t));
    if(tas->tab == NULL){
        fprintf(stderr, "Echec allocation mémoire, pour agrandir le tas.\n");
        exit(EXIT_FAILURE);
    }
    tas->taille_max = newSize;
}

void liberer_tas(tas_min tas){
    free(tas.tab);
    tas.tab = NULL;
}