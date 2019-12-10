#include "HLCio/dataframe.h"



#ifndef H_VNS
#define H_VNS





typedef DF_ELEMENT OPT_VAL;
typedef DF_ELEMENT POLICY;
typedef DF_ELEMENT NEIGHBORHOOD;

// DETERMINISTIC NIGHBORHOODing : 
typedef POLICY* (*NEIGHBORHOOD_STRUCT)();
typedef POLICY* (**NEIGHBORHOOD_STRUCTURES)();

typedef POLICY (*LOCAL_SEARCH)();

// STOCHASTIC NEIGHBORHOODing : 
// typedef POLICY (*STOCHASTIC_NEIGHBORHOOD_STRUCT)();
// typedef POLICY (**STOCHASTIC_NEIGHBORHOOD_STRUCTURES)();

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
}VNS_CONFIG;

extern VNS_CONFIG vns_config;



// ESSENTIAL FUNCTIONS :
    // deterministic neighborhood structures : 

POLICY* block_swapping(POLICY, int, int, int);
POLICY* block_reversing(POLICY, int, int);
POLICY* __block_swapping(POLICY, int, int, int);
POLICY* __block_reversing(POLICY, int, int);
// NEIGHBORHOOD shuffling();
// NEIGHBORHOOD (*neighborhood)();

     // deterministic neighborhood structures : 
// POLICY* stochastic_block_swapping(POLICY, int, int);
// POLICY* stochastic_block_reversing(POLICY, int);
// POLICY stochastic_shuffling();
// POLICY (*stochastic_neighborhood)();

// POLICY (*get_candidate)();
LOCAL_SEARCH_RESULT change_neighborhood(POLICY, POLICY, int);
// POLICY shake(POLICY, STOCHASTIC_NEIGHBORHOOD_STRUCT);
NEIGHBORHOOD_STRUCTURES neistructs(int);
// STOCHASTIC_NEIGHBORHOOD_STRUCTURES stoch_neistructs(int);


// DETERMINISTIC LOCAL SEARCH FUNCTIONS :
POLICY best_improvement(POLICY, NEIGHBORHOOD_STRUCT);
POLICY first_improvement(POLICY, NEIGHBORHOOD_STRUCT);

// STOCHASTIC LOCAL SEARCH : 
POLICY stochastic_hill_climbing(POLICY, NEIGHBORHOOD_STRUCT);

// P.S : THE FULL IMPLEMENTATION OF THE ABOVE'S LOCAL SEARCH ALGOS IS FOUND IN THE VND ALGORITHM BENEATH THIS POST-SCRIPT.


// VARIABLE NEIGHBORHOOD SEARCH :

    // VARIABLE NEIGHBORHOOD DESCENT : 
POLICY VND(POLICY, NEIGHBORHOOD_STRUCTURES, int);
POLICY GVNS(POLICY, NEIGHBORHOOD_STRUCTURES, NEIGHBORHOOD_STRUCTURES, int, int, long);


// POLICY DETERMINISTIC_VND(POLICY, NEIGHBORHOOD_STRUCTURES, int);
// POLICY STOCHASTIC_VND(POLICY, STOCHASTIC_NEIGHBORHOOD_STRUCTURES, int);
    // VNS VARIANTS : 
// POLICY STOCHASTIC_BVNS(POLICY, STOCHASTIC_NEIGHBORHOOD_STRUCTURES, int, int); 
// POLICY STOCHASTIC_GVNS(POLICY, STOCHASTIC_NEIGHBORHOOD_STRUCTURES, STOCHASTIC_NEIGHBORHOOD_STRUCTURES, int, int, int);
// POLICY DETERMINISTIC_GVNS(POLICY, NEIGHBORHOOD_STRUCTURES, NEIGHBORHOOD_STRUCTURES, int, int, long);
// POLICY VNS(POLICY, POLICY (**)());
// POLICY RVNS(POLICY, POLICY (**)());
// POLICY BVNS(POLICY, POLICY (**)());
// POLICY GVNS(POLICY, POLICY (**)());


#endif