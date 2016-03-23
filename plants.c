#include "landscape.h"
#include "array.h"
#include "randpois.h"

int plants(double ***SCAPE,int Pa, int **P,int deltaP,int X,int flower,int *Sds){
    int i, j, n, m, rx[2], ry[2], z, q;
    for(i=0; i<Pa; i++){
        for(j=0; j<flower; j++){
            if(P[i][2+j]==2){                
                Sds[0]++;                
                rx[0] = P[i][0] - deltaP; 
                rx[1] = P[i][0] + deltaP; 
                ry[0] = P[i][1] - deltaP; 
                ry[1] = P[i][1] + deltaP; 
                n = floor((rx[1]-rx[0]+1)*randunif()) - deltaP;
                m = floor((ry[1]-ry[0]+1)*randunif()) - deltaP;        
                z = P[i][0] + n;
                q = P[i][1] + m;
                if(z < 0){
                    z = X - abs(n) + z;    
                }
                if(z >= X){
                    z = n - (z - X);    
                }
                if(q < 0){
                    q = X - abs(m) + q;
                }
                if(q >= X){
                    q = m - (q - X);    
                }
                if(SCAPE[z][q][1] < 1 && SCAPE[z][q][3] == 1){
                    SCAPE[z][q][1] = 1;
                    Sds[1]++;    
                }
            }
        }    
    }
    return EXIT_SUCCESS;
}
    
