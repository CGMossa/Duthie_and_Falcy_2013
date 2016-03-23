#include "landscape.h"
#include "array.h"
#include "randunif.h"
#include "randpois.h"
#include "mutualists.h"
#include "exploiters.h"
#include "plants.h"

int main(void){

/*-------------------DEFINE----------------------------*/    
    FILE *fin;
    FILE *snap;
    FILE *Ppos;
    FILE *Mpos;
    FILE *Epos;
    FILE *Hpos;
    FILE *Po;
    FILE *Mo;
    FILE *Eo;
    
    int i, j, k, rep;
    int m, n, z;
    int M0, E0, P0, Ma, Ea, Pa, *Abs, *Sds;
    int X = 128;
    int L = X*X;
    int sim_count = 0;
    int scount = 100;
    int count;
    int iter;
    
    size_t x = X;

    double *RANK;
    double *LS;
    double delta_theta;
    double BETA; 
    double theta;
    double L_theta;
    double ***SCAPE;

    int deltaP;
    int deltaM;
    int deltaE; 
    int beta; 
    int epsiE; 
    int flower; 
    
    int Open;
    int Used;
    int Misd;
    int ExpU;

    double PlantMort;

    int **M;
    int **E;
    int **P;
    
    srand((unsigned)time(NULL));

    fin = fopen("Results.dat", "w");
    snap = fopen("Snapshot.dat","w");
    Ppos = fopen("Ppos.dat","w");
    Mpos = fopen("Mpos.dat","w");
    Epos = fopen("Epos.dat","w");
    Hpos = fopen("Hpos.dat","w");
    Po   = fopen("Po.dat","w"); 
    Mo   = fopen("Mo.dat","w"); 
    Eo   = fopen("Eo.dat","w"); 
    
    fprintf(fin,"BETA\t\titer\tP\tM\tE\tOpen\tUsed\tMisd\tExpU\tEstb\tSeeds\tMutV\n");
    fprintf(snap,"iter\tLtheta\tPlants\tMutualists\tExploiters\tOpen\tMisd\tUsed\tExpU\n");
    fprintf(Ppos,"Time\tPx\tPy\n");
    fprintf(Mpos,"Time\tMx\tMy\n");
    fprintf(Epos,"Time\tEx\tEy\n");

    printf("BETA\t\titer\tP\tM\tE\tOpen\tUsed\tMisd\tExpU\tEstb\tSeeds\tMutV\n");

    rep = 0;
    while(rep < 1){
    
        BETA =  -0.0; /* -0.5*floor(rep/100); */ 

        M0 = floor(0.19*X*X);     /*Default: floor(0.19*X*X); */
        E0 = floor(0.05*X*X);   /*Default: floor(0.05*X*X); */
        P0 = floor(0.30*X*X);    /*Default: floor(0.30*X*X); */
        Ma = M0;
        Ea = E0;
        Pa = P0;    
        delta_theta = 0.0;
        theta = 0.3;        /* Proportion of habitable cells */
        L_theta = 1.0;      /* Probability ovulation in habitable cell -- Default: 1 */
        deltaP = 2;         /* Default: 2, X/2 */
        deltaM = 2;         /* Default: 2, X/2 */
        deltaE = 2;         /* Default: 2, X/2 */
        beta   = 6;         /* Default: 6 */
        epsiE  = 15;        /* Default: 15 */
        flower = 1;         /* Default: 1  */
        PlantMort = 0.02;   /* Default 0.02 */
    
        LS = (double*) malloc(sizeof(double)*L);

        MAKE_1ARRAY(Sds,3);
        MAKE_1ARRAY(RANK,L);
        MAKE_3ARRAY(SCAPE,x,x,4);
        MAKE_1ARRAY(Abs,3);
        Abs[1] = M0;
        Abs[2] = E0;

        /*------------------/DEFINE-----------------------------*/    

        /*-------------------LANDSCAPE--------------------------*/    
    
        /* The landscape LS will be created below */
        landscape(LS,BETA,x);

        /* Use the top (theta for oviposition) */
        for(i=0; i<L; i++){
            for(j=0; j<L;j++){
                if(LS[i] > LS[j]){
                    RANK[i]++;
                }    
            }
        }        

        for(i=0; i<L; i++){
            if(RANK[i] > (1-theta)*L){
                LS[i] = 1;
            }else{
                LS[i] = 0;
            }
        }    

        FREE_1ARRAY(RANK);

        /*}*/ /*Note: if BETA == 2, homogenous landscape */    
        /* With the landscape created, rewrite LS as array */
        if(BETA==2){
            for(i=0; i<L; i++){
                LS[i] = 1;    
            }    
        }
    
        for(i=0; i<X; i++){
            for(j=0; j<X; j++){
                SCAPE[i][j][0] = 0;
                SCAPE[i][j][1] = 0;
                SCAPE[i][j][2] = 0;
                SCAPE[i][j][3] = LS[(X*i)+j];
            }
        }

        /* BELOW RECORDS THE POSITION OF HABITAT FOR A PARTICULAR TIME STEP */
        /* ************************************************************ */
        for(i=0; i<X; i++){
            for(j=0; j<X; j++){
                if(SCAPE[i][j][3]==1){
                    fprintf(Hpos,"%d\t%d\n",i,j);
                }
            }
        }
        /* ************************************************************ */

        free(LS);

        /*------------------/LANDSCAPE--------------------------*/    

        /*-------------------SET INDIVIDUALS--------------------*/
        MAKE_2ARRAY(M,M0,2); /* Set Mutualists */
        for(i=0; i<M0; i++){
            M[i][0] = floor(X*randunif() * 1);
            M[i][1] = floor(X*randunif() * 1);
        }

        /*  Code below places exploiters */
        MAKE_2ARRAY(E,E0,2); 
        for(i=0; i<E0; i++){
            E[i][0] = floor(X*randunif() * 1);
            E[i][1] = floor(X*randunif() * 1);
        }
    
        MAKE_2ARRAY(P,P0,(2+flower)); /* Set Plants */
        i = 0; /*Last 15 columns are for flowers */
        while(i < P0){
            m = floor(X*randunif() * 1);
            n = floor(X*randunif() * 1);
            if(SCAPE[m][n][1]==0){
                P[i][0] = m;     
                P[i][1] = n;    
                SCAPE[m][n][1] = i;    
                for(j=2; j < (2+flower); j++){
                    P[i][j] = 0;
                }
                i++;
                Abs[0] = i; /*Gets the abundance of plants */
            }
        } /* Note: plants only occupy 1 cell */    

        /*Below starts a large while loop for 1 sim*/
        /*-------------BEGIN THE SIMULATION-------------------------------*/        
        iter = 0; /* iter referes to the time step */
        while(iter < 1000){    /* Can also use below for other conditions */
            /*(Abs[2] > 0  Abs[1] > 0 && Abs[0] > 0 && L_theta >= 0)*/ /*||*/ /* &&*/
            sim_count = sim_count + 1;             

            if(sim_count==scount){
                L_theta = L_theta + delta_theta;
                sim_count = 0; /*printf("%f\t",L_theta);*/
            }         
                
            /*-----------FLOWERS OVULATE BELOW-------------------------*/
            
            for(i=0; i<Pa; i++){
                m = P[i][0]; 
                n = P[i][1]; 
                for(j=2; j<(flower+2); j++){
                    if(randunif() < L_theta){
                        P[i][j] = 1; 
                        /* if(P[i][0] == 1 || iter == 999){
                            fprintf(Ppos,"%d\t%d\t%d\n",iter,m,n);        
                        }  */
                    }
                }
            } 
        
            /*----------\FLOWERS OVULATE BELOW-------------------------*/

            /* BELOW RECORDS PLANT POSITIONS FOR A PARTICULAR TIME STEP */        
            /* ************************************************************ */
            if(iter == 999){
                for(i=0; i<Pa; i++){
                    if(P[i][2] == 1){ 
                        fprintf(Ppos,"%d\t%d\t%d\n",iter,P[i][0],P[i][1]);
                    }        
                }
            }
            /* ************************************************************ */

            Sds[2] = 0;
            mutualists(SCAPE, Abs[1], M, P, beta, deltaM, X, flower, Sds, iter);

            /* BELOW RECORDS POLLINATOR POSITIONS FOR A PARTICULAR TIME STEP */
            /* ************************************************************ */
            for(i=0; i<Ma; i++){
                if(M[i][0] == 1 || iter == 999){
                    fprintf(Mpos,"%d\t%d\t%d\n",iter,M[i][0],M[i][1]);
                }    
            }
            /* ************************************************************ */        

            /* # OF POLLINATED, USED (OVIPOSITED), AND UNPOLLINATED PLANTS */
            /* ************************************************************ */
            if(iter == 999){ 
                Open = 0; 
                Used = 0;  
                Misd = 0;  
                for(i=0; i<Pa; i++){
                    if(P[i][2]==2){
                        Open++;
                        fprintf(Po,"%d\t%d\n",P[i][0],P[i][1]);
                    }
                    if(P[i][2]==1){
                        Misd++;    
                    }    
                    if(P[i][2]==3){
                        Used++;
                        fprintf(Mo,"%d\t%d\n",P[i][0],P[i][1]);                
                    }
                }
            }    
            /* ************************************************************ */

            exploiters(SCAPE, Abs[2], E, P, epsiE, deltaE, X, flower); 

            /* NUMBER OF EXPLOITED FLOWERS FOR A PARTICULAR TIME STEP */        
            /* ************************************************************ */
            if(iter == 999){
                ExpU = 0;
                for(i=0; i<Pa; i++){
                    if(P[i][2]==4){
                        ExpU++;
                        fprintf(Eo,"%d\t%d\n",P[i][0],P[i][1]);
                    }
                }
            }
            /* ************************************************************ */

            /* BELOW RECORDS THE POSITION OF EXPLOITERS FOR A PARTICULAR TIME STEP */
            /* ************************************************************ */
            for(i=0; i<Ea; i++){
                if(E[i][0] == 1 || iter == 999){
                    fprintf(Epos,"%d\t%d\t%d\n",iter,E[i][0],E[i][1]);
                }
            }
            /* ************************************************************ */

            Sds[0] = 0;
            Sds[1] = 0;

            plants(SCAPE, Abs[0], P, deltaP, X, flower, Sds);

            FREE_2ARRAY(P);
            FREE_2ARRAY(M);
            FREE_2ARRAY(E);
    
            /*-------------REMAKE THE MATRICES--------------------------*/    
            Pa = 0;
            Ea = 0;
            Ma = 0;
            /* Below remakes the matrix P based on the landscape */
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][1] > 0){
                        Pa++;
                        SCAPE[i][j][1] = Pa;
                    }
                }    
            }
            MAKE_2ARRAY(P,Pa+1,(2+flower));
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][1] > 0){
                        z = SCAPE[i][j][1]; 
                        P[z-1][0] = i;
                        P[z-1][1] = j;
                        for(k=2; k<(2+flower); k++){
                            P[z-1][k] = 0;
                        }
                    }
                }    
            }
            /* Below remakes the matrix M based on the landscape */
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][0] > 0){
                        z = SCAPE[i][j][0];
                        Ma = Ma + z;
                    }
                }    
            }
            MAKE_2ARRAY(M,Ma+1,2);
            count = 0;
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][0] > 0){
                        z = SCAPE[i][j][0];
                        for(k=count; k<(z+count); k++){
                            M[k][0] = i;
                            M[k][1] = j;            
                        }
                        count = count + z;
                    }
                }    
            }  
            /* Below remakes the matrix E based on the landscape */
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][2] > 0){
                        z = SCAPE[i][j][2];
                       Ea = Ea + z;
                    }
                }    
            }
            MAKE_2ARRAY(E,Ea+1,2);
            count = 0;
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    if(SCAPE[i][j][2] > 0){
                        z = SCAPE[i][j][2];
                        for(k=count; k<(z+count); k++){
                            E[k][0] = i;
                            E[k][1] = j;            
                        }
                        count = count + z;
                    }
                }    
            }     
            /*-------------PLANT MORTALITY-------------------------------*/    
            if(iter > 0){
                for(i=0; i<X; i++){
                    for(j=0; j<X; j++){
                        if(SCAPE[i][j][1] > 0){
                            if(randunif() < PlantMort){
                                SCAPE[i][j][1] = 0;
                            }
                        }    
                    }
                }
            }
            /*------------\PLANT MORTALITY-------------------------------*/    
            Abs[0] = Pa; 
            Abs[1] = Ma;
            Abs[2] = Ea; 
            
            iter++; 
    
            printf("%d\t%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                iter,L_theta,Abs[0],Abs[1],Abs[2],Misd,ExpU,Sds[0],Sds[1],Sds[2]);
    
            fprintf(snap,"%d\t%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                iter,L_theta,Abs[0],Abs[1],Abs[2],Open,Misd,Used,ExpU); 
    
            for(i=0; i<X; i++){
                for(j=0; j<X; j++){
                    SCAPE[i][j][0] = 0;
                    SCAPE[i][j][2] = 0;
                }
            }
        }
        /*-------------\REMAKE THE MATRICES--------------------------*/        
        sim_count = 0; 
        printf("%f\t%d\t%d\t%d\t%d\t",BETA,iter,Abs[0],Abs[1],Abs[2]);
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",Open,Used,Misd,ExpU,Sds[0],Sds[1],Sds[2]);
        rep++; 
        fprintf(fin,"%f\t%d\t%d\t%d\t%d\t",BETA,iter,Abs[0],Abs[1],Abs[2]);
        fprintf(fin,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",Open,Used,Misd,ExpU,Sds[0],Sds[1],Sds[2]);
        FREE_3ARRAY(SCAPE);
        FREE_1ARRAY(Abs);
        FREE_1ARRAY(Sds);
    }
    return 0; 
}



