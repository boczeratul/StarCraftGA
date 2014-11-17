/* 
 * File:   main.cpp
 * Author: rick
 *
 * Created on June 4, 2011, 10:56 AM
 */

#include <cstdlib>
#include "SCTools.h"
#include "ga.h"
#include "ga_var.h"

using namespace std;

int main(int argc, char** argv) {
    
    setSCStateStrings();
    setSCActionVariables();
    setSCDependencies();
        
    int* r = (int*)calloc(60, sizeof(int));

    readRequest(r);
    processRequest(r);

    //initGA (int n_ell, int n_nInitial, int n_selectionPressure, double n_pc, double n_pm, int n_maxGen, int n_maxFe, int optimalFit)
    initGA (5, 1000, 2, 0.5, 0.5, -1, -1, 0);

    //printPopulation(population);
    
    //doIt (int optimalFit, int maxGeneration, int maxNFe, bool output)
    doIt (0, verificationTime, -1, true);
    
    free(r);
    
    system("pause");
    return 0;
}

