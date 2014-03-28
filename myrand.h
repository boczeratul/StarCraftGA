
#ifndef MYRAND_H
#define MYRAND_H

#include "mt19937ar.h"

void initRand();

bool flip (double prob = 0.5);

/** From [a,b) */
double uniform (double a, double b);

/** From [0,1) */
double uniform ();

/** Int From [a,b] */
int uniformInt (int a, int b);

/** Generate a random array of size num, from [a,b] */
void uniformArray (int *array, int num, int a, int b);

#endif
