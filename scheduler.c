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
    
    DF_ELEMENT loader;
    loader.node.Int = 0;
    DF_ELEMENT initval;
    initval.type = DF_ELEMENT_TInt;
    initval.node.Int = 0;
    DF_ELEMENT machines = arrinit(vns_config.ds->data[0][1].node.Int, initval);

    for(int i = 0;i<x.node.Arr->size; i++){
        CMP_RESULT mn = arrcmp(&machines, min);
        CMP_RESULT mx = max(mn.best, loader, 0, 0);

        loader.node.Int = mx.best.node.Int + vns_config.ds->data[2][i].node.Int;
        machines.node.Arr->data[mn.index].node.Int = loader.node.Int + vns_config.ds->data[1][i].node.Int;
    }

    CMP_RESULT mx = arrcmp(&machines, max);

    ov = mx.best;
    arrfree(&machines);
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
    return stochastic_block_swapping(x, 20, 2);
}

POLICY block_reversing_1(POLICY x){
    return stochastic_block_reversing(x, 20);
}

POLICY block_swapping_3(POLICY x){
    return stochastic_block_swapping(x, 30, 4);
}


POLICY block_swapping_2(POLICY x){
    return stochastic_block_swapping(x, 10, 0);
}

POLICY block_reversing_2(POLICY x){
    return stochastic_block_reversing(x, 50);
}

POLICY block_reversing_3(POLICY x){
    return stochastic_block_reversing(x, 100);
}


void main(){
    
    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    FILE *fds = fopen("/mnt/e/INSEA-STUDENT/S1/TECH-OPT/VNS_ASSIGNMENT/c500t3.txt", "r");
    vns_config.ds = csv_to_df(fds, "\t"); 
    DF_STR_TO_INT(&vns_config.ds->data[0][0]);
    DF_STR_TO_INT(&vns_config.ds->data[0][1]);
    df_retype(vns_config.ds, DF_ELEMENT_TInt, 1);
    display_df(vns_config.ds, 1);
    
    POLICY bx = arrcreate(500);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = 499 - i;
    }

    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N1 = stoch_neistructs(3);
    N1[0] = block_swapping_1;
    N1[1] = block_reversing_1;
    N1[2] = block_reversing_3;

    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N2 = stoch_neistructs(3);
    N2[0] = block_swapping_2;
    N2[1] = block_reversing_2;
    N2[2] = block_swapping_3;
 


    // POLICY x = STOCHASTIC_BVNS(bx, N2, 3, 10);
    POLICY x = STOCHASTIC_GVNS(bx, N1, N2, 3, 3, 30);
    OPT_VAL ov = vns_config.f(x); 
    printf(" %d ", ov.node.Int);

    free(N1);
    free(N2);
    arrfree(&x);
    arrfree(&bx);
    fclose(fds);
    df_free(vns_config.ds);

}