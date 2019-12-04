#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "HLCio/dataframe.h"



#ifndef H_VNS
#define H_VNS




typedef DF_ELEMENT OPT_VAL;
typedef DF_ELEMENT POLICY;
typedef DF_ELEMENT NEIGHBORHOOD;
typedef POLICY (*neighborhood_structure)();


typedef struct LOCAL_SEARCH_RESULT{
    POLICY bx; // neighborhood structure optimal policy
    int k; // neighborhood structure index
}LOCAL_SEARCH_RESULT;

typedef struct RESULT{
    POLICY policy;
    OPT_VAL opt_val;
}RESULT;



// OBJECTIVE FUNCTION : 
typedef OPT_VAL (*OPT_FUNC)(POLICY);
extern OPT_FUNC f;

// OPTIMALITY CONDITION : 
typedef bool (*CMP_OPTIMALITY)(POLICY, POLICY);
extern CMP_OPTIMALITY cmp_optimality;

// ESSENTIAL FUNCTIONS :
    // neighborhood structures : 
NEIGHBORHOOD block_swapping(POLICY, int, int);
NEIGHBORHOOD block_reversing(POLICY, int);
NEIGHBORHOOD shuffling();
NEIGHBORHOOD (*neighborhood)();
// POLICY (*get_candidate)();
LOCAL_SEARCH_RESULT change_neighborhood(POLICY, POLICY, int);
// POLICY shake(POLICY, int);


// LOCAL SEARCH FUNCTIONS :
POLICY best_improvement(POLICY);
POLICY first_improvement(POLICY);
// P.S : THE FULL IMPLEMENTATION OF THE ABOVE'S LOCAL SEARCH ALGOS IS FOUND IN THE VND ALGORITHM BENEATH THIS POST-SCRIPT.


// VARIABLE NEIGHBORHOOD SEARCH :

    // VARIABLE NEIGHBORHOOD DESCENT : 
POLICY VND(POLICY, POLICY (**)());
    // VNS VARIANTS : 
POLICY VNS(POLICY, POLICY (**)());
POLICY RVNS(POLICY, POLICY (**)());
POLICY BVNS(POLICY, POLICY (**)());
POLICY GVNS(POLICY, POLICY (**)());


#endif