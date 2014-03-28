#include "ga.h"
#include "myrand.h"
#include <cfloat>
#include <cstring>
#include "limits.h"
#include "stdio.h"
#include <cstdlib>
#include <cmath>
#include "SCTools.h"
#include <time.h>

int ell = 0;                 // chromosome length
int nInitial = 0;            // initial population size
int nCurrent = 0;            // current population size
int selectionPressure = 2;

double pc = 1.0;               // prob of XO
double pm = 0;               // prob of Mutation

int* selectionIndex = 0;
int maxGen = -1;
int maxFe = -1;
int repeat = 1;
int fe = 0;
int generation = 0;
int bestIndex = 0;

int ** population = 0;
int ** offspring = 0;
int* fitness = 0;
int* offspringFitness = 0;

int optimalFitness = 0;

int bestFitness = INT_MAX;
int recordedBestFitness = INT_MAX;
int recordMaker = 0;
double avgFitness = DBL_MAX;


int totalRequest = 0; // min chromosome length
int dontMindAction[61]; // [0] is how many
int requestAction[61]; // [0] is how many
int requests[60];

int dontMindTotalProb = 0;
clock_t start, end;


int** initChromosomes (int size, int ell) {
    int ** chromosomes = new int *[size];

    for ( int i = 0; i < size; ++i )
    {
        chromosomes[i] = new int[MAX_LENGTH+1];
        chromosomes[i][0] = ell;
    }

    return chromosomes;
}


// called after processRrquest
void initGA (int n_ell, int n_nInitial, int n_selectionPressure, double n_pc,
                double n_pm, int n_maxGen, int n_maxFe, int optimalFit) {
   
    ell = n_ell;
    nInitial = n_nInitial;
    nCurrent = nInitial;
    selectionPressure = n_selectionPressure;
    pc = n_pc;
    pm = n_pm;
    maxGen = n_maxGen;
    maxFe = n_maxFe;
    optimalFitness = optimalFit;
    
    population = initChromosomes (nInitial, ell);
    offspring = initChromosomes (nInitial, ell);
    fitness = new int[nInitial];
    offspringFitness = new int[nInitial];
    selectionIndex = new int[nInitial];

    initRand();
    
    dontMindTotalProb = 0;
    for ( int i = 1; i < dontMindAction[0]+1; ++i   )
    {
        dontMindTotalProb = dontMindTotalProb + actionProb[dontMindAction[i]];
    }
    
    
    //initializePopulation ();
    initializePopulationByRequest ();
    
//    printPopulation(population);

    //printf("here!!!\n");
    
    evaluate( population,  fitness);
}

//called after initGA
void initializePopulation () {
    int i, j;
    double p = 0.5;

    for (i = 0; i < nInitial; ++i)
        for (j = 1; j < ell+1; ++j)
            if (uniform () > p)
                population[i][j] = 1;
            else
                population[i][j] = 0;
}


void initializePopulationByRequest () {
    for ( int i = 0; i < nCurrent; ++i )
    {
        generateChromosomeByRequest(population[i]);
    }
}


void evaluate( int** group,  int* fitnessArray ) {
    //printPopulation(group);
    
    evaluateBuildOrder( group,  fitnessArray );
}


void oneMax( int** group,  int* fitnessArray ) {
    for ( int i = 0; i < nCurrent; ++i )
    {
        int totalOne = 0;
        for ( int j = 0; j < group[i][0]; ++j )
        {
            totalOne = totalOne + group[i][j+1];
        }

        fitnessArray[i] = totalOne;
    }

}



void evaluateBuildOrder( int** group,  int* fitnessArray) {
    for ( int i = 0; i < nCurrent; ++i )
    {
        //printChromosome(group[i]);
        //fflush(NULL);
        fitnessArray[i] = testBuildOrder(group[i]);
    }
}



void selection () {
    tournament();
}


void tournament () {
    int i, j;

    // Adjusting population size
    int nNextGeneration = nCurrent;

//    if (nNextGeneration != nCurrent) {
//        delete[]selectionIndex;
//        delete[]offspring;
//        selectionIndex = new int[nNextGeneration];
//        offspring = new Chromosome[nNextGeneration];
//
//        for (i = 0; i < nNextGeneration; i++)
//            offspring[i].init (ell);
//    }

    int randArray[selectionPressure * nNextGeneration];

    int q = (selectionPressure * nNextGeneration) / nCurrent;
    int r = (selectionPressure * nNextGeneration) % nCurrent;

    for (i = 0; i < q; i++)
        uniformArray (randArray + (i * nCurrent), nCurrent, 0, nCurrent - 1);

    uniformArray (randArray + (q * nCurrent), r, 0, nCurrent - 1);


    for (i = 0; i < nNextGeneration; i++)
    {

        int winner = 0;
        double winnerFitness = DBL_MAX;

        for (j = 0; j < selectionPressure; j++)
        {
            int challenger = randArray[selectionPressure * i + j];
            int challengerFitness = fitness[challenger];

            if (challengerFitness < winnerFitness)
            {
                winner = challenger;
                winnerFitness = challengerFitness;
            }

        }
        selectionIndex[i] = winner;
    }
}

// p = q
void copyChromosome(int* p, int* q) {
    memcpy ( p, q,  sizeof(int)*(MAX_LENGTH+1) );    
}


void crossover () {
    int i;
    int nNextGeneration = nCurrent;
    if ((nNextGeneration & 0x1) == 0) 
    {
    	// nNextGeneration is even

        for (i = 0; i < nNextGeneration; i += 2)
            pairwiseXO (population[selectionIndex[i]], population[selectionIndex[i + 1]],
                offspring[i], offspring[i + 1]);

    }
    else 
    {
        for (i = 0; i < nNextGeneration - 1; i += 2)
        {
            pairwiseXO (population[selectionIndex[i]], population[selectionIndex[i + 1]],
                offspring[i], offspring[i + 1]);
        }
        copyChromosome ( offspring[nNextGeneration - 1] , population[selectionIndex[nNextGeneration - 1]] );
    }

}


void pairwiseXO (int* p1, int* p2, int* c1, int* c2) {
    if (uniform () < pc) 
    {
            onePointXO (p1, p2, c1, c2);
//      uniformXO (p1, p2, c1, c2, 0.5);
    }
    else 
    {
        copyChromosome( c1 , p1);
        copyChromosome( c2 , p2);
    }
}


void onePointXO (int* p1, int* p2, int* c1, int* c2) {
//    int i;
    int crossSite1, crossSite2;
    decideCrossSite(p1, p2, crossSite1, crossSite2);   // the crossSite-th gene will be changed
    
    if ( crossSite1 + p2[0] - crossSite2 > MAX_LENGTH  ) //c1[0]
    {
        generateChromosomeByRequest(c1);
    }
    else
    {
        memcpy(c1+1, p1+1, sizeof(int)*crossSite1 );
        memcpy(c1+1+crossSite1, p2+1+crossSite2, sizeof(int)*(p2[0]-crossSite2) );
        c1[0] = crossSite1 + p2[0] - crossSite2;
    }
    
    if ( crossSite2 + p1[0] - crossSite1 > MAX_LENGTH ) //c2[0]
    {
        generateChromosomeByRequest(c2);
    }
    else
    {
        memcpy(c2+1, p2+1, sizeof(int)*crossSite2 );
        memcpy(c2+1+crossSite2, p1+1+crossSite1, sizeof(int)*(p1[0]-crossSite1) );
        c2[0] = crossSite2 + p1[0] - crossSite1;
    }
    
    
    
    //memcpy(c1+1, p1+1, sizeof(int)*crossSite1 );
    //memcpy(c2+1, p2+1, sizeof(int)*crossSite2 );

    
    //memcpy(c1+1+crossSite1, p2+1+crossSite2, sizeof(int)*(p2[0]-crossSite2) );
    //memcpy(c2+1+crossSite2, p1+1+crossSite1, sizeof(int)*(p1[0]-crossSite1) );
    

    
    //c1[0] = crossSite1 + p2[0] - crossSite2;
    //c2[0] = crossSite2 + p1[0] - crossSite1;
}

// return the index of the array ( do not plus [0] )
int decideCrossSite(int* p1, int* p2, int& crossSite1, int& crossSite2) {    
    int ratio = uniform();
    int length = p1[0];
    crossSite1 =  1 + length * ratio ;    
    length = p2[0];
    crossSite2 =  1 + length * ratio ;
    return 0;
}


void fullReplace () {
        for ( int i = 0; i < nCurrent; ++i)
        {
                copyChromosome( population[i] , offspring[i] );
        }
}

void RTR () {

    int i, j;

 
     int windowSize = nCurrent/20;
//    int windowSize = ( nCurrent < ell / 20) ? nCurrent : ell / 20;    // error  :  windowSize may be 0!!

    // Rick modified
    if ( windowSize == 0 )
    {
        fullReplace ();
        return;
    }
    // end Rick


    int *randArray = new int[windowSize];

    for (i = 0; i < nCurrent; ++i)
    {
        int index = 0 ;
        int distance;
        int minDistance = INT_MAX;                // max distance
        int maxFitness = 0;

        uniformArray (randArray, windowSize, 0, nCurrent - 1);

        for (j = 0; j < windowSize; j++)
        {
            //distance = getBBDistance (offspring[i], population[randArray[j]]);
            distance = getDistance (offspring[i], population[randArray[j]]);
            if (distance < minDistance)
            {
                index = randArray[j];
                minDistance = distance;
                maxFitness = fitness[index];
            }
            else if (distance == minDistance && maxFitness < fitness[randArray[j]] )
            {
                index = randArray[j];
                maxFitness = fitness[index];
            }
        }

        

        if ( offspringFitness[i] < fitness[index] ||
           (offspringFitness[i] == fitness[index] && offspring[i][0] < population[index][0] ) )
        {
            copyChromosome( population[index] , offspring[i] ) ;
            fitness[index] = offspringFitness[i];
            if ( SHOW_REPLACEMENT )
            {
                printf ("Replacing (%d) with ", index);
                //offspring[i].printOut ();
                printf ("\n");
            }
        }
        else if (SHOW_REPLACEMENT)
        {
            {
                printf ("Not replacing (%d) with offspring %d",  index, i);
                //offspring[i].printOut ();
                printf ("\n");
            }
        }
    }
    
    delete [] randArray;
}


int getDistance (int* & c1, int* & c2) {

    int distance = 0;

    for ( int i = 1; i < c1[0]+1; ++i )
        distance =  distance + abs(c1[i] - c2[i]);  // hamming distance

    return distance;
}

void mutation () {
    //simpleMutation ();
    mutationClock ();
}

void mutationClock () {
    
    int nNextGeneration = nCurrent;
//    
//    if (pm <= 1e-6) return; // can't deal with too small pm
//
//    int pointer = (int) (log(1-uniform()) / log(1-pm) + 1);
//
//    while (pointer < nNextGeneration * ell) 
//    {
//        int q = pointer / ell;
//        int r = pointer % ell;
//
//        // for oneMax
//        //int val = offspring[q][r];
//        //offspring[q][r] = 1-val ;      
//        
//        // for SC
//        mutateChromosome(offspring[q], r);
//        
//        
//        
//
//        // Compute next mutation clock
//        pointer += (int) (log(1-uniform()) / log(1-pm) + 1);
//    }
    
    for ( int i = 0; i < nNextGeneration; ++i )
    {
        if (uniform () < pm)
            mutateChromosome(offspring[i], uniformInt(1, offspring[i][0]));
    }
}


void mutateChromosome(int* chromosome, int r ) {

    double roll = uniform();
    double swapP = 0.33;
    double deleteP = 0.66;
    //double addP = 1;
    
    if ( roll < swapP && r != chromosome[0] && r != 0 )
    {
        // swap
        int temp = chromosome[r+1];
        chromosome[r+1] = chromosome[r];
        chromosome[r] = temp;
    }
    else if (roll < deleteP)
    {
        // delete dont mind
        int action = chromosome[r];
        for ( int i = 1; i < dontMindAction[0]+1; ++i )
        {
            if ( dontMindAction[i] == action )
            {
                // copy 
                for ( int w = r; w < chromosome[0]; ++w )
                {
                    chromosome[w] = chromosome[w+1];
                }
                --chromosome[0];
                break;
            }
        }
    }
    else
    {
        // insert dont mind
        if ( chromosome[0] < MAX_LENGTH-2)
        {


            int pivot =  uniformInt (0, dontMindTotalProb-1); //[0, dontMindTotalProb-1]
            int index = 1;
            while ( pivot > 0 )
            {
                pivot = pivot - actionProb[dontMindAction[index]];
                ++index;
            }
            int action = dontMindAction[index];

            // copy 
            for ( int i = chromosome[0]; i > r-1; --i )
            {
                chromosome[i+1] = chromosome[i];
            }
            chromosome[r] = action;

            ++chromosome[0];

        }
    }
}




int doIt (int optimalFit, int maxGeneration, int maxNFe, bool output) {
    
    //printf("start\n");
    //fflush(NULL);
    generation = 0;
    bestFitness = INT_MAX;
    recordedBestFitness = INT_MAX;
    recordMaker = 0;
    avgFitness = INT_MAX;
    optimalFitness = optimalFit;
    maxGen = maxGeneration;
    maxFe = maxNFe;

    start = clock();
    while ( !shouldTerminate () )
    {
        oneRun (output);
    }
    end = clock();
    
    demoBuildOrder(population[bestIndex]);
    printf("\n## It took us %.3f seconds to find the BuildOrder! ##\n", double(end - start)/CLOCKS_PER_SEC);
    return generation;
}

bool shouldTerminate () {
    
    bool termination = false;

    if (maxFe != -1)
    {
        if (fe > maxFe)
            termination = true;
    }

//    if (maxGen != -1)
//    {
//        if (generation > maxGen)
//            termination = true;
//    }
    
    if (bestFitness < recordedBestFitness){
        recordedBestFitness = bestFitness;
        recordMaker = generation;
    }
    
    if (generation - recordMaker > maxGen)
        termination = true;
//
//    if ( optimalFitness >= bestFitness )
//        termination = true;

    return termination;

}


void oneRun (bool output) {
    
    selection ();
//    printf("1\n");
    
//    printf("\n\n\nBefore crossover\n");
//    printf("population\n");
//    printPopulation(population);

    
    crossover ();
//    printf("2\n");
//
//    printf("\n\n\nAfter crossover\n");
//    printf("population\n");
//    printPopulation(population);
//    printf("offspring\n");
//    printPopulation(offspring);
    
    
    mutation ();
//    printf("3\n");
//    
//    printf("\n\n\nAfter mutation\n");
//    printf("population\n");
//    printPopulation(population);
//    printf("offspring\n");
//    printPopulation(offspring);
    
////    printf("Before modifiction\n");
////    printf("population\n");
////    printPopulation(population);
////    printf("offspring\n");
////    printPopulation(offspring);
    
    modifyChromosome();
//    printf("4\n");
        
    evaluate( offspring,  offspringFitness);
//    printf("5\n");
//    
//    printf("\n\n\nAfter modifiction+evaluation\n");
//    printf("\n\npopulation\n");
//    printPopulationAndFitness(population);
    
//    printf("\n\noffspring\n");
//    printPopulationAndFitness(offspring);
    
    
    RTR();
//    printf("6\n");
    
    
    
    evaluate( population,  fitness);
//    printf("7\n");
    
//    printf("\n\n\nAfter RTR\n");
//    printf("\n\npopulation\n");
//    printPopulationAndFitness(population);

    int min = INT_MAX;
    int total = 0;
    
    for ( int i = 0; i < nCurrent; ++i )
    {
        int fit = fitness[i];
        total = total + fit;
        if (fit < min)
        {
            min = fit;
            bestIndex = i;
        }        
    }
    
    avgFitness =  total / nCurrent;
    bestFitness = min;

    if (output)
        showStatistics ();

    ++generation;
}


void showStatistics () {

    printf ("Gen:%d  Min BuildOrder Time:%d  \n",  generation, bestFitness);
    //printf ("best BuildOrder: ");
    //printChromosome( population[bestIndex] );
    //printf ("\n");

    fflush(NULL);
}

void printChromosome(int* chromosome) {
    for ( int i = 1; i < chromosome[0]+1; ++i )
    {
        printf("%d ", chromosome[i]);
    }
    printf("  (length=%d)        ", chromosome[0]);    
}

void printFitness(int populationOrOffspring , int ith) {
    if ( populationOrOffspring == 0)
        printf("  (score=%d)", fitness[ith]);    
    else
        printf("  (score=%d)", offspringFitness[ith]);   
}



void printPopulation(int** group) {
    for ( int i = 0; i < nCurrent; ++i )
    {
        printChromosome(group[i]);
        printf("\n");
    }
}

void printPopulationAndFitness(int** group) {
    if ( group == population )
    {
        for ( int i = 0; i < nCurrent; ++i )
        {
            printChromosome(group[i]);
            printFitness(0, i);
            printf("\n");
        }
    }
    else
    {
        for ( int i = 0; i < nCurrent; ++i )
        {
            printChromosome(group[i]);
            printFitness(1, i);
            printf("\n");
        }
    }
    
}


void processRequest(int request[60]) {
    totalRequest = 0;
    memcpy(requests, request, sizeof(int)*60);

    dontMindAction[0] = 0;
    requestAction[0] = 0;

    for ( int i = 0; i < 60; ++i )
    {
        if ( request[i] > 0 )
        {
            totalRequest = totalRequest + request[i];
            requestAction[requestAction[0]+1] = i;
            ++requestAction[0];
        }
        else if ( request[i] == -1 )
        {
            dontMindAction[dontMindAction[0]+1] = i;
            ++dontMindAction[0];
        }
    }
    
//    printf("requests :\n");
//    for ( int i = 0; i < 60; ++i )
//    {
//        printf("(%d,  %d)   ", i, requests[i]);
//    }
//    printf("\n\n\n\nrequestAction :\n");
//    for ( int i = 1; i < requestAction[0]+1; ++i   )
//    {
//        printf("%d  ", requestAction[i]);
//    }
//    printf("\n\n\n\ndontMindAction :\n");
//    for ( int i = 1; i < dontMindAction[0]+1; ++i   )
//    {
//        printf("%d  ", dontMindAction[i]);
//    }
    
    
    

}


//// must called after processRequest
//void generateChromosomeByRequest(int* chromosome) {
//
//    chromosome[0] = uniformInt(totalRequest, MAX_LENGTH); //[totalRequest,MAX_LENGTH]
//
//    for ( int i = 1; i < chromosome[0]+1; ++i )
//    {
//        chromosome[i] = dontMindAction[uniformInt(1,dontMindAction[0])]; // [1,dontMindAction[0]]
//    }
//
//    int* requestedSites = (int*)malloc(sizeof(int)*totalRequest);
//    uniformArray (requestedSites, totalRequest, 1, chromosome[0]);  // [1,chromosome[0]]
//
//    int done = 0;
//    int index = 1;
//    int max = 0;
//    while (done < totalRequest)
//    {
//        for ( int i = 0; i < requests[requestAction[index]]; ++i )
//        {
//            chromosome[requestedSites[done]] = requestAction[index];
//            if ( requestedSites[done] > max )
//                max = requestedSites[done];
//            ++done;
//        }
//        ++index;
//    }
//
//    chromosome[0] = max;
//
//    free(requestedSites);
//}



void generateChromosomeByRequest(int* chromosome)
{
    int i, j, length = 0;
    int actionCounts[60];
    int actionAvailable[60];
    
    int actionRW[60];
    int totalRW, roll;
    
    for (i = 0; i < 60; i++)
    {
        actionCounts[i] = 0;
        actionAvailable[i] = 0;
    }
    
    while(1) {
        totalRW = 0;
        for (i = 0; i < 60; i++)
        {
            if (requests[i] == -1 || requests[i] - actionCounts[i] > 0)
            {
                actionAvailable[i] = 1;
                for (j = 0; j < 60; j++)
                {
                    if (actionDependency[i][j] && actionCounts[j] == 0)
                        actionAvailable[i] = 0;
                }
            }
            else
                actionAvailable[i] = 0;
            
            if (actionAvailable[i])
            {
                totalRW += actionProb[i];
                actionRW[i] = totalRW;
            }
            else
                actionRW[i] = totalRW;
        }
        
        roll = rand() % totalRW;
        for (i = 0; i < 60; i++)
            if (roll < actionRW[i])
            {
                chromosome[length + 1] = i;
                length++;
                actionCounts[i]++;
                break;
            }
        
        int allReqMet = 1;
        
        for (i = 0; i < 60; i++)
            if (requests[i] > actionCounts[i])
                allReqMet = 0;
        
        if (allReqMet || length == 249)
        {
            chromosome[0] = length;
            break;
        }
    }
}


void modifyChromosomeByRequest(int* chromosome) {
    int count[60];
    int toAdd[MAX_LENGTH];
    int total2Add = 0;
    
    int tempChromosome[MAX_LENGTH+1];
    int length = 0;
    
    for ( int i = 0; i < 60; ++ i )
    {
        count[i] = 0;
    }
    
    // handle more
    for ( int i = 1; i < chromosome[0]+1; ++i )
    {
        int action = chromosome[i];
        if ( count[action] < requests[action]  )  // only take care requirements
        {
            tempChromosome[length] = action;
            ++length;
            ++count[action];
        }
        else if ( requests[action] == -1 )
        {
            tempChromosome[length] = action;
            ++length;
        }
    }
//    printf("a\n");
    
    //handle less
    for ( int i = 0; i < 60; ++i )
    {
        for ( int j = 0; j < requests[i]-count[i]; ++j )
        {
            toAdd[total2Add] = i;
            ++total2Add;
        }
    }
    
    
    chromosome[0] = length+total2Add;
    
    if ( chromosome[0] > MAX_LENGTH )
    {
        generateChromosomeByRequest(chromosome);        
        return;
    }
    
//    printf("b\n");
    
    int* toAddIndex = (int*) malloc(sizeof(int)*(total2Add));
    uniformArray (toAddIndex, total2Add,  1, chromosome[0]);
    
    
    int max = 0;    
    
    for ( int i = 1; i < chromosome[0]+1; ++i )
    {
        chromosome[i] = -1;        
    }
    
//    printf("c\n");
    //fill in toAdd
    for ( int i = 0; i < total2Add; ++i )
    {
        chromosome[toAddIndex[i]] = toAdd[i];
        
        //handle crop end
        if ( toAddIndex[i] > max )
        {
            max = toAddIndex[i];        
        }
    }
    
    // the last one be request
    if ( max != 0 && chromosome[0] != max)
    {
        chromosome[chromosome[0]] = chromosome[max];
        chromosome[max] = -1;
        max = chromosome[0];
    }
    
    //fill in original
    int pivot = 0;
//    printf("d\n");
    
    for ( int i = 1; i < chromosome[0]+1; ++i )
    {
        if ( chromosome[i] == -1 )
        {
            chromosome[i] = tempChromosome[pivot];
            ++pivot;
            
            // handle crop end
            if ( i > max )
            {
                for ( int a = 1; a < requestAction[0]+1; ++a )
                {
                    if ( chromosome[i] == requestAction[a] )
                    {
                        max = i;
                        break;
                    }
                }                
            }
        }
    }
    
//    printf("e\n");
    
    chromosome[0] = max;
    
    free(toAddIndex);
    
}


void modifyChromosome() {
    for ( int i = 0; i < nCurrent; ++i )
    {
        //printf("chromosome %d = ", i);
        //printChromosome(offspring[i]);
        modifyChromosomeByRequest(offspring[i]);
    }
}
