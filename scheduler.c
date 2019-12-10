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

        loader.node.Int = mx.best.node.Int + vns_config.ds->data[2][x.node.Arr->data[i].node.Int].node.Int;
        machines.node.Arr->data[mn.index].node.Int = loader.node.Int + vns_config.ds->data[1][x.node.Arr->data[i].node.Int].node.Int;
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
    return block_swapping(x, 1, 10, i);
}

POLICY* s2(POLICY x, int i){
    return block_swapping(x, 10, 0, i);
}

POLICY* s3(POLICY x, int i){
    return block_swapping(x, 5, 5, i);
}


POLICY* s4(POLICY x, int i){
    return block_reversing(x, 10, i);
}

POLICY* s5(POLICY x, int i){
    return block_reversing(x, 30, i);
}

POLICY* s6(POLICY x, int i){
    return block_reversing(x, 100, i);
}


void main(){
    vns_config.f = f;
    vns_config.cmp_optimality = cmp;
    vns_config.local_search = first_improvement;
    // vns_config.STOCHASTIC_LR_FAILURE_LIMIT = 100;

    FILE *fds = fopen("./dataset/c500t3.txt", "r");
    
    if(fds == NULL)
        exit(EXIT_FAILURE); 
    
    vns_config.ds = csv_to_df(fds, "\t"); 

    DF_STR_TO_INT(&vns_config.ds->data[0][0]);
    DF_STR_TO_INT(&vns_config.ds->data[0][1]);
    df_retype(vns_config.ds, DF_ELEMENT_TInt, 1);
    display_df(vns_config.ds, 1);
    

    // int s[] = {10, 1, 12, 13, 4, 5, 6, 7, 8, 19, 0, 11, 22, 3, 14, 25, 16, 17, 18, 9, 20, 21, 2, 23, 24, 15, 26, 37, 38, 29, 30, 41, 32, 33, 44, 45, 36, 27, 28, 39, 40, 31, 52, 53, 34, 35, 46, 47, 48, 49, 60, 51, 42, 43, 54, 55, 56, 67, 58, 59, 50, 61, 62, 63, 64, 65, 66, 87, 68, 69, 70, 71, 82, 73, 74, 75, 76, 57, 78, 89, 80, 81, 72, 83, 84, 85, 86, 77, 88, 79, 90, 91, 92, 93, 94, 105, 96, 97, 98, 99, 100, 101, 102, 103, 104, 95, 116, 107, 108, 109, 110, 111, 112, 113, 114, 125, 106, 127, 118, 119, 120, 121, 122, 133, 124, 115, 126, 117, 128, 129, 130, 131, 132, 123, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 164, 155, 156, 157, 158, 159, 160, 161, 162, 173, 154, 165, 166, 167, 168, 169, 170, 181, 172, 163, 174, 185, 176, 177, 178, 179, 180, 171, 182, 183, 184, 175, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 224, 215, 216, 217, 218, 229, 220, 221, 222, 223, 214, 235, 226, 227, 228, 219, 230, 231, 232, 243, 234, 225, 236, 237, 238, 239, 250, 241, 242, 233, 254, 245, 246, 247, 258, 249, 240, 251, 252, 253, 244, 255, 256, 257, 268, 269, 260, 261, 262, 263, 264, 265, 266, 267, 248, 259, 270, 271, 282, 273, 284, 275, 276, 277, 278, 279, 280, 281, 272, 283, 274, 285, 296, 287, 288, 299, 300, 291, 292, 293, 294, 295, 286, 307, 298, 289, 290, 301, 312, 303, 304, 305, 306, 317, 308, 309, 310, 311, 302, 313, 314, 315, 316, 297, 318, 319, 320, 341, 332, 323, 324, 325, 336, 327, 338, 329, 330, 331, 322, 333, 334, 345, 346, 347, 328, 339, 340, 321, 342, 343, 344, 335, 326, 337, 348, 359, 350, 361, 352, 353, 364, 355, 356, 357, 358, 349, 360, 351, 362, 363, 354, 365, 366, 367, 378, 369, 370, 371, 372, 373, 374, 375, 376, 377, 368, 379, 380, 381, 392, 383, 384, 385, 386, 387, 388, 399, 400, 391, 382, 393, 394, 395, 396, 397, 398, 389, 390, 401, 402, 403, 404, 405, 416, 407, 408, 409, 410, 411, 412, 423, 414, 415, 406, 417, 418, 419, 420, 421, 422, 413, 424, 425, 426, 427, 438, 429, 430, 441, 432, 433, 434, 435, 436, 437, 428, 439, 440, 451, 442, 443, 444, 445, 446, 447, 458, 449, 450, 431, 452, 453, 454, 455, 456, 457, 448, 459, 460, 471, 462, 473, 464, 475, 476, 477, 468, 469, 470, 461, 492, 463, 474, 465, 466, 467, 488, 479, 480, 491, 472, 483, 494, 485, 486, 487, 478, 489, 490, 481, 482, 493, 484, 495, 496, 497, 498, 499};


    POLICY bx = arrcreate(500);
    for(int i = 0; i<bx.node.Arr->size; i++){
        bx.node.Arr->data[i].type = DF_ELEMENT_TInt;
        bx.node.Arr->data[i].node.Int = i;
    }

    NEIGHBORHOOD_STRUCTURES N1 = neistructs(3);
    N1[0] = s1;
    N1[1] = s2;
    N1[2] = s3;

    NEIGHBORHOOD_STRUCTURES N2 = neistructs(3);
    N2[0] = s4;
    N2[1] = s5;
    N2[2] = s6;

    POLICY x = GVNS(bx, N1, N2, 3, 3, 100);

    OPT_VAL ov = vns_config.f(x); 
    printf(" %d ", ov.node.Int);


    arrfree(&x);
    free(N1);
    free(N2);
    
    arrfree(&bx);
    fclose(fds);
    df_free(vns_config.ds,0);


    exit(EXIT_SUCCESS);

}