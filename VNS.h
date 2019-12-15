#include "HLCio/dataframe.h"



#ifndef H_VNS
#define H_VNS





typedef DF_ELEMENT OPT_VAL;
typedef DF_ELEMENT POLICY;
typedef DF_ELEMENT NEIGHBORHOOD;

typedef POLICY* (*NEIGHBORHOOD_STRUCT)();
typedef POLICY* (**NEIGHBORHOOD_STRUCTURES)();

typedef POLICY (*LOCAL_SEARCH)();


typedef struct LOCAL_SEARCH_RESULT{
    POLICY bx; // neighborhood structure optimal policy
    int k; // neighborhood structure index
}LOCAL_SEARCH_RESULT;

typedef struct VNS_RESULT{
    POLICY policy;
    OPT_VAL opt_val;
}VNS_RESULT;



// OBJECTIVE FUNCTION : 
typedef OPT_VAL (*OPT_FUNC)(POLICY);

// OPTIMALITY CONDITION : 
typedef CMP_RESULT (*CMP_OPTIMALITY)(POLICY, POLICY, int, int);



// VNS CONFIG STRUCT :
typedef struct VNS_CONFIG{
    OPT_FUNC f;
    CMP_OPTIMALITY cmp_optimality;
    LOCAL_SEARCH local_search;
    int STOCHASTIC_LR_FAILURE_LIMIT;
    DATAFRAME *ds;
    int* other_params;
}VNS_CONFIG;

extern VNS_CONFIG vns_config;



// ESSENTIAL FUNCTIONS :
    
    // neighborhood structures : 

POLICY* block_swapping(POLICY, int, int, int);
POLICY* block_reversing(POLICY, int, int);
POLICY* __block_swapping(POLICY, int, int, int);
POLICY* __block_reversing(POLICY, int, int);


LOCAL_SEARCH_RESULT change_neighborhood(POLICY, POLICY, int);
NEIGHBORHOOD_STRUCTURES neistructs(int);


// DETERMINISTIC LOCAL SEARCH FUNCTIONS :
POLICY best_improvement(POLICY, NEIGHBORHOOD_STRUCT);
POLICY first_improvement(POLICY, NEIGHBORHOOD_STRUCT);

// STOCHASTIC LOCAL SEARCH : 
POLICY stochastic_hill_climbing(POLICY, NEIGHBORHOOD_STRUCT);


// VARIABLE NEIGHBORHOOD SEARCH :

    // VARIABLE NEIGHBORHOOD DESCENT : 
POLICY VND(POLICY, NEIGHBORHOOD_STRUCTURES, int);
    // GENERAL VARIABLE NEIGHBORHOOD SEARCH : 
POLICY GVNS(POLICY, NEIGHBORHOOD_STRUCTURES, NEIGHBORHOOD_STRUCTURES, int, int, long);

// free VNS config :
void fVNS();

#endif