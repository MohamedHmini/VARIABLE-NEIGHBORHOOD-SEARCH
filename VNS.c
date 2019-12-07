#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "VNS.h"
#include "HLCio/dataframe.h"



VNS_CONFIG vns_config;


//


// POLICY shake(POLICY x, STOCHASTIC_NEIGHBORHOOD_STRUCT nei_struct){
//     POLICY nx = nei_struct(x);
//     return nx
// }


// DETERMINISTIC NEIGHBORHOODing :


NEIGHBORHOOD block_swapping(POLICY x, int block_size, int swap_factor){
    NEIGHBORHOOD N;
    int bound = (int)(x.node.Arr->size/block_size) - swap_factor;

    for(int i = 0;i<bound;i++){
        POLICY nx = df_element_copy(x);
        int lb = i+block_size + swap_factor;
        // int ub = i+2*block_size + swap_factor;
        // swapping : 
        for(int j = 0;j<block_size;j++){
            int tmp;
            tmp = nx.node.Arr->data[i + j].node.Int;
            nx.node.Arr->data[i + j].node.Int = nx.node.Arr->data[lb + j].node.Int;
            nx.node.Arr->data[lb + j].node.Int = tmp;
        }

        arrpush(&N,nx);
    }

    return N;
}


NEIGHBORHOOD block_reversing(POLICY x, int block_size){
    NEIGHBORHOOD N;
    int bound = (int)x.node.Arr->size/block_size;

    for(int i = 0;i<bound;i++){
        POLICY nx = df_element_copy(x);

        for(int j = 0;j<(int)block_size/2;j++){
            int tmp;
            tmp = nx.node.Arr->data[i + 1 + j].node.Int;
            nx.node.Arr->data[i + 1 + j].node.Int = nx.node.Arr->data[i + block_size - j].node.Int;
            nx.node.Arr->data[i + block_size - j].node.Int = tmp;
        }
        arrpush(&N, nx);
    }

    return N;
}


// POLICY one_step_block_swapping(POLICY x, int block_size, int swap_factor, int i){

//     int bound = (int)(x.node.Arr->size/block_size) - swap_factor;
    
//     POLICY nx = df_element_copy(x);
//     // POLICY nx = x;
//     int lb = i+block_size + swap_factor;
//     int ub = i+2*block_size + swap_factor;

//     // swapping : 
//     for(int j = 0;j<block_size;j++){
//         int tmp;
//         tmp = nx.node.Arr->data[i + j].node.Int;
//         nx.node.Arr->data[i + j].node.Int = nx.node.Arr->data[lb + j].node.Int;
//         nx.node.Arr->data[lb + j].node.Int = tmp;
//     }

//     return nx;
// }



NEIGHBORHOOD_STRUCTURES neistructs(int size){
    NEIGHBORHOOD_STRUCTURES Ns = malloc(sizeof(NEIGHBORHOOD_STRUCT) * size);
    return Ns;
}



STOCHASTIC_NEIGHBORHOOD_STRUCTURES stoch_neistructs(int size){
    STOCHASTIC_NEIGHBORHOOD_STRUCTURES Ns = malloc(sizeof(STOCHASTIC_NEIGHBORHOOD_STRUCT) * size);
    return Ns;
}


// STOCHASTIC NEIGHBORHOODing : 


POLICY stochastic_block_swapping(POLICY x, int block_size, int swap_factor){

    int bound = (int)(x.node.Arr->size/block_size) - swap_factor;
    srand(time(NULL));
    int i = randint(0, bound);

    POLICY nx = df_element_copy(x);
    // POLICY nx = x;
    int lb = i+block_size + swap_factor;
    int ub = i+2*block_size + swap_factor;

    // swapping : 
    for(int j = 0;j<block_size;j++){
        int tmp;
        tmp = nx.node.Arr->data[i + j].node.Int;
        nx.node.Arr->data[i + j].node.Int = nx.node.Arr->data[lb + j].node.Int;
        nx.node.Arr->data[lb + j].node.Int = tmp;
    }

    return nx;
}


POLICY stochastic_block_reversing(POLICY x, int block_size){

    int bound = (int)x.node.Arr->size/block_size;
    srand(time(NULL));
    int i = randint(0, bound);

    POLICY nx = df_element_copy(x);
    // POLICY nx = x;
    for(int j = 0;j<(int)block_size/2;j++){
        int tmp;
        tmp = nx.node.Arr->data[i + 1 + j].node.Int;
        nx.node.Arr->data[i + 1 + j].node.Int = nx.node.Arr->data[i + block_size - j].node.Int;
        nx.node.Arr->data[i + block_size - j].node.Int = tmp;
    }

    return nx;
}


// ESSENTIAL FUNCTIONS : 

LOCAL_SEARCH_RESULT change_neighborhood(POLICY x, POLICY bx, int k){
    LOCAL_SEARCH_RESULT lsr;
    CMP_RESULT cmp_r = vns_config.cmp_optimality(x, bx, 0, 1);

    if(cmp_r.index == 0){
        // printf("change of nei");
        k = 0;
    }
    else{
        k++;
    }

    lsr.k = k;
    lsr.bx = df_element_copy(cmp_r.best);
    // lsr.bx = cmp_r.best;
    arrfree(&cmp_r.best);
    
    return lsr;
}

// DETERMINISTIC LOCAL SEARCH : 
POLICY best_improvement(POLICY bx, NEIGHBORHOOD N){
    arrpush(&N, bx);
    
    CMP_RESULT r;
    r.best = bx;
    r.index = N.node.Arr->size - 1;

    r = arrcmp(&N, vns_config.cmp_optimality);

    return bx;
}


POLICY first_improvement(POLICY bx, STOCHASTIC_NEIGHBORHOOD_STRUCT nei_struct){
    // int bound;
    // POLICY bxc = df_element_copy(bx);

    // for(int i = 0;i<bound;i++){
    //     POLICY x = nei_struct(bx,i);
    //     CMP_RESULT cr = vns_config.cmp_optimality(x, bx, 0, 1);
        
    //     if(cr.index == 0){
    //         arrfree(&bxc);
    //         bxc = df_element_copy(x);
    //         counter = stop_condition;
    //     }
    //     else{
    //         counter--;
    //     }
    //     arrfree(&cr.best);
    //     arrfree(&x);
    // }


    // return bxc;
}


// STOCHASTIC LOCAL SEARCH : 
POLICY stochastic_hill_climbing(POLICY bx, STOCHASTIC_NEIGHBORHOOD_STRUCT nei_struct, int stop_condition){
    int counter = stop_condition;
    POLICY bxc = df_element_copy(bx);

    while(counter > 0){
        // printf("\n\tLS : %d", counter);
        // arrshow(&bxc);
        POLICY x = nei_struct(bxc);
        CMP_RESULT cr = vns_config.cmp_optimality(x, bxc, 0, 1);
        
        
        if(cr.index == 0){
            arrfree(&bxc);
            bxc = df_element_copy(x);
            counter = stop_condition;
        }
        else{
            counter--;
        }
        arrfree(&cr.best);
        arrfree(&x);
    }
    

    return bxc;    
}


// GLOBAL OPT : 


// POLICY VND(POLICY bx, NEIGHBORHOOD_STRUCTURES structs, int kmax){
//     int k = 0;

//     while(k < kmax){
//         NEIGHBORHOOD N = structs[k](bx);
//         POLICY x = best_improvement(bx, N);
//         LOCAL_SEARCH_RESULT lsr = change_neighborhood(x, bx, k);
//         bx = lsr.bx;
//         k = lsr.k;
//     }

//     return bx;
// }



POLICY STOCHASTIC_VND(POLICY bx, STOCHASTIC_NEIGHBORHOOD_STRUCTURES structs, int kmax){
    int k = 0;
    POLICY bxc = df_element_copy(bx);
    // POLICY bxc = bx;
    
    while(k < kmax){        
        POLICY x = stochastic_hill_climbing(bxc, structs[k], 700);
        LOCAL_SEARCH_RESULT lsr = change_neighborhood(x, bxc, k);
        k = lsr.k;
        arrfree(&bxc);
        arrfree(&x);
        bxc = df_element_copy(lsr.bx);
        // bxc = lsr.bx;
        arrfree(&lsr.bx);
    }

    return bxc;
}


POLICY STOCHASTIC_BVNS(POLICY bx, STOCHASTIC_NEIGHBORHOOD_STRUCTURES nei_structs, int kmax, int nbr_of_iterations){
    
    POLICY bxc = df_element_copy(bx);
    // POLICY bxc = bx;

    while(nbr_of_iterations > 0){
        int k = 0;
        while(k < kmax){
            POLICY x = nei_structs[k](bxc);
            POLICY x_2 = STOCHASTIC_VND(x, nei_structs, kmax);
            arrfree(&x);
            LOCAL_SEARCH_RESULT lsr = change_neighborhood(x_2, bxc, k);
            k = lsr.k;
            arrfree(&x_2);
            arrfree(&bxc);
            bxc = df_element_copy(lsr.bx);
            arrfree(&lsr.bx);
        }

        nbr_of_iterations--;
    }

    return bxc;
}


POLICY STOCHASTIC_GVNS(POLICY bx, STOCHASTIC_NEIGHBORHOOD_STRUCTURES N1, STOCHASTIC_NEIGHBORHOOD_STRUCTURES N2, int kmax, int lmax, int nbr_of_iterations){
    
    POLICY bxc = df_element_copy(bx);
    // POLICY bxc = bx;

    while(nbr_of_iterations > 0){
        int k = 0;
        while(k < kmax){
            POLICY x = N1[k](bxc);
            POLICY x_2 = STOCHASTIC_VND(x, N2, kmax);
            arrfree(&x);
            LOCAL_SEARCH_RESULT lsr = change_neighborhood(x_2, bxc, k);
            k = lsr.k;
            arrfree(&x_2);
            arrfree(&bxc);
            bxc = df_element_copy(lsr.bx);
            arrfree(&lsr.bx);
        }

        nbr_of_iterations--;
        // arrshow(&bxc);
        printf("\nITER %d : %d",nbr_of_iterations, vns_config.f(bxc).node.Int);
    }

    return bxc;
}

// POLICY RVNS(){}
// POLICY BVNS(){}
// POLICY GVNS(){}

