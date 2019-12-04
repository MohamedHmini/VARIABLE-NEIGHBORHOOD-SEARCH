#include<stdio.h>
#include<stdlib.h>
#include <stdarg.h>
#include "test.h"

int *i = NULL;
// int fun(int (*sf)(int), int i){
//     return sf(i);
// }


// int subfun(int i){
//     return i;
// }


// typedef int(**fs)();


// int *p = NULL;


// void main(){
//     // int (*f)(int (*sf)(int), int) = fun;
//     // printf("%d", f(subfun, 20));

//     fs f = malloc(sizeof(int (*)()) * 100);
//     f[99] = &subfun;
//     printf("%d", f[99](10));
// }