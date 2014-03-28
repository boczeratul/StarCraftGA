/***************************************************************************
                          myrand.cc  -  description
                             -------------------
    begin                : Sep 24 2001
    copyright            : (C) 2001 by Tian-Li Yu
    email                : tianliyu@illigal.ge.uiuc.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "myrand.h"

#ifdef PI
#undef PI
#endif

#define PI 3.14159265
#define N 624

void initRand () {

    unsigned long init_key[N];

    srand ((unsigned long) time (NULL));

    for (int i = 0; i < N; i++)
        init_key[i] = (unsigned long) time (NULL) * rand ();

    init_by_array (init_key, N);
}




bool flip (double prob)
{
    return (uniform () < prob);
}


/** From [0,1) */
double uniform ()
{
    return genrand_res53 ();
}


/** From [a,b) */
double uniform (double a, double b)
{
    return uniform () * (b - a) + a;
}


/** Int From [a,b] */
int uniformInt (int a, int b)
{
    return (a + (int) (uniform () * (b - a + 1)));
}


/** Generate a random array of size num, from [a,b] */
/** num <= b-a+1 */
void uniformArray (int *array, int num, int a, int b)
{

    int *base = new int[b - a + 1];
    int i;
    int r;

    for (i = 0; i < b - a + 1; i++)
        base[i] = a + i;

    for (i = 0; i < num; i++) {
        r = uniformInt (0, b - a - i);
        array[i] = base[r];
        base[r] = base[b - a - i];
    }

    delete[]base;
}
