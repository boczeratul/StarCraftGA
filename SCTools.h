/* 
 * File:   SCTools.h
 * Author: Jacky
 *
 * Created on 2011年5月18日, 上午 10:58
 */

#ifndef SCTOOLS_H
#define	SCTOOLS_H

#ifdef	__cplusplus
extern "C" {
#endif

extern int SCVerbose;
    
extern int SCGameTime;
extern int SCWaitTime;
extern int SCState[73];
extern char* SCStateString[73];

extern int actionProb[60];
extern int actionDependency[60][60];

    /* SC2 State (Zerg)
     * 
     * Resources
     * 0: mineral
     * 1: gas
     * 
     * Buildings
     * 2: hatchery
     * 3: extractor
     * 4: evolution chamber
     * 5: spawning pool
     * 6: roach warren
     * 7: baneling nest
     * 8: hydralisk den
     * 9: nydus network
     * 10: infestation pit
     * 11: spire
     * 12: ultralisk cavern
     * 13: spine crawler
     * 14: spore crawler
     * 
     * Units
     * 15: larva
     * 16: drone
     * 17: overlord
     * 18: zergling
     * 19: baneling
     * 20: roach
     * 21: queen
     * 22: hydralisk
     * 23: infestor
     * 24: overseer
     * 25: nydus worm
     * 26: mutalisk
     * 27: corruptor
     * 28: brood lord
     * 29: ultralisk
     *
     * Production Space Used
     * 30: hatchery
     * 31: hatchery larva
     * 32: extractor
     * 33: evolution chamber
     * 34: spawning pool
     * 35: roach warren
     * 36: baneling nest
     * 37: hydralisk den
     * 38: nydus network
     * 39: infestation pit
     * 40: spire
     * 41: ultralisk cavern
     *
     * Tech
     * 42: burrow
     * 43: overlord speed
     * 44: overlord carriage
     * 45: melee L1
     * 46: melee L2
     * 47: melee L3
     * 48: range L1
     * 49: range L2
     * 50: range L3
     * 51: defence L1
     * 52: defence L2
     * 53: defence L3
     * 54: zergling speed
     * 55: zergling attack speed
     * 56: roach speed
     * 57: roach burrow movement
     * 58: baneling speed
     * 59: hydralisk range
     * 60: air attack L1
     * 61: air attack L2
     * 62: air attack L3
     * 63: air defence L1
     * 64: air defence L2
     * 65: air defence L3
     * 66: infestor energy
     * 67: infestor mind control
     * 68: ultralisk defence
     * 69: lair tech
     * 70: hive tech
     * 71: greater spire tech
     *
     * Special
     * 72: egg
     */

extern int SCActionRequirements[60][10][3];
extern int SCActionCosts[60][10][2];
extern int SCActionGains[60][10][2];
extern int SCActionTime[60];
extern char* SCActionString[60];

extern int numOfActions;
extern int Actions[100][2];
extern int SCBuildOrder[250];
extern int SCBuildOrderPointer;
extern int validBO;

extern int verificationTime;
    
    /* SC2 Actions (Zerg)
     *
     * Resource
     * 0: gather gas
     * 1: stop gather gas
     *
     * Building
     * 2: build hatchery
     * 3: build extractor
     * 4: build evolution chamber
     * 5: build spawning pool
     * 6: build roach warren
     * 7: build baneling nest
     * 8: build hydralisk den
     * 9: build nydus network
     * 10: build infestation pit
     * 11: build spire
     * 12: build ultralisk cavern
     * 13: build spine crawler
     * 14: build spore crawler
     *
     * Unit
     * 15: extra larva
     * 16: spawn drone
     * 17: spawn overlord
     * 18: spawn zergling
     * 19: spawn baneling
     * 20: spawn roach
     * 21: spawn queen
     * 22: spawn hydralisk
     * 23: spawn infestor
     * 24: spawn overseer
     * 25: spawn nydus worm
     * 26: spawn mutalisk
     * 27: spawn corruptor
     * 28: spawn brood lord
     * 29: spawn ultralisk
     *
     * Tech
     * 30: upgrade burrow
     * 31: upgrade overlord speed
     * 32: upgrade overlord carriage
     * 33: upgrade melee L1
     * 34: upgrade melee L2
     * 35: upgrade melee L3
     * 36: upgrade range L1
     * 37: upgrade range L2
     * 38: upgrade range L3
     * 39: upgrade defence L1
     * 40: upgrade defence L2
     * 41: upgrade defence L3
     * 42: upgrade zergling speed
     * 43: upgrade zergling attack speed
     * 44: upgrade roach speed
     * 45: upgrade roach burrow movement
     * 46: upgrade baneling speed
     * 47: upgrade hydralisk range
     * 48: upgrade air attack L1
     * 49: upgrade air attack L2
     * 50: upgrade air attack L3
     * 51: upgrade air defence L1
     * 52: upgrade air defence L2
     * 53: upgrade air defence L3
     * 54: upgrade infestor energy
     * 55: upgrade infestor mind control
     * 56: upgrade ultralisk defence
     * 57: upgrade lair tech
     * 58: upgrade hive tech
     * 59: upgrade greater spire tech
     */



    
void initializeGame();
void setSCStateStrings();
void setSCActionVariables();

void getResources();
int supply();
int maxsupply();
void showInformation();
void oneSecInSC(); // run text-based SC for one second
int oneSecInSCNonStop();
int oneSecInSCDemo();

void runSCSimulation();
int testBuildOrder(int buildOrder[250]);
int demoBuildOrder(int buildOrder[250]);

int isValidAction(int action);
void performAction(int action);
void finishAction(int action);

void readRequest(int ActionList[60]);
void setSCDependencies();


#ifdef	__cplusplus
}
#endif

#endif	/* SCTOOLS_H */

