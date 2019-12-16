#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include "VNS.h"
#include "HLCio/advio.h"
#include "HLCio/dataframe.h"
#define GVNS_STOPPING_CONDITION 300
// #define EVALUATION_FACTOR 20

/// the quick sort algorithm is taken from geeksforgeeks platform!

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

int partition (int arr[], int xi[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);  // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        if (arr[j] < pivot) 
        { 
            i++;   
            swap(&arr[i], &arr[j]); 
            swap(&xi[i], &xi[j]);
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    swap(&xi[i + 1], &xi[high]); 
    return (i + 1); 
} 
  
  
void quickSort(int arr[], int xi[], int low, int high) 
{ 
    if (low < high) 
    { 
        
        int pi = partition(arr, xi, low, high); 
        quickSort(arr, xi, low, pi - 1); 
        quickSort(arr, xi, pi + 1, high); 
    } 
} 



POLICY LPT(){
    POLICY x = arrcreate(500);
    int s = vns_config.ds->len_cols;
    int arr[s];
    int xi[s];

    for (int i = 0; i < s; i++){
        arr[i] = vns_config.ds->data[1][i].node.Int;
        xi[i] = i;
    }

    quickSort(arr, xi, 0, s);

    for(int i = 0; i<x.node.Arr->size; i++){
        x.node.Arr->data[i].type = DF_ELEMENT_TInt;
        x.node.Arr->data[i].node.Int = xi[i];
    }
    

    return x;    
}


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
    DF_ELEMENT machines = arrinit(vns_config.other_params[1], initval);

    for(int i = 0;i<x.node.Arr->size; i++){
        CMP_RESULT mn = arrcmp(&machines, min);
        CMP_RESULT mx = max(mn.best, loader, 0, 0);

        loader.node.Int = mx.best.node.Int + vns_config.ds->data[1][x.node.Arr->data[i].node.Int].node.Int;
        machines.node.Arr->data[mn.index].node.Int = loader.node.Int + vns_config.ds->data[0][x.node.Arr->data[i].node.Int].node.Int;
    }

    CMP_RESULT mx = arrcmp(&machines, max);

    ov = mx.best;
    arrfree(&machines);
    return ov;    
}


CMP_RESULT cmp(POLICY x, POLICY bx, int i, int j){
    CMP_RESULT r;
            
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


POLICY* s1(POLICY x, int i){
    return block_swapping(x, 10, 0, i);
}

POLICY* s2(POLICY x, int i){
    return block_swapping(x, 4, 30, i);
}

POLICY* s3(POLICY x, int i){
    return block_swapping(x, 1, 100, i);
}


POLICY* s4(POLICY x, int i){
    return block_reversing(x, 20, i);
}

POLICY* s5(POLICY x, int i){
    return block_reversing(x, 30, i);
}

POLICY* s6(POLICY x, int i){
    return block_reversing(x, 10, i);
}



int main(int argc, char *args[]){
    char *filename = args[1];
    int EVALUATION_FACTOR = atoi(args[2]);
    
    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    vns_config.local_search = first_improvement;
    // vns_config.STOCHASTIC_LR_FAILURE_LIMIT = 300;

    FILE *fds = fopen(filename, "r");
    
    if(fds == NULL)
        exit(EXIT_FAILURE); 
    
    // load the number of commodities and the number of drivers :
    vns_config.other_params = (int*)malloc(sizeof(int) * 2);
    char* fline = get_line(fds);

    vns_config.other_params[0] = atoi(strtok(fline, " "));
    vns_config.other_params[1] = atoi(strtok(NULL, " "));
    free(fline);
    rewind(fds);  
    printf("\n %d ", vns_config.other_params[0]);  

    // load the commodities processing time / set up time :
    vns_config.ds = csv_to_df(fds, 1, "\t"); 

    // retyping the data to int :
    df_retype(vns_config.ds, DF_ELEMENT_TInt, 0);
    // display_df(vns_config.ds, 0);
    
    // initiating the solution with LONGEST PROCESSING TIME first :
    // POLICY bx = LPT();

    POLICY bx = arrcreate(vns_config.other_params[0]);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = i;
    }

    OPT_VAL ov = vns_config.f(bx); 
    printf(" %d ", ov.node.Int);

    // creating the first neighborhood structure set :
    NEIGHBORHOOD_STRUCTURES N1 = neistructs(3);
    N1[0] = s1;
    N1[1] = s2;
    N1[2] = s3;

    // creating the second neighborhood structure set :
    NEIGHBORHOOD_STRUCTURES N2 = neistructs(3);
    N2[0] = s4;
    N2[1] = s5;
    N2[2] = s6;

    int ev_counter = EVALUATION_FACTOR;
    DF_ELEMENT EVA_ARR_OV = arrcreate(EVALUATION_FACTOR);
    DF_ELEMENT EVA_ARR_ET = arrcreate(EVALUATION_FACTOR);

    while(ev_counter > 0){
        printf("\n iter %d\n", EVALUATION_FACTOR - ev_counter);
        int i;
        double total_time;
        clock_t start, end;
        start = clock();
        //time count starts 
        srand(time(NULL));

        // running GVNS :
        POLICY x = GVNS(bx, N2, N1, 3, 3, 15);
        arrshow(&x);
        printf("here");
        ov = vns_config.f(x); 
        
        end = clock();
	    total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
        
        EVA_ARR_OV.node.Arr->data[EVALUATION_FACTOR - ev_counter].node.Int = ov.node.Int;
        EVA_ARR_OV.node.Arr->data[EVALUATION_FACTOR - ev_counter].type = DF_ELEMENT_TInt;
        EVA_ARR_ET.node.Arr->data[EVALUATION_FACTOR - ev_counter].node.Int = (int) total_time;
        EVA_ARR_ET.node.Arr->data[EVALUATION_FACTOR - ev_counter].type = DF_ELEMENT_TInt;

        arrfree(&x);
        ev_counter--;
    }
    
    printf("\nEVALUATION FACTOR : %d\n", EVALUATION_FACTOR);
    CMP_RESULT cr = arrcmp(&EVA_ARR_OV, min);
    printf("\nOV MIN :  %d ", cr.best.node.Int);
    cr = arrcmp(&EVA_ARR_OV, max);
    printf("\nOV MAX :  %d ", cr.best.node.Int);

    int avg = 0;
    int sum = 0;

    for (int i = 0; i < EVALUATION_FACTOR; i++){
        sum += EVA_ARR_OV.node.Arr->data[i].node.Int;
    }
    avg = sum/EVALUATION_FACTOR;

    printf("\nOV AVG :  %d ", avg);

    cr = arrcmp(&EVA_ARR_ET, min);
    printf("\nET MIN :  %d ", cr.best.node.Int);
    cr = arrcmp(&EVA_ARR_ET, max);
    printf("\nET MAX :  %d ", cr.best.node.Int);

    avg = 0;
    sum = 0;

    for (int i = 0; i < EVALUATION_FACTOR; i++){
        sum += EVA_ARR_ET.node.Arr->data[i].node.Int;
    }
    avg = sum/EVALUATION_FACTOR;

    printf("\nET AVG :  %d ", avg);

    arrfree(&EVA_ARR_OV);
    arrfree(&EVA_ARR_ET);
    free(N1);
    free(N2);    
    arrfree(&bx);
    fclose(fds);
    fVNS();

    exit(EXIT_SUCCESS);

}