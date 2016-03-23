#include "as183.h"

/* Fills a vector, mvec, with random unifs */

double randunif(void){
    int seed[3];
    double randun;
    seed[0] = rand();
    seed[1] = rand();
    seed[2] = rand();
    randun  = as183(seed); 
    return randun;
}
