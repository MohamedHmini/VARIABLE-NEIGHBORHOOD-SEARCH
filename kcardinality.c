#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "VNS.h"
#include "HLCio/advio.h"
#include "HLCio/dataframe.h"



OPT_VAL f(POLICY x){
    OPT_VAL ov;
    
    // display_df(vns_config.ds, 0);
    ov.node.Int = 0;

    for(int i = 1;i<x.node.Arr->size;i++){
        // arrshow(&x);
        ov.node.Int += vns_config.ds->data[ x.node.Arr->data[i - 1].node.Int ][ x.node.Arr->data[i].node.Int ].node.Int;
    }

    return ov;    
}


CMP_RESULT cmp(POLICY x, POLICY bx, int i, int j){
    CMP_RESULT r;
        
    // printf(" %d ",vns_config.f(x).node.Int);

    if(vns_config.f(x).node.Int < vns_config.f(bx).node.Int){
        r.best = df_element_copy(x);
        r.index = i;
    }
    else{
        r.best = df_element_copy(bx);
        r.index = j;
    }
    
    return r;
}


POLICY* block_swapping_1(POLICY x, int i){
    return block_swapping(x, 1, 1, i);
}

POLICY* block_reversing_1(POLICY x, int i){
    return block_reversing(x, 4, i);
}

POLICY* block_reversing_4(POLICY x, int i){
    return block_reversing(x, 6, i);
}


POLICY* block_swapping_2(POLICY x, int i){
    return block_swapping(x, 3, 0, i);
}

POLICY* block_reversing_2(POLICY x, int i){
    return block_reversing(x, 9, i);
}

POLICY* block_reversing_3(POLICY x, int i){
    return block_reversing(x, 6, i);
}



void main(){
    
    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    vns_config.local_search = first_improvement;
    // vns_config.STOCHASTIC_LR_FAILURE_LIMIT = 100;

    // vns_config.STOCHASTIC_LR_FAILURE_LIMIT = 10;
    FILE *fds = fopen("./datasets/ds2.csv", "r");
    
    vns_config.ds = csv_to_df(fds, " ");     
    df_retype(vns_config.ds, DF_ELEMENT_TInt, 0);
    display_df(vns_config.ds, 0);
    
    POLICY bx = arrcreate(10);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = 9 - i;
    }
    

    NEIGHBORHOOD_STRUCTURES N1 = neistructs(3);
    N1[0] = block_swapping_1;
    N1[1] = block_reversing_1;
    N1[2] = block_reversing_4;

    NEIGHBORHOOD_STRUCTURES N2 = neistructs(3);
    N2[0] = block_swapping_2;
    N2[1] = block_reversing_2;
    N2[2] = block_reversing_3;

    
    arrshow(&bx);
    OPT_VAL ov = vns_config.f(bx); 
    printf(" %d ", ov.node.Int);    
    
    POLICY x = GVNS(bx, N1, N2, 3, 3, 300);
    
    arrshow(&x);
    ov = vns_config.f(x); 
    printf(" %d ", ov.node.Int);
    
    arrfree(&x);
    free(N1);
    free(N2);
    arrfree(&bx);
    fclose(fds);
    df_free(vns_config.ds, 0);

}