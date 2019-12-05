#include<stdio.h>
#include<stdlib.h>
#include <stdarg.h>
#include <time.h>
#define randint(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))





void main(){
    srand(time(NULL));
    int r = randint(10,50);
    printf("%d",r);
}