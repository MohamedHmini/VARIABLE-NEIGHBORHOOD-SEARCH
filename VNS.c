#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "VNS.h"
#include "HLCio/dataframe.h"



// the global VNS_CONFIG variable :

VNS_CONFIG vns_config;



// neighborhood structures array generator : 

NEIGHBORHOOD_STRUCTURES neistructs(int size){
    NEIGHBORHOOD_STRUCTURES Ns = malloc(sizeof(NEIGHBORHOOD_STRUCT) * size);
    return Ns;
}




// NEIGHBORHOOD STRUCTURES : 


POLICY* __block_swapping(POLICY x, int block_size, int swap_factor, int i){

    int lb = i+block_size + swap_factor;
    int ub = i+2*block_size + swap_factor;

    POLICY* nx = malloc(sizeof(DF_ELEMENT));
    *nx = df_element_copy(x);

    // swapping : 
    for(int j = 0;j<block_size;j++){
        int tmp;
        tmp = nx->node.Arr->data[i + j].node.Int;
        nx->node.Arr->data[i + j].node.Int = nx->node.Arr->data[lb + j].node.Int;
        nx->node.Arr->data[lb + j].node.Int = tmp;
    }

    return nx;
}


POLICY* __block_reversing(POLICY x, int block_size, int i){

    POLICY* nx = df_element_create();
    *nx = df_element_copy(x);

    // reversing : 
    for(int j = 0;j< (int)((block_size-2)/2);j++){
        int tmp;
        int first_element = 1 + j;
        int second_element = block_size - 2 - j;
        tmp = nx->node.Arr->data[i + first_element].node.Int;
        nx->node.Arr->data[i + first_element].node.Int = nx->node.Arr->data[i + second_element].node.Int;
        nx->node.Arr->data[i + second_element].node.Int = tmp;
    }

    return nx;
}



POLICY* block_swapping(POLICY x, int block_size, int swap_factor,  int i){
    int bound = x.node.Arr->size - (2*block_size + swap_factor) + 1;
    POLICY* nx = NULL;

    if(i == -1){
        srand(time(NULL));
        i = randint(0, bound);
    }
       
    if(i < bound)
        nx = __block_swapping(x, block_size, swap_factor, i);

    return nx;
}


POLICY* block_reversing(POLICY x, int block_size,  int i){
    int bound = x.node.Arr->size - block_size + 1;
    POLICY* nx = NULL;
    
    if(i == -1){
        srand(time(NULL));
        i = randint(0, bound);
    }
    
    if(i < bound)
        nx = __block_reversing(x, block_size, i);

    return nx;
}



// ESSENTIAL FUNCTIONS : 

LOCAL_SEARCH_RESULT change_neighborhood(POLICY x, POLICY bx, int k){
    LOCAL_SEARCH_RESULT lsr;
    CMP_RESULT cmp_r = vns_config.cmp_optimality(x, bx, 0, 1);

    if(cmp_r.index == 0){
        k = 0;
    }
    else{
        k++;
    }

    lsr.k = k;
    lsr.bx = df_element_copy(cmp_r.best);
    arrfree(&cmp_r.best);
    
    return lsr;
}

// DETERMINISTIC LOCAL SEARCH : 
POLICY best_improvement(POLICY bx, NEIGHBORHOOD_STRUCT nei_struct){
    POLICY bxc = df_element_copy(bx);
    POLICY *x = NULL;
    CMP_RESULT cr;

    // local search exploitation : 
    do{
        POLICY started_with = df_element_copy(bxc);
        int i = 0;

        while((x = nei_struct(bxc,i)) != NULL){  
            // arrshow(x);  
            cr = vns_config.cmp_optimality(*x, bxc, 0, 1); 

            if(cr.index == 0){
                arrfree(&bxc);
                bxc = df_element_copy(*x);
            }

            arrfree(&cr.best);
            arrfree(x);
            free(x);
            x = NULL;
            i++;        
        }

        cr = vns_config.cmp_optimality(bxc, started_with, 0, 1);
        arrfree(&cr.best);
        arrfree(&started_with);
    }while(cr.index == 0);
        
    return bxc;
}


POLICY first_improvement(POLICY bx, NEIGHBORHOOD_STRUCT nei_struct){
    POLICY bxc = df_element_copy(bx);
    CMP_RESULT cr;

    // local search exploitation : 
    do{
        POLICY *x = NULL;
        POLICY started_with = df_element_copy(bxc);
        int i = 0;
        
        while((x = nei_struct(bxc,i)) != NULL){   
            cr = vns_config.cmp_optimality(*x, bxc, 0, 1); 

            if(cr.index == 0){
                arrfree(&bxc);
                bxc = df_element_copy(*x);
                arrfree(&cr.best);
                arrfree(x);
                free(x);
                x = NULL;
                break;
            }

            arrfree(&cr.best);
            arrfree(x);
            free(x);
            x = NULL;
            i++;        
        }

        cr = vns_config.cmp_optimality(bxc, started_with, 0, 1);
        arrfree(&cr.best);
        arrfree(&started_with);
    }while(cr.index == 0);
        
    return bxc;
}


// STOCHASTIC LOCAL SEARCH : 
POLICY stochastic_hill_climbing(POLICY bx, NEIGHBORHOOD_STRUCT nei_struct){
    int stop_condition = vns_config.STOCHASTIC_LR_FAILURE_LIMIT;
    int counter = stop_condition;
    POLICY bxc = df_element_copy(bx);

    while(counter > 0){
        // arrshow(&bxc);
        POLICY *x = nei_struct(bxc, -1);
        CMP_RESULT cr = vns_config.cmp_optimality(*x, bxc, 0, 1);
        
        
        if(cr.index == 0){
            arrfree(&bxc);
            bxc = df_element_copy(*x);
            counter = stop_condition;
        }
        else{
            counter--;
        }
        arrfree(&cr.best);
        arrfree(x);
        free(x);
        x = NULL;
    }
    

    return bxc;    
}




POLICY VND(POLICY bx, NEIGHBORHOOD_STRUCTURES structs, int kmax){
    int k = 0;
    POLICY bxc = df_element_copy(bx);
    
    while(k < kmax){      
        POLICY x = vns_config.local_search(bxc, structs[k]);
        LOCAL_SEARCH_RESULT lsr = change_neighborhood(x, bxc, k);
        // change of neighborhood
        k = lsr.k;
        // change of the best solution
        arrfree(&bxc);
        arrfree(&x);
        bxc = df_element_copy(lsr.bx);
        arrfree(&lsr.bx);
        
    }
    
    return bxc;
}




POLICY GVNS(POLICY bx, NEIGHBORHOOD_STRUCTURES N1, NEIGHBORHOOD_STRUCTURES N2, int kmax, int lmax, int stopping_condition){
    
    POLICY bxc = df_element_copy(bx);
    int counter = stopping_condition;

    while(counter > 0){
        POLICY x_start = df_element_copy(bxc);
        int k = 0;
        while(k < kmax){
            printf("%d\n", vns_config.f(bxc).node.Int);
            // shake : 
            POLICY* x = N1[k](bxc, -1);

            if(x != NULL){
                
                // local search : 
                POLICY x_2 = VND(*x, N2, lmax);
                
                arrfree(x);
                free(x);
                x = NULL;

                // changing the neighborhood :
                LOCAL_SEARCH_RESULT lsr = change_neighborhood(x_2, bxc, k);
                k = lsr.k;

                arrfree(&x_2);
                arrfree(&bxc);
                bxc = df_element_copy(lsr.bx);
                arrfree(&lsr.bx);
            }
            
            
        }

        // change the status of stopping condition : 
        CMP_RESULT cr = vns_config.cmp_optimality(bxc, x_start, 0, 1);
        if(cr.index == 0)
            counter = stopping_condition;
        else
            counter--;
        arrfree(&cr.best);
        arrfree(&x_start);
    }

    return bxc;
}



void fVNS(){
    free(vns_config.other_params);
    df_free(vns_config.ds, 0);
}