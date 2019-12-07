#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include "VNS.h"
#include "HLCio/advio.h"
#include "HLCio/dataframe.h"


OPT_VAL f(POLICY x){
    OPT_VAL ov;
    ov.type = DF_ELEMENT_TInt;
    ov.node.Int = 0;

    for(int i = 1;i<x.node.Arr->size;i++){
        int m = x.node.Arr->data[i - 1].node.Int;
        int n = x.node.Arr->data[i].node.Int;
        int v =  vns_config.ds->data[ m ][ n ].node.Int;
        ov.node.Int += v;
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
    return stochastic_block_swapping(x, 2, 2);
}

POLICY block_reversing_1(POLICY x){
    return stochastic_block_reversing(x, 6);
}


POLICY block_reversing_3(POLICY x){
    return stochastic_block_reversing(x, 8);
}

POLICY block_swapping_2(POLICY x){
    return stochastic_block_swapping(x, 2, 0);
}

POLICY block_reversing_2(POLICY x){
    return stochastic_block_reversing(x, 4);
}

POLICY block_swapping_3(POLICY x){
    return stochastic_block_swapping(x, 3, 0);
}

int main(){
    
    FILE *fds = NULL;
    fds = fopen("/mnt/e/INSEA-STUDENT/S1/TECH-OPT/TSP/ds2.csv", "r");

    if(fds == NULL){
        perror("file error");
        exit(1);
    }

    DATAFRAME *ds = csv_to_df(fds, " ");   
    df_retype(ds, DF_ELEMENT_TInt, 0); 
    display_df(ds);
    
    POLICY bx = arrcreate(10);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = 9 - i;
    }


    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N1 = stoch_neistructs(3);
    N1[0] = block_swapping_1;
    N1[1] = block_reversing_1;
    N1[2] = block_reversing_3;

    STOCHASTIC_NEIGHBORHOOD_STRUCTURES N2 = stoch_neistructs(3);
    N2[0] = block_swapping_2;
    N2[1] = block_reversing_2;
    N2[2] = block_swapping_3;

    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    vns_config.ds = ds;


    POLICY x = STOCHASTIC_GVNS(bx, N1, N2, 3, 3, 50000);
    OPT_VAL ov = vns_config.f(x);
    printf("opt : %d ", ov.node.Int);
    arrfree(&x);
    arrfree(&bx);
    free(N1);
    free(N2);

    // while((c = strtok(NULL, " ")) != NULL){
    //     printf("%s",c);
    // }

    // printf("%s",c);


    // for(int i = 0; i< 1; i++){
    //     DF_ELEMENT e = arrcreate(10);
    //     for(int j = 0;j<e.node.Arr->size;j++){
    //         e.node.Arr->data[j].type = DF_ELEMENT_TInt;
    //         e.node.Arr->data[j].node.Int = j;
    //     }

    //     for(int k = 0;k<100;k++){
    //         DF_ELEMENT c = df_element_copy(e);
    //         arrfree(&c);
    //     }


    //     arrfree(&e);
        
    // }

    
    // df_retype(ds, DF_ELEMENT_TInt, 0);
    // 
    
    // POLICY bx = arrcreate(10);
    // for(int i = 0; i<bx.node.Arr->size; i++){
    //     bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
    //     bx.node.Arr->data[i].node.Int = 9 - i;
    // }

    fclose(fds);
    df_free(ds);
}


// ==1447== 31 bytes in 1 blocks are possibly lost in loss record 2 of 5
// ==1447==    at 0x4837D7B: realloc (vg_replace_malloc.c:836)
// ==1447==    by 0x109C0D: get_line (advio.c:39)
// ==1447==    by 0x109D00: read_lines (advio.c:79)
// ==1447==    by 0x109F2E: csv_to_df (advio.c:152)
// ==1447==    by 0x10AADF: main (test.c:13)