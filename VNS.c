#include<stdio.h>
#include<stdlib.h>
#include "VNS.h"
#include "HLCio/dataframe.h"



OPT_FUNC f = NULL;


// DETERMINISTIC NEIGHBORHOODing :


NEIGHBORHOOD block_swapping(POLICY x, int block_size, int swap_factor){
    NEIGHBORHOOD N;
    int bound = (int)(x.node.Arr->size/block_size) - swap_factor;

    for(int i = 0;i<bound;i++){
        POLICY nx = df_element_copy(x);
        int lb = i+block_size + swap_factor;
        int ub = i+2*block_size + swap_factor;
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

        for(int j = 0;j<;j++){
            int tmp;
            tmp = nx.node.Arr->data[i + 1 + j].node.Int;
            nx.node.Arr->data[i + 1 + j].node.Int = nx.node.Arr->data[i + block_size - j].node.Int;
            nx.node.Arr->data[i + block_size - j].node.Int = tmp;
        }
        arrpush(&N, nx);
    }

    return N;
}



// STOCHASTIC NEIGHBORHOODing : 


// ESSENTIAL FUNCTIONS : 

LOCAL_SEARCH_RESULT change_neighborhood(POLICY x, POLICY bx, int k){
    LOCAL_SEARCH_RESULT lsr;

    if(cmp_optimality(x, bx)){
        lsr.bx = x;
        lsr.k = 1;
    }
    else{
        lsr.bx = bx;
        lsr.k++;
    }

    return lsr;
}

// LOCAL SEARCH : 
POLICY best_improvement(POLICY bx){
    
    return bx;
}