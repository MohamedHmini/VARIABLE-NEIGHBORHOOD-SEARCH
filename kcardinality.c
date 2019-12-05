#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "VNS.h"
#include "HLCio/advio.h"
#include "HLCio/dataframe.h"


CMP_RESULT min(DF_ELEMENT e1, DF_ELEMENT e2, int i, int j){
    CMP_RESULT r;

    if(e1.node.Int < e2.node.Int){
        r.best = e1;
        r.index = i;
    }
    else{
        r.best = e2;
        r.index = j;
    }

    return r;
}


CMP_RESULT max(DF_ELEMENT e1, DF_ELEMENT e2, int i, int j){
    CMP_RESULT r;

    if(e1.node.Int > e2.node.Int){
        r.best = e1;
        r.index = i;
    }
    else{
        r.best = e2;
        r.index = j;
    }

    return r;
}


OPT_VAL f(POLICY x){
    OPT_VAL ov;
    
    ov.node.Int = 0;

    for(int i = 1;i<x.node.Arr->size;i++){
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


POLICY block_swapping_1(POLICY x){
    return stochastic_block_swapping(x, 2, 0);
}

POLICY block_reversing_1(POLICY x){
    return stochastic_block_reversing(x, 4);
}

POLICY block_swapping_2(POLICY x){
    return stochastic_block_swapping(x, 2, 0);
}

POLICY block_reversing_2(POLICY x){
    return stochastic_block_reversing(x, 4);
}


void main(){
    
    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    FILE *fds = fopen("E:\\INSEA-STUDENT\\S1\\TECH-OPT\\TSP\\ds2.csv", "r");
    vns_config.ds = csv_to_df(fds, " ");     
    df_retype(vns_config.ds, DF_ELEMENT_TInt, 0);
    display_df(vns_config.ds);
    
    POLICY bx = arrcreate(10);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = 9 - i;
    }

    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N1 = stoch_neistructs(2);
    N1[0] = block_swapping_1;
    N1[1] = block_reversing_1;

    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N2 = stoch_neistructs(2);
    N2[0] = block_swapping_2;
    N2[1] = block_swapping_2;
    

    // arrshow(&bx);
    
    arrshow(&bx);
    OPT_VAL ov = vns_config.f(bx); 
    printf(" %d ", ov.node.Int);    
    POLICY x = STOCHASTIC_BVNS(bx, N2, 2, 100);
    // POLICY x = STOCHASTIC_VND(bx, N2, 2);
    //  OLICY x = stochastic_hill_climbing(bx, N2[0], 10000);
    arrshow(&x);
    ov = vns_config.f(x); 
    printf(" %d ", ov.node.Int);
    // arrshow(&x);
    // arrfree(&x);
    arrfree(&bx);
    fclose(fds);
    df_free(vns_config.ds);

}