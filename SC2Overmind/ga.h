/* 
 * File:   ga.h
 * Author: rick
 *
 * Created on June 4, 2011, 11:24 AM
 */

#ifndef GA_H
#define	GA_H

#define MAX_LENGTH 249

#define SHOW_REPLACEMENT false

int** initChromosomes (int size, int ell);

void initializePopulation ();

void initGA (int n_ell, int n_nInitial, int n_selectionPressure, double n_pc, double n_pm, int n_maxGen, int n_maxFe, int optimalFit);

void evaluate( int** group,  int* fitnessArray);

void oneMax( int** group,  int* fitnessArray);

void evaluateBuildOrder( int** group,  int* fitnessArray);

void selection();

void tournament ();

void copyChromosome(int* p, int* q);

void crossover();

void pairwiseXO (int* p1, int* p2, int* c1, int* c2);

void onePointXO (int* p1, int* p2, int* c1, int* c2);

int decideCrossSite(int* p1, int* p2, int& crossSite1, int& crossSite2);


void fullReplace ();

void RTR ();

int getDistance (int* & c1, int* & c2);

void mutation ();

void mutationClock ();

void mutateChromosome(int* chromosome, int r );

int doIt (int optimalFit, int maxGeneration, int maxNFe, bool output);

bool shouldTerminate ();

void oneRun (bool output);

void showStatistics () ;

void printChromosome(int* chromosome);

void printPopulation(int** group);

void printPopulationAndFitness(int** group);

void printFitness(int populationOrOffspring , int ith);

void processRequest(int request[60]);

void initializePopulationByRequest ();

void generateChromosomeByRequest(int* chromosome);

void modifyChromosomeByRequest(int* chromosome);

void  modifyChromosome();



#endif	/* GA_H */

