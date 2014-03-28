/* 
 * File:   ga_var.h
 * Author: rick
 *
 * Created on June 4, 2011, 11:37 AM
 */

#ifndef GA_VAR_H
#define	GA_VAR_H

extern int ell;                 // chromosome length
extern int nInitial;            // initial population size
extern int nCurrent;            // current population size
extern int selectionPressure;

extern double pc;               // prob of XO
extern double pm;               // prob of Mutation

extern int* selectionIndex;
extern int maxGen;
extern int maxFe;
extern int repeat;
extern int fe;
extern int generation;
extern int bestIndex;


extern int** population;
extern int** offspring;
extern int* fitness;
extern int* offspringFitness;

extern int optimalFitness;

extern int bestFitness;
extern int recordedBestFitness;
extern int recordMaker;
extern double avgFitness;

extern int totalRequest; // min chromosome length
extern int dontMindAction[61]; // [0] is how many
extern int requestAction[61]; // [0] is how many
extern int requests[60];

extern int dontMindTotalProb;

#endif	/* GA_VAR_H */

