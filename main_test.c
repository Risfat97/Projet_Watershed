#include "imagepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int main(int argc, char *argv[]){
    assert(argc == 4);

    imagepng im = lire_image(argv[1]); //lecture de l'image en noir et blanc
    imagepng grd = calculer_gradient(im, 1);
    imagepng marq = lire_image(argv[2]); //lecture de l'image marqueur
    float temps;
    clock_t t1, t2;
 
    t1 = clock();
    calculerLPE(grd, marq);
    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps calculerLPE = %f\n", temps);

    mis_en_evidence(im, marq);
    ecrire_image_mis_en_evidence(im, marq, argv[3]);

    liberer_image(im);
    liberer_image(grd);
    liberer_image(marq);
    return EXIT_SUCCESS;
}