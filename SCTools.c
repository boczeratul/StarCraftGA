#include "SCTools.h"
#include <stdio.h>
#include <limits.h>

int SCVerbose;
    
int SCGameTime;
int SCWaitTime;
int SCState[73];
char* SCStateString[73];

int actionDependency[60][60];
int actionProb[60];

int SCActionRequirements[60][10][3];
int SCActionCosts[60][10][2];
int SCActionGains[60][10][2];
int SCActionTime[60];
char* SCActionString[60];

int numOfActions;
int Actions[100][2];
int SCBuildOrder[250];
int SCBuildOrderPointer;
int validBO;

int verificationTime;

void initializeGame()
{
    if (SCVerbose)
    {
        printf("#### Welcome to Text-Based StarCraft 2 ####\n");
        printf("####   You have been spawned as Zerg   ####\n");
    }
    
    SCGameTime = 0;
    SCWaitTime = 0;
    numOfActions = 0;
    
    int i;
    for (i = 0; i < 73; i++)
        SCState[i] = 0;

    SCState[0] = 50; // initial minerals
    SCState[2] = 1; // initial hatchery
    SCState[15] = 3; // initial larvae
    SCState[16] = 6; // initial drones
    SCState[17] = 1; // initial overlord
}

void setSCStateStrings()
{
    SCStateString[0] = "mineral";
    SCStateString[1] = "gas";
    
    SCStateString[2] = "hatchery";
    SCStateString[3] = "extractor";
    SCStateString[4] = "evolution chamber";
    SCStateString[5] = "spawning pool";
    SCStateString[6] = "roach warren";
    SCStateString[7] = "baneling nest";
    SCStateString[8] = "hydralisk den";
    SCStateString[9] = "nydus network";
    SCStateString[10] = "infestation pit";
    SCStateString[11] = "spire";
    SCStateString[12] = "ultralisk cavern";
    SCStateString[13] = "spine crawler";
    SCStateString[14] = "spore crawler";
    
    SCStateString[15] = "larva";
    SCStateString[16] = "drone";
    SCStateString[17] = "overlord";
    SCStateString[18] = "zergling";
    SCStateString[19] = "baneling";
    SCStateString[20] = "roach";
    SCStateString[21] = "queen";
    SCStateString[22] = "hydralisk";
    SCStateString[23] = "infestor";
    SCStateString[24] = "overseer";
    SCStateString[25] = "nydus worm";
    SCStateString[26] = "mutalisk";
    SCStateString[27] = "corruptor";
    SCStateString[28] = "brood lord";
    SCStateString[29] = "ultralisk";
    
    SCStateString[30] = "processes at hatchery";
    SCStateString[31] = "breeding larva ingections";
    SCStateString[32] = "drones at extractor";
    SCStateString[33] = "processes at evolution chamber";
    SCStateString[34] = "processes at spawning pool";
    SCStateString[35] = "processes at roach warren";
    SCStateString[36] = "processes at baneling nest";
    SCStateString[37] = "processes at hydralisk den";
    SCStateString[38] = "processes at nydus network";
    SCStateString[39] = "processes at infestation pit";
    SCStateString[40] = "processes at spire";
    SCStateString[41] = "processes at ultralisk cavern";

    SCStateString[42] = "burrow";
    SCStateString[43] = "overlord speed";
    SCStateString[44] = "overlord carriage";
    SCStateString[45] = "melee attack L1";
    SCStateString[46] = "melee attack L2";
    SCStateString[47] = "melee attack L3";
    SCStateString[48] = "range attack L1";
    SCStateString[49] = "range attack L2";
    SCStateString[50] = "range attack L3";
    SCStateString[51] = "defence L1";
    SCStateString[52] = "defence L2";
    SCStateString[53] = "defence L3";
    SCStateString[54] = "zergling speed";
    SCStateString[55] = "zergling attack speed";
    SCStateString[56] = "roach speed";
    SCStateString[57] = "roach burrow movement";
    SCStateString[58] = "baneling speed";
    SCStateString[59] = "hydralisk attack range";
    SCStateString[60] = "air attack L1";
    SCStateString[61] = "air attack L2";
    SCStateString[62] = "air attack L3";
    SCStateString[63] = "air defence L1";
    SCStateString[64] = "air defence L2";
    SCStateString[65] = "air defence L3";
    SCStateString[66] = "infestor energy";
    SCStateString[67] = "infestor mind control";
    SCStateString[68] = "ultralisk defence";
    SCStateString[69] = "lair tech";
    SCStateString[70] = "hive tech";
    SCStateString[71] = "greater spire tech";
}

void setSCActionVariables()
{
    // 0: gather gas
    SCActionRequirements[0][0][0] = 1;
    SCActionRequirements[0][1][0] = 32;
    SCActionRequirements[0][1][1] = 3; // number of workers at gas
    SCActionRequirements[0][1][2] = 3; // vaspine gas

    SCActionCosts[0][0][0] = 1;
    SCActionCosts[0][1][0] = 32;
    SCActionCosts[0][1][1] = -1;

    SCActionGains[0][0][0] = 0;

    SCActionTime[0] = 1;
    SCActionString[0] = "Send a drone to gas";

    // 1: stop gather gas
    SCActionRequirements[1][0][0] = 1;
    SCActionRequirements[1][1][0] = 32;
    SCActionRequirements[1][1][1] = 1;
    SCActionRequirements[1][1][2] = 0;

    SCActionCosts[1][0][0] = 1;
    SCActionCosts[1][1][0] = 32;
    SCActionCosts[1][1][1] = 1;

    SCActionGains[1][0][0] = 0;

    SCActionTime[1] = 1;
    SCActionString[1] = "Pull a drone from gas";

    // 2: build hatchery
    SCActionRequirements[2][0][0] = 2;
    SCActionRequirements[2][1][0] = 0;
    SCActionRequirements[2][1][1] = 300;
    SCActionRequirements[2][1][2] = 0;
    SCActionRequirements[2][2][0] = 16;
    SCActionRequirements[2][2][1] = 1;
    SCActionRequirements[2][2][2] = 0;

    SCActionCosts[2][0][0] = 2;
    SCActionCosts[2][1][0] = 16;
    SCActionCosts[2][1][1] = 1;
    SCActionCosts[2][2][0] = 0;
    SCActionCosts[2][2][1] = 300;

    SCActionGains[2][0][0] = 1;
    SCActionGains[2][1][0] = 2;
    SCActionGains[2][1][1] = 1;

    SCActionTime[2] = 100;
    SCActionString[2] = "Build a hatchery";

    // 3: build extractor
    SCActionRequirements[3][0][0] = 2;
    SCActionRequirements[3][1][0] = 0;
    SCActionRequirements[3][1][1] = 25;
    SCActionRequirements[3][1][2] = 0;
    SCActionRequirements[3][2][0] = 16;
    SCActionRequirements[3][2][1] = 1;
    SCActionRequirements[3][2][2] = 0;

    SCActionCosts[3][0][0] = 2;
    SCActionCosts[3][1][0] = 16;
    SCActionCosts[3][1][1] = 1;
    SCActionCosts[3][2][0] = 0;
    SCActionCosts[3][2][1] = 25;

    SCActionGains[3][0][0] = 1;
    SCActionGains[3][1][0] = 3;
    SCActionGains[3][1][1] = 1;

    SCActionTime[3] = 30;
    SCActionString[3] = "Build an extractor";

    // 4: build evolution chamber
    SCActionRequirements[4][0][0] = 3;
    SCActionRequirements[4][1][0] = 0;
    SCActionRequirements[4][1][1] = 75;
    SCActionRequirements[4][1][2] = 0;
    SCActionRequirements[4][2][0] = 16;
    SCActionRequirements[4][2][1] = 1;
    SCActionRequirements[4][2][2] = 0;
    SCActionRequirements[4][3][0] = 2;
    SCActionRequirements[4][3][1] = 1;
    SCActionRequirements[4][3][2] = 0;
    
    SCActionCosts[4][0][0] = 2;
    SCActionCosts[4][1][0] = 16;
    SCActionCosts[4][1][1] = 1;
    SCActionCosts[4][2][0] = 0;
    SCActionCosts[4][2][1] = 75;
    
    SCActionGains[4][0][0] = 1;
    SCActionGains[4][1][0] = 4;
    SCActionGains[4][1][1] = 1;
    
    SCActionTime[4] = 35;
    SCActionString[4] = "Build an evolution chamber";
    
    // 5: build spawning pool
    SCActionRequirements[5][0][0] = 3;
    SCActionRequirements[5][1][0] = 0;
    SCActionRequirements[5][1][1] = 200;
    SCActionRequirements[5][1][2] = 0;
    SCActionRequirements[5][2][0] = 16;
    SCActionRequirements[5][2][1] = 1;
    SCActionRequirements[5][2][2] = 0;
    SCActionRequirements[5][3][0] = 2;
    SCActionRequirements[5][3][1] = 1;
    SCActionRequirements[5][3][2] = 0;

    SCActionCosts[5][0][0] = 2;
    SCActionCosts[5][1][0] = 16;
    SCActionCosts[5][1][1] = 1;
    SCActionCosts[5][2][0] = 0;
    SCActionCosts[5][2][1] = 200;

    SCActionGains[5][0][0] = 1;
    SCActionGains[5][1][0] = 5;
    SCActionGains[5][1][1] = 1;

    SCActionTime[5] = 65;
    SCActionString[5] = "Build a spawning pool";

    // 6: build roach warren
    SCActionRequirements[6][0][0] = 3;
    SCActionRequirements[6][1][0] = 0;
    SCActionRequirements[6][1][1] = 150;
    SCActionRequirements[6][1][2] = 0;
    SCActionRequirements[6][2][0] = 16;
    SCActionRequirements[6][2][1] = 1;
    SCActionRequirements[6][2][2] = 0;
    SCActionRequirements[6][3][0] = 5;
    SCActionRequirements[6][3][1] = 1;
    SCActionRequirements[6][3][2] = 0;
    
    SCActionCosts[6][0][0] = 2;
    SCActionCosts[6][1][0] = 16;
    SCActionCosts[6][1][1] = 1;
    SCActionCosts[6][2][0] = 0;
    SCActionCosts[6][2][1] = 150;
    
    SCActionGains[6][0][0] = 1;
    SCActionGains[6][1][0] = 6;
    SCActionGains[6][1][1] = 1;
    
    SCActionTime[6] = 55;
    SCActionString[6] = "Build a roach warren";
    
    // 7: build baneling nest
    SCActionRequirements[7][0][0] = 4;
    SCActionRequirements[7][1][0] = 0;
    SCActionRequirements[7][1][1] = 100;
    SCActionRequirements[7][1][2] = 0;
    SCActionRequirements[7][2][0] = 1;
    SCActionRequirements[7][2][1] = 50;
    SCActionRequirements[7][2][2] = 0;
    SCActionRequirements[7][3][0] = 16;
    SCActionRequirements[7][3][1] = 1;
    SCActionRequirements[7][3][2] = 0;
    SCActionRequirements[7][4][0] = 5;
    SCActionRequirements[7][4][1] = 1;
    SCActionRequirements[7][4][2] = 0;
    
    SCActionCosts[7][0][0] = 3;
    SCActionCosts[7][1][0] = 16;
    SCActionCosts[7][1][1] = 1;
    SCActionCosts[7][2][0] = 0;
    SCActionCosts[7][2][1] = 100;
    SCActionCosts[7][3][0] = 1;
    SCActionCosts[7][3][1] = 50;
    
    SCActionGains[7][0][0] = 1;
    SCActionGains[7][1][0] = 7;
    SCActionGains[7][1][1] = 1;
    
    SCActionTime[7] = 60;
    SCActionString[7] = "Build a baneling nest";
    
    // 8: build hydralisk den
    SCActionRequirements[8][0][0] = 4;
    SCActionRequirements[8][1][0] = 0;
    SCActionRequirements[8][1][1] = 100;
    SCActionRequirements[8][1][2] = 0;
    SCActionRequirements[8][2][0] = 1;
    SCActionRequirements[8][2][1] = 100;
    SCActionRequirements[8][2][2] = 0;
    SCActionRequirements[8][3][0] = 16;
    SCActionRequirements[8][3][1] = 1;
    SCActionRequirements[8][3][2] = 0;
    SCActionRequirements[8][4][0] = 69;
    SCActionRequirements[8][4][1] = 2;
    SCActionRequirements[8][4][2] = 0;
    
    SCActionCosts[8][0][0] = 3;
    SCActionCosts[8][1][0] = 16;
    SCActionCosts[8][1][1] = 1;
    SCActionCosts[8][2][0] = 0;
    SCActionCosts[8][2][1] = 100;
    SCActionCosts[8][3][0] = 1;
    SCActionCosts[8][3][1] = 100;
    
    SCActionGains[8][0][0] = 1;
    SCActionGains[8][1][0] = 8;
    SCActionGains[8][1][1] = 1;
    
    SCActionTime[8] = 40;
    SCActionString[8] = "Build a hydralisk den";
    
    // 9: build nydus network
    SCActionRequirements[9][0][0] = 4;
    SCActionRequirements[9][1][0] = 0;
    SCActionRequirements[9][1][1] = 150;
    SCActionRequirements[9][1][2] = 0;
    SCActionRequirements[9][2][0] = 1;
    SCActionRequirements[9][2][1] = 200;
    SCActionRequirements[9][2][2] = 0;
    SCActionRequirements[9][3][0] = 16;
    SCActionRequirements[9][3][1] = 1;
    SCActionRequirements[9][3][2] = 0;
    SCActionRequirements[9][4][0] = 69;
    SCActionRequirements[9][4][1] = 2;
    SCActionRequirements[9][4][2] = 0;
    
    SCActionCosts[9][0][0] = 3;
    SCActionCosts[9][1][0] = 16;
    SCActionCosts[9][1][1] = 1;
    SCActionCosts[9][2][0] = 0;
    SCActionCosts[9][2][1] = 150;
    SCActionCosts[9][3][0] = 1;
    SCActionCosts[9][3][1] = 200;
    
    SCActionGains[9][0][0] = 1;
    SCActionGains[9][1][0] = 9;
    SCActionGains[9][1][1] = 1;
    
    SCActionTime[9] = 50;
    SCActionString[9] = "Build a nydus network";
    
    // 10: build infestation pit
    SCActionRequirements[10][0][0] = 4;
    SCActionRequirements[10][1][0] = 0;
    SCActionRequirements[10][1][1] = 100;
    SCActionRequirements[10][1][2] = 0;
    SCActionRequirements[10][2][0] = 1;
    SCActionRequirements[10][2][1] = 100;
    SCActionRequirements[10][2][2] = 0;
    SCActionRequirements[10][3][0] = 16;
    SCActionRequirements[10][3][1] = 1;
    SCActionRequirements[10][3][2] = 0;
    SCActionRequirements[10][4][0] = 69;
    SCActionRequirements[10][4][1] = 2;
    SCActionRequirements[10][4][2] = 0;
    
    SCActionCosts[10][0][0] = 3;
    SCActionCosts[10][1][0] = 16;
    SCActionCosts[10][1][1] = 1;
    SCActionCosts[10][2][0] = 0;
    SCActionCosts[10][2][1] = 100;
    SCActionCosts[10][3][0] = 1;
    SCActionCosts[10][3][1] = 100;
    
    SCActionGains[10][0][0] = 1;
    SCActionGains[10][1][0] = 10;
    SCActionGains[10][1][1] = 1;
    
    SCActionTime[10] = 50;
    SCActionString[10] = "Build an infestation pit";
    
    // 11: build spire
    SCActionRequirements[11][0][0] = 4;
    SCActionRequirements[11][1][0] = 0;
    SCActionRequirements[11][1][1] = 200;
    SCActionRequirements[11][1][2] = 0;
    SCActionRequirements[11][2][0] = 1;
    SCActionRequirements[11][2][1] = 200;
    SCActionRequirements[11][2][2] = 0;
    SCActionRequirements[11][3][0] = 16;
    SCActionRequirements[11][3][1] = 1;
    SCActionRequirements[11][3][2] = 0;
    SCActionRequirements[11][4][0] = 69;
    SCActionRequirements[11][4][1] = 2;
    SCActionRequirements[11][4][2] = 0;
    
    SCActionCosts[11][0][0] = 3;
    SCActionCosts[11][1][0] = 16;
    SCActionCosts[11][1][1] = 1;
    SCActionCosts[11][2][0] = 0;
    SCActionCosts[11][2][1] = 200;
    SCActionCosts[11][3][0] = 1;
    SCActionCosts[11][3][1] = 200;
    
    SCActionGains[11][0][0] = 1;
    SCActionGains[11][1][0] = 11;
    SCActionGains[11][1][1] = 1;
    
    SCActionTime[11] = 100;
    SCActionString[11] = "Build a spire";
    
    // 12: build ultralisk cavern
    SCActionRequirements[12][0][0] = 4;
    SCActionRequirements[12][1][0] = 0;
    SCActionRequirements[12][1][1] = 150;
    SCActionRequirements[12][1][2] = 0;
    SCActionRequirements[12][2][0] = 1;
    SCActionRequirements[12][2][1] = 200;
    SCActionRequirements[12][2][2] = 0;
    SCActionRequirements[12][3][0] = 16;
    SCActionRequirements[12][3][1] = 1;
    SCActionRequirements[12][3][2] = 0;
    SCActionRequirements[12][4][0] = 70;
    SCActionRequirements[12][4][1] = 2;
    SCActionRequirements[12][4][2] = 0;
    
    SCActionCosts[12][0][0] = 3;
    SCActionCosts[12][1][0] = 16;
    SCActionCosts[12][1][1] = 1;
    SCActionCosts[12][2][0] = 0;
    SCActionCosts[12][2][1] = 150;
    SCActionCosts[12][3][0] = 1;
    SCActionCosts[12][3][1] = 200;
    
    SCActionGains[12][0][0] = 1;
    SCActionGains[12][1][0] = 12;
    SCActionGains[12][1][1] = 1;
    
    SCActionTime[12] = 65;
    SCActionString[12] = "Build an ultralisk cavern";
    
    // 13: build spine crawler
    SCActionRequirements[13][0][0] = 3;
    SCActionRequirements[13][1][0] = 0;
    SCActionRequirements[13][1][1] = 100;
    SCActionRequirements[13][1][2] = 0;
    SCActionRequirements[13][2][0] = 16;
    SCActionRequirements[13][2][1] = 1;
    SCActionRequirements[13][2][2] = 0;
    SCActionRequirements[13][3][0] = 5;
    SCActionRequirements[13][3][1] = 1;
    SCActionRequirements[13][3][2] = 0;
    
    SCActionCosts[13][0][0] = 2;
    SCActionCosts[13][1][0] = 16;
    SCActionCosts[13][1][1] = 1;
    SCActionCosts[13][2][0] = 0;
    SCActionCosts[13][2][1] = 100;
    
    SCActionGains[13][0][0] = 1;
    SCActionGains[13][1][0] = 13;
    SCActionGains[13][1][1] = 1;
    
    SCActionTime[13] = 50;
    SCActionString[13] = "Build a spine crawler";
    
    // 14: build spore crawler
    SCActionRequirements[14][0][0] = 3;
    SCActionRequirements[14][1][0] = 0;
    SCActionRequirements[14][1][1] = 75;
    SCActionRequirements[14][1][2] = 0;
    SCActionRequirements[14][2][0] = 16;
    SCActionRequirements[14][2][1] = 1;
    SCActionRequirements[14][2][2] = 0;
    SCActionRequirements[14][3][0] = 4;
    SCActionRequirements[14][3][1] = 1;
    SCActionRequirements[14][3][2] = 0;
    
    SCActionCosts[14][0][0] = 2;
    SCActionCosts[14][1][0] = 16;
    SCActionCosts[14][1][1] = 1;
    SCActionCosts[14][2][0] = 0;
    SCActionCosts[14][2][1] = 75;
    
    SCActionGains[14][0][0] = 1;
    SCActionGains[14][1][0] = 14;
    SCActionGains[14][1][1] = 1;
    
    SCActionTime[14] = 30;
    SCActionString[14] = "Build a spore crawler";
    
    // 15: extra larva
    SCActionRequirements[15][0][0] = 2;
    SCActionRequirements[15][1][0] = 31;
    SCActionRequirements[15][1][1] = 1;
    SCActionRequirements[15][1][2] = 2;
    SCActionRequirements[15][2][0] = 21;
    SCActionRequirements[15][2][1] = 1;
    SCActionRequirements[15][2][2] = 0;
    
    SCActionCosts[15][0][0] = 1;
    SCActionCosts[15][1][0] = 31;
    SCActionCosts[15][1][1] = -1;
    
    SCActionGains[15][0][0] = 2;
    SCActionGains[15][1][0] = 31;
    SCActionGains[15][1][1] = -1;
    SCActionGains[15][2][0] = 15;
    SCActionGains[15][2][1] = 4;
    
    SCActionTime[15] = 40;
    SCActionString[15] = "Larva injection";
    
    // 16: spawn drone
    SCActionRequirements[16][0][0] = 3;
    SCActionRequirements[16][1][0] = 0;
    SCActionRequirements[16][1][1] = 50;
    SCActionRequirements[16][1][2] = 0;
    SCActionRequirements[16][2][0] = 15;
    SCActionRequirements[16][2][1] = 1;
    SCActionRequirements[16][2][2] = 0;
    SCActionRequirements[16][3][0] = 73;
    SCActionRequirements[16][3][1] = 1;
    SCActionRequirements[16][3][2] = 0;

    SCActionCosts[16][0][0] = 3;
    SCActionCosts[16][1][0] = 15;
    SCActionCosts[16][1][1] = 1;
    SCActionCosts[16][2][0] = 0;
    SCActionCosts[16][2][1] = 50;
    SCActionCosts[16][3][0] = 72;
    SCActionCosts[16][3][1] = -1;

    SCActionGains[16][0][0] = 2;
    SCActionGains[16][1][0] = 16;
    SCActionGains[16][1][1] = 1;
    SCActionGains[16][2][0] = 72;
    SCActionGains[16][2][1] = -1;

    SCActionTime[16] = 17;
    SCActionString[16] = "Spawn a drone";

    // 17: spawn overlord
    SCActionRequirements[17][0][0] = 3;
    SCActionRequirements[17][1][0] = 0;
    SCActionRequirements[17][1][1] = 100;
    SCActionRequirements[17][1][2] = 0;
    SCActionRequirements[17][2][0] = 15;
    SCActionRequirements[17][2][1] = 1;
    SCActionRequirements[17][2][2] = 0;

    SCActionCosts[17][0][0] = 2;
    SCActionCosts[17][1][0] = 15;
    SCActionCosts[17][1][1] = 1;
    SCActionCosts[17][2][0] = 0;
    SCActionCosts[17][2][1] = 100;

    SCActionGains[17][0][0] = 1;
    SCActionGains[17][1][0] = 17;
    SCActionGains[17][1][1] = 1;

    SCActionTime[17] = 25;
    SCActionString[17] = "Spawn an overlord";

    // 18: spawn zergling
    SCActionRequirements[18][0][0] = 4;
    SCActionRequirements[18][1][0] = 0;
    SCActionRequirements[18][1][1] = 50;
    SCActionRequirements[18][1][2] = 0;
    SCActionRequirements[18][2][0] = 15;
    SCActionRequirements[18][2][1] = 1;
    SCActionRequirements[18][2][2] = 0;
    SCActionRequirements[18][3][0] = 73;
    SCActionRequirements[18][3][1] = 1;
    SCActionRequirements[18][3][2] = 0;
    SCActionRequirements[18][4][0] = 5;
    SCActionRequirements[18][4][1] = 1;
    SCActionRequirements[18][4][2] = 0;

    SCActionCosts[18][0][0] = 3;
    SCActionCosts[18][1][0] = 15;
    SCActionCosts[18][1][1] = 1;
    SCActionCosts[18][2][0] = 0;
    SCActionCosts[18][2][1] = 50;
    SCActionCosts[18][3][0] = 72;
    SCActionCosts[18][3][1] = -1;

    SCActionGains[18][0][0] = 2;
    SCActionGains[18][1][0] = 18;
    SCActionGains[18][1][1] = 2;
    SCActionGains[18][2][0] = 72;
    SCActionGains[18][2][1] = -1;

    SCActionTime[18] = 24;
    SCActionString[18] = "Spawn 2 zerglings";

    // 19: spawn baneling
    SCActionRequirements[19][0][0] = 4;
    SCActionRequirements[19][1][0] = 0; //25 minerals
    SCActionRequirements[19][1][1] = 25;
    SCActionRequirements[19][1][2] = 0;
    SCActionRequirements[19][2][0] = 18; //1 zergling
    SCActionRequirements[19][2][1] = 1;
    SCActionRequirements[19][2][2] = 0;
    SCActionRequirements[19][3][0] = 7; //1 baneling nest
    SCActionRequirements[19][3][1] = 1;
    SCActionRequirements[19][3][2] = 0;
    SCActionRequirements[19][4][0] = 1; //25 gas
    SCActionRequirements[19][4][1] = 25;
    SCActionRequirements[19][4][2] = 0;
    
    SCActionCosts[19][0][0] = 3;
    SCActionCosts[19][1][0] = 18;
    SCActionCosts[19][1][1] = 1;
    SCActionCosts[19][2][0] = 0;
    SCActionCosts[19][2][1] = 25;
    SCActionCosts[19][3][0] = 1;
    SCActionCosts[19][3][1] = 25;
    
    SCActionGains[19][0][0] = 1;
    SCActionGains[19][1][0] = 19;
    SCActionGains[19][1][1] = 1;
    
    SCActionTime[19] = 20;
    SCActionString[19] = "Morph a baneling";
    
    // 20: spawn roach
    SCActionRequirements[20][0][0] = 5;
    SCActionRequirements[20][1][0] = 0; //75 minerals
    SCActionRequirements[20][1][1] = 75;
    SCActionRequirements[20][1][2] = 0;
    SCActionRequirements[20][2][0] = 15; //1 larva
    SCActionRequirements[20][2][1] = 1;
    SCActionRequirements[20][2][2] = 0;
    SCActionRequirements[20][3][0] = 73; //2 supply
    SCActionRequirements[20][3][1] = 2;
    SCActionRequirements[20][3][2] = 0;
    SCActionRequirements[20][4][0] = 6; //1 roach warren
    SCActionRequirements[20][4][1] = 1;
    SCActionRequirements[20][4][2] = 0;
    SCActionRequirements[20][5][0] = 1; //25 gas
    SCActionRequirements[20][5][1] = 25;
    SCActionRequirements[20][5][2] = 0;
    
    SCActionCosts[20][0][0] = 4;
    SCActionCosts[20][1][0] = 15;
    SCActionCosts[20][1][1] = 1;
    SCActionCosts[20][2][0] = 0;
    SCActionCosts[20][2][1] = 75;
    SCActionCosts[20][3][0] = 72;
    SCActionCosts[20][3][1] = -2;
    SCActionCosts[20][4][0] = 1;
    SCActionCosts[20][4][1] = 25;
    
    SCActionGains[20][0][0] = 2;
    SCActionGains[20][1][0] = 20;
    SCActionGains[20][1][1] = 1;
    SCActionGains[20][2][0] = 72;
    SCActionGains[20][2][1] = -2;
    
    SCActionTime[20] = 27;
    SCActionString[20] = "Spawn a roach";
    
    // 21: spawn queen
    SCActionRequirements[21][0][0] = 4;
    SCActionRequirements[21][1][0] = 0; //150 minerals
    SCActionRequirements[21][1][1] = 150;
    SCActionRequirements[21][1][2] = 0;
    SCActionRequirements[21][2][0] = 30; //1 hatchery space
    SCActionRequirements[21][2][1] = 1;
    SCActionRequirements[21][2][2] = 2;
    SCActionRequirements[21][3][0] = 73; //2 supply
    SCActionRequirements[21][3][1] = 2;
    SCActionRequirements[21][3][2] = 0;
    SCActionRequirements[21][4][0] = 5; //1 pool
    SCActionRequirements[21][4][1] = 1;
    SCActionRequirements[21][4][2] = 0;
    
    SCActionCosts[21][0][0] = 3;
    SCActionCosts[21][1][0] = 30;
    SCActionCosts[21][1][1] = -1;
    SCActionCosts[21][2][0] = 0;
    SCActionCosts[21][2][1] = 150;
    SCActionCosts[21][3][0] = 72;
    SCActionCosts[21][3][1] = -2;
    
    SCActionGains[21][0][0] = 3;
    SCActionGains[21][1][0] = 30;
    SCActionGains[21][1][1] = -1;
    SCActionGains[21][2][0] = 72;
    SCActionGains[21][2][1] = -2;
    SCActionGains[21][3][0] = 21;
    SCActionGains[21][3][1] = 1;
    
    SCActionTime[21] = 50;
    SCActionString[21] = "Spawn a queen";
    
    // 22: spawn hydralisk
    SCActionRequirements[22][0][0] = 5;
    SCActionRequirements[22][1][0] = 0; //minerals
    SCActionRequirements[22][1][1] = 100;
    SCActionRequirements[22][1][2] = 0;
    SCActionRequirements[22][2][0] = 15; //larva
    SCActionRequirements[22][2][1] = 1;
    SCActionRequirements[22][2][2] = 0;
    SCActionRequirements[22][3][0] = 73; //supply
    SCActionRequirements[22][3][1] = 2;
    SCActionRequirements[22][3][2] = 0;
    SCActionRequirements[22][4][0] = 8; //building
    SCActionRequirements[22][4][1] = 1;
    SCActionRequirements[22][4][2] = 0;
    SCActionRequirements[22][5][0] = 1; //gas
    SCActionRequirements[22][5][1] = 50;
    SCActionRequirements[22][5][2] = 0;
    
    SCActionCosts[22][0][0] = 4;
    SCActionCosts[22][1][0] = 15;
    SCActionCosts[22][1][1] = 1;
    SCActionCosts[22][2][0] = 0;
    SCActionCosts[22][2][1] = 100;
    SCActionCosts[22][3][0] = 72;
    SCActionCosts[22][3][1] = -2;
    SCActionCosts[22][4][0] = 1;
    SCActionCosts[22][4][1] = 50;
    
    SCActionGains[22][0][0] = 2;
    SCActionGains[22][1][0] = 22;
    SCActionGains[22][1][1] = 1;
    SCActionGains[22][2][0] = 72;
    SCActionGains[22][2][1] = -2;
    
    SCActionTime[22] = 33;
    SCActionString[22] = "Spawn a hydralisk";
    
    // 23: spawn infestor
    SCActionRequirements[23][0][0] = 5;
    SCActionRequirements[23][1][0] = 0; //minerals
    SCActionRequirements[23][1][1] = 100;
    SCActionRequirements[23][1][2] = 0;
    SCActionRequirements[23][2][0] = 15; //larva
    SCActionRequirements[23][2][1] = 1;
    SCActionRequirements[23][2][2] = 0;
    SCActionRequirements[23][3][0] = 73; //supply
    SCActionRequirements[23][3][1] = 2;
    SCActionRequirements[23][3][2] = 0;
    SCActionRequirements[23][4][0] = 10; //building
    SCActionRequirements[23][4][1] = 1;
    SCActionRequirements[23][4][2] = 0;
    SCActionRequirements[23][5][0] = 1; //gas
    SCActionRequirements[23][5][1] = 150;
    SCActionRequirements[23][5][2] = 0;
    
    SCActionCosts[23][0][0] = 4;
    SCActionCosts[23][1][0] = 15;
    SCActionCosts[23][1][1] = 1;
    SCActionCosts[23][2][0] = 0;
    SCActionCosts[23][2][1] = 100;
    SCActionCosts[23][3][0] = 72;
    SCActionCosts[23][3][1] = -2;
    SCActionCosts[23][4][0] = 1;
    SCActionCosts[23][4][1] = 150;
    
    SCActionGains[23][0][0] = 2;
    SCActionGains[23][1][0] = 23;
    SCActionGains[23][1][1] = 1;
    SCActionGains[23][2][0] = 72;
    SCActionGains[23][2][1] = -2;
    
    SCActionTime[23] = 50;
    SCActionString[23] = "Spawn an infestor";
    
    // 24: spawn overseer
    SCActionRequirements[24][0][0] = 4;
    SCActionRequirements[24][1][0] = 0; //minerals
    SCActionRequirements[24][1][1] = 50;
    SCActionRequirements[24][1][2] = 0;
    SCActionRequirements[24][2][0] = 17; //overlord
    SCActionRequirements[24][2][1] = 1;
    SCActionRequirements[24][2][2] = 0;
    SCActionRequirements[24][3][0] = 69; //building
    SCActionRequirements[24][3][1] = 1;
    SCActionRequirements[24][3][2] = 0;
    SCActionRequirements[24][4][0] = 1; //gas
    SCActionRequirements[24][4][1] = 100;
    SCActionRequirements[24][4][2] = 0;
    
    SCActionCosts[24][0][0] = 3;
    SCActionCosts[24][1][0] = 17;
    SCActionCosts[24][1][1] = 1;
    SCActionCosts[24][2][0] = 0;
    SCActionCosts[24][2][1] = 50;
    SCActionCosts[24][3][0] = 1;
    SCActionCosts[24][3][1] = 100;
    
    SCActionGains[24][0][0] = 1;
    SCActionGains[24][1][0] = 24;
    SCActionGains[24][1][1] = 1;
    
    SCActionTime[24] = 17;
    SCActionString[24] = "Morph an overseer";
    
    // 25: spawn nydus worm
    SCActionRequirements[25][0][0] = 3;
    SCActionRequirements[25][1][0] = 0; //minerals
    SCActionRequirements[25][1][1] = 100;
    SCActionRequirements[25][1][2] = 0;
    SCActionRequirements[25][2][0] = 9; //building
    SCActionRequirements[25][2][1] = 1;
    SCActionRequirements[25][2][2] = 0;
    SCActionRequirements[25][3][0] = 1; //gas
    SCActionRequirements[25][3][1] = 100;
    SCActionRequirements[25][3][2] = 0;
    
    SCActionCosts[25][0][0] = 2;
    SCActionCosts[25][1][0] = 0;
    SCActionCosts[25][1][1] = 100;
    SCActionCosts[25][2][0] = 1;
    SCActionCosts[25][2][1] = 100;
    
    SCActionGains[25][0][0] = 1;
    SCActionGains[25][1][0] = 25;
    SCActionGains[25][1][1] = 1;
    
    SCActionTime[25] = 20;
    SCActionString[25] = "Spawn an nydus worm";
    
    // 26: spawn mutalisk
    SCActionRequirements[26][0][0] = 5;
    SCActionRequirements[26][1][0] = 0; //minerals
    SCActionRequirements[26][1][1] = 100;
    SCActionRequirements[26][1][2] = 0;
    SCActionRequirements[26][2][0] = 15; //larva
    SCActionRequirements[26][2][1] = 1;
    SCActionRequirements[26][2][2] = 0;
    SCActionRequirements[26][3][0] = 73; //supply
    SCActionRequirements[26][3][1] = 2;
    SCActionRequirements[26][3][2] = 0;
    SCActionRequirements[26][4][0] = 11; //building
    SCActionRequirements[26][4][1] = 1;
    SCActionRequirements[26][4][2] = 0;
    SCActionRequirements[26][5][0] = 1; //gas
    SCActionRequirements[26][5][1] = 100;
    SCActionRequirements[26][5][2] = 0;
    
    SCActionCosts[26][0][0] = 4;
    SCActionCosts[26][1][0] = 15;
    SCActionCosts[26][1][1] = 1;
    SCActionCosts[26][2][0] = 0;
    SCActionCosts[26][2][1] = 100;
    SCActionCosts[26][3][0] = 72;
    SCActionCosts[26][3][1] = -2;
    SCActionCosts[26][4][0] = 1;
    SCActionCosts[26][4][1] = 100;
    
    SCActionGains[26][0][0] = 2;
    SCActionGains[26][1][0] = 26;
    SCActionGains[26][1][1] = 1;
    SCActionGains[26][2][0] = 72;
    SCActionGains[26][2][1] = -2;
    
    SCActionTime[26] = 33;
    SCActionString[26] = "Spawn a mutalisk";
    
    // 27: spawn corruptor
    SCActionRequirements[27][0][0] = 5;
    SCActionRequirements[27][1][0] = 0; //minerals
    SCActionRequirements[27][1][1] = 150;
    SCActionRequirements[27][1][2] = 0;
    SCActionRequirements[27][2][0] = 15; //larva
    SCActionRequirements[27][2][1] = 1;
    SCActionRequirements[27][2][2] = 0;
    SCActionRequirements[27][3][0] = 73; //supply
    SCActionRequirements[27][3][1] = 2;
    SCActionRequirements[27][3][2] = 0;
    SCActionRequirements[27][4][0] = 11; //building
    SCActionRequirements[27][4][1] = 1;
    SCActionRequirements[27][4][2] = 0;
    SCActionRequirements[27][5][0] = 1; //gas
    SCActionRequirements[27][5][1] = 100;
    SCActionRequirements[27][5][2] = 0;
    
    SCActionCosts[27][0][0] = 4;
    SCActionCosts[27][1][0] = 15;
    SCActionCosts[27][1][1] = 1;
    SCActionCosts[27][2][0] = 0;
    SCActionCosts[27][2][1] = 150;
    SCActionCosts[27][3][0] = 72;
    SCActionCosts[27][3][1] = -2;
    SCActionCosts[27][4][0] = 1;
    SCActionCosts[27][4][1] = 100;
    
    SCActionGains[27][0][0] = 2;
    SCActionGains[27][1][0] = 27;
    SCActionGains[27][1][1] = 1;
    SCActionGains[27][2][0] = 72;
    SCActionGains[27][2][1] = -2;
    
    SCActionTime[27] = 40;
    SCActionString[27] = "Spawn a corruptor";
    
    // 28: spawn brood lord
    SCActionRequirements[28][0][0] = 5;
    SCActionRequirements[28][1][0] = 0; //minerals
    SCActionRequirements[28][1][1] = 150;
    SCActionRequirements[28][1][2] = 0;
    SCActionRequirements[28][2][0] = 27; //corruptor
    SCActionRequirements[28][2][1] = 1;
    SCActionRequirements[28][2][2] = 0;
    SCActionRequirements[28][3][0] = 73; //supply
    SCActionRequirements[28][3][1] = 2;
    SCActionRequirements[28][3][2] = 0;
    SCActionRequirements[28][4][0] = 71; //building
    SCActionRequirements[28][4][1] = 1;
    SCActionRequirements[28][4][2] = 0;
    SCActionRequirements[28][5][0] = 1; //gas
    SCActionRequirements[28][5][1] = 150;
    SCActionRequirements[28][5][2] = 0;
    
    SCActionCosts[28][0][0] = 4;
    SCActionCosts[28][1][0] = 27;
    SCActionCosts[28][1][1] = 1;
    SCActionCosts[28][2][0] = 0;
    SCActionCosts[28][2][1] = 150;
    SCActionCosts[28][3][0] = 72;
    SCActionCosts[28][3][1] = -4;
    SCActionCosts[28][4][0] = 1;
    SCActionCosts[28][4][1] = 150;
    
    SCActionGains[28][0][0] = 2;
    SCActionGains[28][1][0] = 28;
    SCActionGains[28][1][1] = 1;
    SCActionGains[28][2][0] = 72;
    SCActionGains[28][2][1] = -4;
    
    SCActionTime[28] = 34;
    SCActionString[28] = "Morph a brood lord";
    
    // 29: spawn ultralisk
    SCActionRequirements[29][0][0] = 5;
    SCActionRequirements[29][1][0] = 0; //minerals
    SCActionRequirements[29][1][1] = 300;
    SCActionRequirements[29][1][2] = 0;
    SCActionRequirements[29][2][0] = 15; //larva
    SCActionRequirements[29][2][1] = 1;
    SCActionRequirements[29][2][2] = 0;
    SCActionRequirements[29][3][0] = 73; //supply
    SCActionRequirements[29][3][1] = 6;
    SCActionRequirements[29][3][2] = 0;
    SCActionRequirements[29][4][0] = 12; //building
    SCActionRequirements[29][4][1] = 1;
    SCActionRequirements[29][4][2] = 0;
    SCActionRequirements[29][5][0] = 1; //gas
    SCActionRequirements[29][5][1] = 200;
    SCActionRequirements[29][5][2] = 0;
    
    SCActionCosts[29][0][0] = 4;
    SCActionCosts[29][1][0] = 15;
    SCActionCosts[29][1][1] = 1;
    SCActionCosts[29][2][0] = 0;
    SCActionCosts[29][2][1] = 300;
    SCActionCosts[29][3][0] = 72;
    SCActionCosts[29][3][1] = -6;
    SCActionCosts[29][4][0] = 1;
    SCActionCosts[29][4][1] = 200;
    
    SCActionGains[29][0][0] = 2;
    SCActionGains[29][1][0] = 29;
    SCActionGains[29][1][1] = 1;
    SCActionGains[29][2][0] = 72;
    SCActionGains[29][2][1] = -6;
    
    SCActionTime[29] = 70;
    SCActionString[29] = "Spawn an ultralisk";
    
    // 30: upgrade burrow
    SCActionRequirements[30][0][0] = 4;
    SCActionRequirements[30][1][0] = 0; //minerals
    SCActionRequirements[30][1][1] = 100;
    SCActionRequirements[30][1][2] = 0;
    SCActionRequirements[30][2][0] = 1; //gas
    SCActionRequirements[30][2][1] = 100;
    SCActionRequirements[30][2][2] = 0;
    SCActionRequirements[30][3][0] = 30; //hatchery space
    SCActionRequirements[30][3][1] = 1;
    SCActionRequirements[30][3][2] = 2;
    SCActionRequirements[30][4][0] = 42; //no burrow yet
    SCActionRequirements[30][4][1] = 1;
    SCActionRequirements[30][4][2] = 1;
    
    SCActionCosts[30][0][0] = 4;
    SCActionCosts[30][1][0] = 0;
    SCActionCosts[30][1][1] = 100;
    SCActionCosts[30][2][0] = 1;
    SCActionCosts[30][2][1] = 100;
    SCActionCosts[30][3][0] = 30;
    SCActionCosts[30][3][1] = -1;
    SCActionCosts[30][4][0] = 42;
    SCActionCosts[30][4][1] = -1;
    
    SCActionGains[30][0][0] = 2;
    SCActionGains[30][1][0] = 42;
    SCActionGains[30][1][1] = 1;
    SCActionGains[30][2][0] = 30;
    SCActionGains[30][2][1] = -1;
    
    SCActionTime[30] = 100;
    SCActionString[30] = "upgrade burrow";
    
    // 31: upgrade overlord speed
    SCActionRequirements[31][0][0] = 5;
    SCActionRequirements[31][1][0] = 0; //minerals
    SCActionRequirements[31][1][1] = 100;
    SCActionRequirements[31][1][2] = 0;
    SCActionRequirements[31][2][0] = 1; //gas
    SCActionRequirements[31][2][1] = 100;
    SCActionRequirements[31][2][2] = 0;
    SCActionRequirements[31][3][0] = 30; //hatchery space
    SCActionRequirements[31][3][1] = 1;
    SCActionRequirements[31][3][2] = 2;
    SCActionRequirements[31][4][0] = 43; //no tech yet
    SCActionRequirements[31][4][1] = 1;
    SCActionRequirements[31][4][2] = 1;
    SCActionRequirements[31][5][0] = 69; //lair tech
    SCActionRequirements[31][5][1] = 2;
    SCActionRequirements[31][5][2] = 0;
    
    SCActionCosts[31][0][0] = 4;
    SCActionCosts[31][1][0] = 0;
    SCActionCosts[31][1][1] = 100;
    SCActionCosts[31][2][0] = 1;
    SCActionCosts[31][2][1] = 100;
    SCActionCosts[31][3][0] = 30;
    SCActionCosts[31][3][1] = -1;
    SCActionCosts[31][4][0] = 43;
    SCActionCosts[31][4][1] = -1;
    
    SCActionGains[31][0][0] = 2;
    SCActionGains[31][1][0] = 43;
    SCActionGains[31][1][1] = 1;
    SCActionGains[31][2][0] = 30;
    SCActionGains[31][2][1] = -1;
    
    SCActionTime[31] = 60;
    SCActionString[31] = "upgrade overlord speed";
    
    // 32: upgrade overlord carriage
    SCActionRequirements[32][0][0] = 5;
    SCActionRequirements[32][1][0] = 0; //minerals
    SCActionRequirements[32][1][1] = 200;
    SCActionRequirements[32][1][2] = 0;
    SCActionRequirements[32][2][0] = 1; //gas
    SCActionRequirements[32][2][1] = 200;
    SCActionRequirements[32][2][2] = 0;
    SCActionRequirements[32][3][0] = 30; //hatchery space
    SCActionRequirements[32][3][1] = 1;
    SCActionRequirements[32][3][2] = 2;
    SCActionRequirements[32][4][0] = 44; //no tech yet
    SCActionRequirements[32][4][1] = 1;
    SCActionRequirements[32][4][2] = 1;
    SCActionRequirements[32][5][0] = 69; //lair tech
    SCActionRequirements[32][5][1] = 2;
    SCActionRequirements[32][5][2] = 0;
    
    SCActionCosts[32][0][0] = 4;
    SCActionCosts[32][1][0] = 0;
    SCActionCosts[32][1][1] = 200;
    SCActionCosts[32][2][0] = 1;
    SCActionCosts[32][2][1] = 200;
    SCActionCosts[32][3][0] = 30;
    SCActionCosts[32][3][1] = -1;
    SCActionCosts[32][4][0] = 44;
    SCActionCosts[32][4][1] = -1;
    
    SCActionGains[32][0][0] = 2;
    SCActionGains[32][1][0] = 44;
    SCActionGains[32][1][1] = 1;
    SCActionGains[32][2][0] = 30;
    SCActionGains[32][2][1] = -1;
    
    SCActionTime[32] = 130;
    SCActionString[32] = "upgrade overlord carriage";
    
    // 33: upgrade melee L1
    SCActionRequirements[33][0][0] = 4;
    SCActionRequirements[33][1][0] = 0; //minerals
    SCActionRequirements[33][1][1] = 100;
    SCActionRequirements[33][1][2] = 0;
    SCActionRequirements[33][2][0] = 1; //gas
    SCActionRequirements[33][2][1] = 100;
    SCActionRequirements[33][2][2] = 0;
    SCActionRequirements[33][3][0] = 33; //ev space
    SCActionRequirements[33][3][1] = 1;
    SCActionRequirements[33][3][2] = 4;
    SCActionRequirements[33][4][0] = 45; //no tech yet
    SCActionRequirements[33][4][1] = 1;
    SCActionRequirements[33][4][2] = 1;
    
    SCActionCosts[33][0][0] = 4;
    SCActionCosts[33][1][0] = 0;
    SCActionCosts[33][1][1] = 100;
    SCActionCosts[33][2][0] = 1;
    SCActionCosts[33][2][1] = 100;
    SCActionCosts[33][3][0] = 33;
    SCActionCosts[33][3][1] = -1;
    SCActionCosts[33][4][0] = 45;
    SCActionCosts[33][4][1] = -1;
    
    SCActionGains[33][0][0] = 2;
    SCActionGains[33][1][0] = 45;
    SCActionGains[33][1][1] = 1;
    SCActionGains[33][2][0] = 33;
    SCActionGains[33][2][1] = -1;
    
    SCActionTime[33] = 160;
    SCActionString[33] = "upgrade melee attack L1";
    
    // 34: upgrade melee L2
    SCActionRequirements[34][0][0] = 6;
    SCActionRequirements[34][1][0] = 0; //minerals
    SCActionRequirements[34][1][1] = 150;
    SCActionRequirements[34][1][2] = 0;
    SCActionRequirements[34][2][0] = 1; //gas
    SCActionRequirements[34][2][1] = 150;
    SCActionRequirements[34][2][2] = 0;
    SCActionRequirements[34][3][0] = 33; //ev space
    SCActionRequirements[34][3][1] = 1;
    SCActionRequirements[34][3][2] = 4;
    SCActionRequirements[34][4][0] = 46; //no tech yet
    SCActionRequirements[34][4][1] = 1;
    SCActionRequirements[34][4][2] = 1;
    SCActionRequirements[34][5][0] = 69; //lair tech
    SCActionRequirements[34][5][1] = 2;
    SCActionRequirements[34][5][2] = 0;
    SCActionRequirements[34][6][0] = 45; //previos tech
    SCActionRequirements[34][6][1] = 2;
    SCActionRequirements[34][6][2] = 0;
    
    SCActionCosts[34][0][0] = 4;
    SCActionCosts[34][1][0] = 0;
    SCActionCosts[34][1][1] = 150;
    SCActionCosts[34][2][0] = 1;
    SCActionCosts[34][2][1] = 150;
    SCActionCosts[34][3][0] = 33;
    SCActionCosts[34][3][1] = -1;
    SCActionCosts[34][4][0] = 46;
    SCActionCosts[34][4][1] = -1;
    
    SCActionGains[34][0][0] = 2;
    SCActionGains[34][1][0] = 46;
    SCActionGains[34][1][1] = 1;
    SCActionGains[34][2][0] = 33;
    SCActionGains[34][2][1] = -1;
    
    SCActionTime[34] = 190;
    SCActionString[34] = "upgrade melee attack L2";
    
    // 35: upgrade melee L3
    SCActionRequirements[35][0][0] = 6;
    SCActionRequirements[35][1][0] = 0; //minerals
    SCActionRequirements[35][1][1] = 200;
    SCActionRequirements[35][1][2] = 0;
    SCActionRequirements[35][2][0] = 1; //gas
    SCActionRequirements[35][2][1] = 200;
    SCActionRequirements[35][2][2] = 0;
    SCActionRequirements[35][3][0] = 33; //ev space
    SCActionRequirements[35][3][1] = 1;
    SCActionRequirements[35][3][2] = 4;
    SCActionRequirements[35][4][0] = 47; //no tech yet
    SCActionRequirements[35][4][1] = 1;
    SCActionRequirements[35][4][2] = 1;
    SCActionRequirements[35][5][0] = 70; //hive tech
    SCActionRequirements[35][5][1] = 2;
    SCActionRequirements[35][5][2] = 0;
    SCActionRequirements[35][6][0] = 46; //previos tech
    SCActionRequirements[35][6][1] = 2;
    SCActionRequirements[35][6][2] = 0;
    
    SCActionCosts[35][0][0] = 4;
    SCActionCosts[35][1][0] = 0;
    SCActionCosts[35][1][1] = 200;
    SCActionCosts[35][2][0] = 1;
    SCActionCosts[35][2][1] = 200;
    SCActionCosts[35][3][0] = 33;
    SCActionCosts[35][3][1] = -1;
    SCActionCosts[35][4][0] = 47;
    SCActionCosts[35][4][1] = -1;
    
    SCActionGains[35][0][0] = 2;
    SCActionGains[35][1][0] = 47;
    SCActionGains[35][1][1] = 1;
    SCActionGains[35][2][0] = 33;
    SCActionGains[35][2][1] = -1;
    
    SCActionTime[35] = 220;
    SCActionString[35] = "upgrade melee attack L3";
    
    // 36: upgrade range L1
    SCActionRequirements[36][0][0] = 4;
    SCActionRequirements[36][1][0] = 0; //minerals
    SCActionRequirements[36][1][1] = 100;
    SCActionRequirements[36][1][2] = 0;
    SCActionRequirements[36][2][0] = 1; //gas
    SCActionRequirements[36][2][1] = 100;
    SCActionRequirements[36][2][2] = 0;
    SCActionRequirements[36][3][0] = 33; //ev space
    SCActionRequirements[36][3][1] = 1;
    SCActionRequirements[36][3][2] = 4;
    SCActionRequirements[36][4][0] = 48; //no tech yet
    SCActionRequirements[36][4][1] = 1;
    SCActionRequirements[36][4][2] = 1;
    
    SCActionCosts[36][0][0] = 4;
    SCActionCosts[36][1][0] = 0;
    SCActionCosts[36][1][1] = 100;
    SCActionCosts[36][2][0] = 1;
    SCActionCosts[36][2][1] = 100;
    SCActionCosts[36][3][0] = 33;
    SCActionCosts[36][3][1] = -1;
    SCActionCosts[36][4][0] = 48;
    SCActionCosts[36][4][1] = -1;
    
    SCActionGains[36][0][0] = 2;
    SCActionGains[36][1][0] = 48;
    SCActionGains[36][1][1] = 1;
    SCActionGains[36][2][0] = 33;
    SCActionGains[36][2][1] = -1;
    
    SCActionTime[36] = 160;
    SCActionString[36] = "upgrade ranged attack L1";
    
    // 37: upgrade range L2
    SCActionRequirements[37][0][0] = 6;
    SCActionRequirements[37][1][0] = 0; //minerals
    SCActionRequirements[37][1][1] = 150;
    SCActionRequirements[37][1][2] = 0;
    SCActionRequirements[37][2][0] = 1; //gas
    SCActionRequirements[37][2][1] = 150;
    SCActionRequirements[37][2][2] = 0;
    SCActionRequirements[37][3][0] = 33; //ev space
    SCActionRequirements[37][3][1] = 1;
    SCActionRequirements[37][3][2] = 4;
    SCActionRequirements[37][4][0] = 49; //no tech yet
    SCActionRequirements[37][4][1] = 1;
    SCActionRequirements[37][4][2] = 1;
    SCActionRequirements[37][5][0] = 69; //lair tech
    SCActionRequirements[37][5][1] = 2;
    SCActionRequirements[37][5][2] = 0;
    SCActionRequirements[37][6][0] = 48; //previos tech
    SCActionRequirements[37][6][1] = 2;
    SCActionRequirements[37][6][2] = 0;
    
    SCActionCosts[37][0][0] = 4;
    SCActionCosts[37][1][0] = 0;
    SCActionCosts[37][1][1] = 150;
    SCActionCosts[37][2][0] = 1;
    SCActionCosts[37][2][1] = 150;
    SCActionCosts[37][3][0] = 33;
    SCActionCosts[37][3][1] = -1;
    SCActionCosts[37][4][0] = 49;
    SCActionCosts[37][4][1] = -1;
    
    SCActionGains[37][0][0] = 2;
    SCActionGains[37][1][0] = 49;
    SCActionGains[37][1][1] = 1;
    SCActionGains[37][2][0] = 33;
    SCActionGains[37][2][1] = -1;
    
    SCActionTime[37] = 190;
    SCActionString[37] = "upgrade ranged attack L2";
    
    // 38: upgrade range L3
    SCActionRequirements[38][0][0] = 6;
    SCActionRequirements[38][1][0] = 0; //minerals
    SCActionRequirements[38][1][1] = 200;
    SCActionRequirements[38][1][2] = 0;
    SCActionRequirements[38][2][0] = 1; //gas
    SCActionRequirements[38][2][1] = 200;
    SCActionRequirements[38][2][2] = 0;
    SCActionRequirements[38][3][0] = 33; //ev space
    SCActionRequirements[38][3][1] = 1;
    SCActionRequirements[38][3][2] = 4;
    SCActionRequirements[38][4][0] = 50; //no tech yet
    SCActionRequirements[38][4][1] = 1;
    SCActionRequirements[38][4][2] = 1;
    SCActionRequirements[38][5][0] = 70; //lair tech
    SCActionRequirements[38][5][1] = 2;
    SCActionRequirements[38][5][2] = 0;
    SCActionRequirements[38][6][0] = 49; //previos tech
    SCActionRequirements[38][6][1] = 2;
    SCActionRequirements[38][6][2] = 0;
    
    SCActionCosts[38][0][0] = 4;
    SCActionCosts[38][1][0] = 0;
    SCActionCosts[38][1][1] = 200;
    SCActionCosts[38][2][0] = 1;
    SCActionCosts[38][2][1] = 200;
    SCActionCosts[38][3][0] = 33;
    SCActionCosts[38][3][1] = -1;
    SCActionCosts[38][4][0] = 50;
    SCActionCosts[38][4][1] = -1;
    
    SCActionGains[38][0][0] = 2;
    SCActionGains[38][1][0] = 50;
    SCActionGains[38][1][1] = 1;
    SCActionGains[38][2][0] = 33;
    SCActionGains[38][2][1] = -1;
    
    SCActionTime[38] = 220;
    SCActionString[38] = "upgrade ranged attack L3";
    
    // 39: upgrade defence L1
    SCActionRequirements[39][0][0] = 4;
    SCActionRequirements[39][1][0] = 0; //minerals
    SCActionRequirements[39][1][1] = 150;
    SCActionRequirements[39][1][2] = 0;
    SCActionRequirements[39][2][0] = 1; //gas
    SCActionRequirements[39][2][1] = 150;
    SCActionRequirements[39][2][2] = 0;
    SCActionRequirements[39][3][0] = 33; //ev space
    SCActionRequirements[39][3][1] = 1;
    SCActionRequirements[39][3][2] = 4;
    SCActionRequirements[39][4][0] = 51; //no tech yet
    SCActionRequirements[39][4][1] = 1;
    SCActionRequirements[39][4][2] = 1;
    
    SCActionCosts[39][0][0] = 4;
    SCActionCosts[39][1][0] = 0;
    SCActionCosts[39][1][1] = 150;
    SCActionCosts[39][2][0] = 1;
    SCActionCosts[39][2][1] = 150;
    SCActionCosts[39][3][0] = 33;
    SCActionCosts[39][3][1] = -1;
    SCActionCosts[39][4][0] = 51;
    SCActionCosts[39][4][1] = -1;
    
    SCActionGains[39][0][0] = 2;
    SCActionGains[39][1][0] = 51;
    SCActionGains[39][1][1] = 1;
    SCActionGains[39][2][0] = 33;
    SCActionGains[39][2][1] = -1;
    
    SCActionTime[39] = 160;
    SCActionString[39] = "upgrade defence L1";
    
    // 40: upgrade defence L2
    SCActionRequirements[40][0][0] = 6;
    SCActionRequirements[40][1][0] = 0; //minerals
    SCActionRequirements[40][1][1] = 225;
    SCActionRequirements[40][1][2] = 0;
    SCActionRequirements[40][2][0] = 1; //gas
    SCActionRequirements[40][2][1] = 225;
    SCActionRequirements[40][2][2] = 0;
    SCActionRequirements[40][3][0] = 33; //ev space
    SCActionRequirements[40][3][1] = 1;
    SCActionRequirements[40][3][2] = 4;
    SCActionRequirements[40][4][0] = 52; //no tech yet
    SCActionRequirements[40][4][1] = 1;
    SCActionRequirements[40][4][2] = 1;
    SCActionRequirements[40][5][0] = 69; //lair tech
    SCActionRequirements[40][5][1] = 2;
    SCActionRequirements[40][5][2] = 0;
    SCActionRequirements[40][6][0] = 51; //previos tech
    SCActionRequirements[40][6][1] = 2;
    SCActionRequirements[40][6][2] = 0;
    
    SCActionCosts[40][0][0] = 4;
    SCActionCosts[40][1][0] = 0;
    SCActionCosts[40][1][1] = 225;
    SCActionCosts[40][2][0] = 1;
    SCActionCosts[40][2][1] = 225;
    SCActionCosts[40][3][0] = 33;
    SCActionCosts[40][3][1] = -1;
    SCActionCosts[40][4][0] = 52;
    SCActionCosts[40][4][1] = -1;
    
    SCActionGains[40][0][0] = 2;
    SCActionGains[40][1][0] = 52;
    SCActionGains[40][1][1] = 1;
    SCActionGains[40][2][0] = 33;
    SCActionGains[40][2][1] = -1;
    
    SCActionTime[40] = 190;
    SCActionString[40] = "upgrade defence L2";
    
    // 41: upgrade defence L3
    SCActionRequirements[41][0][0] = 6;
    SCActionRequirements[41][1][0] = 0; //minerals
    SCActionRequirements[41][1][1] = 300;
    SCActionRequirements[41][1][2] = 0;
    SCActionRequirements[41][2][0] = 1; //gas
    SCActionRequirements[41][2][1] = 300;
    SCActionRequirements[41][2][2] = 0;
    SCActionRequirements[41][3][0] = 33; //ev space
    SCActionRequirements[41][3][1] = 1;
    SCActionRequirements[41][3][2] = 4;
    SCActionRequirements[41][4][0] = 53; //no tech yet
    SCActionRequirements[41][4][1] = 1;
    SCActionRequirements[41][4][2] = 1;
    SCActionRequirements[41][5][0] = 70; //lair tech
    SCActionRequirements[41][5][1] = 2;
    SCActionRequirements[41][5][2] = 0;
    SCActionRequirements[41][6][0] = 52; //previos tech
    SCActionRequirements[41][6][1] = 2;
    SCActionRequirements[41][6][2] = 0;
    
    SCActionCosts[41][0][0] = 4;
    SCActionCosts[41][1][0] = 0;
    SCActionCosts[41][1][1] = 300;
    SCActionCosts[41][2][0] = 1;
    SCActionCosts[41][2][1] = 300;
    SCActionCosts[41][3][0] = 33;
    SCActionCosts[41][3][1] = -1;
    SCActionCosts[41][4][0] = 53;
    SCActionCosts[41][4][1] = -1;
    
    SCActionGains[41][0][0] = 2;
    SCActionGains[41][1][0] = 53;
    SCActionGains[41][1][1] = 1;
    SCActionGains[41][2][0] = 33;
    SCActionGains[41][2][1] = -1;
    
    SCActionTime[41] = 220;
    SCActionString[41] = "upgrade defence L3";
    
    // 42: upgrade zergling speed
    SCActionRequirements[42][0][0] = 4;
    SCActionRequirements[42][1][0] = 0; //minerals
    SCActionRequirements[42][1][1] = 100;
    SCActionRequirements[42][1][2] = 0;
    SCActionRequirements[42][2][0] = 1; //gas
    SCActionRequirements[42][2][1] = 100;
    SCActionRequirements[42][2][2] = 0;
    SCActionRequirements[42][3][0] = 34; //space
    SCActionRequirements[42][3][1] = 1;
    SCActionRequirements[42][3][2] = 5;
    SCActionRequirements[42][4][0] = 54; //no tech yet
    SCActionRequirements[42][4][1] = 1;
    SCActionRequirements[42][4][2] = 1;
    
    SCActionCosts[42][0][0] = 4;
    SCActionCosts[42][1][0] = 0;
    SCActionCosts[42][1][1] = 100;
    SCActionCosts[42][2][0] = 1;
    SCActionCosts[42][2][1] = 100;
    SCActionCosts[42][3][0] = 34;
    SCActionCosts[42][3][1] = -1;
    SCActionCosts[42][4][0] = 54;
    SCActionCosts[42][4][1] = -1;
    
    SCActionGains[42][0][0] = 2;
    SCActionGains[42][1][0] = 54;
    SCActionGains[42][1][1] = 1;
    SCActionGains[42][2][0] = 34;
    SCActionGains[42][2][1] = -1;
    
    SCActionTime[42] = 110;
    SCActionString[42] = "upgrade zergling speed";
    
    // 43: upgrade zergling attack speed
    SCActionRequirements[43][0][0] = 5;
    SCActionRequirements[43][1][0] = 0; //minerals
    SCActionRequirements[43][1][1] = 200;
    SCActionRequirements[43][1][2] = 0;
    SCActionRequirements[43][2][0] = 1; //gas
    SCActionRequirements[43][2][1] = 200;
    SCActionRequirements[43][2][2] = 0;
    SCActionRequirements[43][3][0] = 34; //space
    SCActionRequirements[43][3][1] = 1;
    SCActionRequirements[43][3][2] = 5;
    SCActionRequirements[43][4][0] = 55; //no tech yet
    SCActionRequirements[43][4][1] = 1;
    SCActionRequirements[43][4][2] = 1;
    SCActionRequirements[43][5][0] = 70; //hive tech
    SCActionRequirements[43][5][1] = 2;
    SCActionRequirements[43][5][2] = 0;
    
    SCActionCosts[43][0][0] = 4;
    SCActionCosts[43][1][0] = 0;
    SCActionCosts[43][1][1] = 200;
    SCActionCosts[43][2][0] = 1;
    SCActionCosts[43][2][1] = 200;
    SCActionCosts[43][3][0] = 34;
    SCActionCosts[43][3][1] = -1;
    SCActionCosts[43][4][0] = 55;
    SCActionCosts[43][4][1] = -1;
    
    SCActionGains[43][0][0] = 2;
    SCActionGains[43][1][0] = 55;
    SCActionGains[43][1][1] = 1;
    SCActionGains[43][2][0] = 34;
    SCActionGains[43][2][1] = -1;
    
    SCActionTime[43] = 130;
    SCActionString[43] = "upgrade zergling attack speed";
    
    // 44: upgrade roach speed
    SCActionRequirements[44][0][0] = 5;
    SCActionRequirements[44][1][0] = 0; //minerals
    SCActionRequirements[44][1][1] = 100;
    SCActionRequirements[44][1][2] = 0;
    SCActionRequirements[44][2][0] = 1; //gas
    SCActionRequirements[44][2][1] = 100;
    SCActionRequirements[44][2][2] = 0;
    SCActionRequirements[44][3][0] = 35; //space
    SCActionRequirements[44][3][1] = 1;
    SCActionRequirements[44][3][2] = 6;
    SCActionRequirements[44][4][0] = 56; //no tech yet
    SCActionRequirements[44][4][1] = 1;
    SCActionRequirements[44][4][2] = 1;
    SCActionRequirements[44][5][0] = 69; //lair tech
    SCActionRequirements[44][5][1] = 2;
    SCActionRequirements[44][5][2] = 0;
    
    SCActionCosts[44][0][0] = 4;
    SCActionCosts[44][1][0] = 0;
    SCActionCosts[44][1][1] = 100;
    SCActionCosts[44][2][0] = 1;
    SCActionCosts[44][2][1] = 100;
    SCActionCosts[44][3][0] = 35;
    SCActionCosts[44][3][1] = -1;
    SCActionCosts[44][4][0] = 56;
    SCActionCosts[44][4][1] = -1;
    
    SCActionGains[44][0][0] = 2;
    SCActionGains[44][1][0] = 56;
    SCActionGains[44][1][1] = 1;
    SCActionGains[44][2][0] = 35;
    SCActionGains[44][2][1] = -1;
    
    SCActionTime[44] = 110;
    SCActionString[44] = "upgrade roach speed";
    
    // 45: upgrade roach burrow movement
    SCActionRequirements[43][0][0] = 5;
    SCActionRequirements[43][1][0] = 0; //minerals
    SCActionRequirements[43][1][1] = 150;
    SCActionRequirements[43][1][2] = 0;
    SCActionRequirements[43][2][0] = 1; //gas
    SCActionRequirements[43][2][1] = 150;
    SCActionRequirements[43][2][2] = 0;
    SCActionRequirements[43][3][0] = 35; //space
    SCActionRequirements[43][3][1] = 1;
    SCActionRequirements[43][3][2] = 6;
    SCActionRequirements[43][4][0] = 57; //no tech yet
    SCActionRequirements[43][4][1] = 1;
    SCActionRequirements[43][4][2] = 1;
    SCActionRequirements[43][5][0] = 69; //lair tech
    SCActionRequirements[43][5][1] = 2;
    SCActionRequirements[43][5][2] = 0;
    
    SCActionCosts[43][0][0] = 4;
    SCActionCosts[43][1][0] = 0;
    SCActionCosts[43][1][1] = 150;
    SCActionCosts[43][2][0] = 1;
    SCActionCosts[43][2][1] = 150;
    SCActionCosts[43][3][0] = 35;
    SCActionCosts[43][3][1] = -1;
    SCActionCosts[43][4][0] = 57;
    SCActionCosts[43][4][1] = -1;
    
    SCActionGains[43][0][0] = 2;
    SCActionGains[43][1][0] = 57;
    SCActionGains[43][1][1] = 1;
    SCActionGains[43][2][0] = 35;
    SCActionGains[43][2][1] = -1;
    
    SCActionTime[43] = 110;
    SCActionString[43] = "upgrade zergling attack speed";
    
    // 46: upgrade baneling speed
    SCActionRequirements[46][0][0] = 5;
    SCActionRequirements[46][1][0] = 0; //minerals
    SCActionRequirements[46][1][1] = 150;
    SCActionRequirements[46][1][2] = 0;
    SCActionRequirements[46][2][0] = 1; //gas
    SCActionRequirements[46][2][1] = 150;
    SCActionRequirements[46][2][2] = 0;
    SCActionRequirements[46][3][0] = 36; //space
    SCActionRequirements[46][3][1] = 1;
    SCActionRequirements[46][3][2] = 5;
    SCActionRequirements[46][4][0] = 58; //no tech yet
    SCActionRequirements[46][4][1] = 1;
    SCActionRequirements[46][4][2] = 1;
    SCActionRequirements[46][5][0] = 69; //lair tech
    SCActionRequirements[46][5][1] = 2;
    SCActionRequirements[46][5][2] = 0;
    
    SCActionCosts[46][0][0] = 4;
    SCActionCosts[46][1][0] = 0;
    SCActionCosts[46][1][1] = 150;
    SCActionCosts[46][2][0] = 1;
    SCActionCosts[46][2][1] = 150;
    SCActionCosts[46][3][0] = 36;
    SCActionCosts[46][3][1] = -1;
    SCActionCosts[46][4][0] = 58;
    SCActionCosts[46][4][1] = -1;
    
    SCActionGains[46][0][0] = 2;
    SCActionGains[46][1][0] = 58;
    SCActionGains[46][1][1] = 1;
    SCActionGains[46][2][0] = 36;
    SCActionGains[46][2][1] = -1;
    
    SCActionTime[46] = 110;
    SCActionString[46] = "upgrade baneling speed";
    
    // 47: upgrade hydralisk range
    SCActionRequirements[47][0][0] = 4;
    SCActionRequirements[47][1][0] = 0; //minerals
    SCActionRequirements[47][1][1] = 150;
    SCActionRequirements[47][1][2] = 0;
    SCActionRequirements[47][2][0] = 1; //gas
    SCActionRequirements[47][2][1] = 150;
    SCActionRequirements[47][2][2] = 0;
    SCActionRequirements[47][3][0] = 37; //space
    SCActionRequirements[47][3][1] = 1;
    SCActionRequirements[47][3][2] = 8;
    SCActionRequirements[47][4][0] = 59; //no tech yet
    SCActionRequirements[47][4][1] = 1;
    SCActionRequirements[47][4][2] = 1;
    
    SCActionCosts[47][0][0] = 4;
    SCActionCosts[47][1][0] = 0;
    SCActionCosts[47][1][1] = 150;
    SCActionCosts[47][2][0] = 1;
    SCActionCosts[47][2][1] = 150;
    SCActionCosts[47][3][0] = 37;
    SCActionCosts[47][3][1] = -1;
    SCActionCosts[47][4][0] = 59;
    SCActionCosts[47][4][1] = -1;
    
    SCActionGains[47][0][0] = 2;
    SCActionGains[47][1][0] = 59;
    SCActionGains[47][1][1] = 1;
    SCActionGains[47][2][0] = 37;
    SCActionGains[47][2][1] = -1;
    
    SCActionTime[47] = 80;
    SCActionString[47] = "upgrade hydralisk attack range";
    
    // 48: upgrade air attack L1
    SCActionRequirements[48][0][0] = 4;
    SCActionRequirements[48][1][0] = 0; //minerals
    SCActionRequirements[48][1][1] = 100;
    SCActionRequirements[48][1][2] = 0;
    SCActionRequirements[48][2][0] = 1; //gas
    SCActionRequirements[48][2][1] = 100;
    SCActionRequirements[48][2][2] = 0;
    SCActionRequirements[48][3][0] = 40; //spire space
    SCActionRequirements[48][3][1] = 1;
    SCActionRequirements[48][3][2] = 11;
    SCActionRequirements[48][4][0] = 60; //no tech yet
    SCActionRequirements[48][4][1] = 1;
    SCActionRequirements[48][4][2] = 1;
    
    SCActionCosts[48][0][0] = 4;
    SCActionCosts[48][1][0] = 0;
    SCActionCosts[48][1][1] = 100;
    SCActionCosts[48][2][0] = 1;
    SCActionCosts[48][2][1] = 100;
    SCActionCosts[48][3][0] = 40;
    SCActionCosts[48][3][1] = -1;
    SCActionCosts[48][4][0] = 60;
    SCActionCosts[48][4][1] = -1;
    
    SCActionGains[48][0][0] = 2;
    SCActionGains[48][1][0] = 60;
    SCActionGains[48][1][1] = 1;
    SCActionGains[48][2][0] = 40;
    SCActionGains[48][2][1] = -1;
    
    SCActionTime[48] = 160;
    SCActionString[48] = "upgrade air attack L1";
    
    // 49: upgrade air attack L2
    SCActionRequirements[49][0][0] = 6;
    SCActionRequirements[49][1][0] = 0; //minerals
    SCActionRequirements[49][1][1] = 175;
    SCActionRequirements[49][1][2] = 0;
    SCActionRequirements[49][2][0] = 1; //gas
    SCActionRequirements[49][2][1] = 175;
    SCActionRequirements[49][2][2] = 0;
    SCActionRequirements[49][3][0] = 40; //spire space
    SCActionRequirements[49][3][1] = 1;
    SCActionRequirements[49][3][2] = 11;
    SCActionRequirements[49][4][0] = 61; //no tech yet
    SCActionRequirements[49][4][1] = 1;
    SCActionRequirements[49][4][2] = 1;
    SCActionRequirements[49][5][0] = 69; //lair tech
    SCActionRequirements[49][5][1] = 2;
    SCActionRequirements[49][5][2] = 0;
    SCActionRequirements[49][6][0] = 60; //previos tech
    SCActionRequirements[49][6][1] = 2;
    SCActionRequirements[49][6][2] = 0;
    
    SCActionCosts[49][0][0] = 4;
    SCActionCosts[49][1][0] = 0;
    SCActionCosts[49][1][1] = 175;
    SCActionCosts[49][2][0] = 1;
    SCActionCosts[49][2][1] = 175;
    SCActionCosts[49][3][0] = 40;
    SCActionCosts[49][3][1] = -1;
    SCActionCosts[49][4][0] = 61;
    SCActionCosts[49][4][1] = -1;
    
    SCActionGains[49][0][0] = 2;
    SCActionGains[49][1][0] = 61;
    SCActionGains[49][1][1] = 1;
    SCActionGains[49][2][0] = 40;
    SCActionGains[49][2][1] = -1;
    
    SCActionTime[49] = 190;
    SCActionString[49] = "upgrade air attack L2";
    
    // 50: upgrade air attack L3
    SCActionRequirements[50][0][0] = 6;
    SCActionRequirements[50][1][0] = 0; //minerals
    SCActionRequirements[50][1][1] = 250;
    SCActionRequirements[50][1][2] = 0;
    SCActionRequirements[50][2][0] = 1; //gas
    SCActionRequirements[50][2][1] = 250;
    SCActionRequirements[50][2][2] = 0;
    SCActionRequirements[50][3][0] = 40; //spire space
    SCActionRequirements[50][3][1] = 1;
    SCActionRequirements[50][3][2] = 11;
    SCActionRequirements[50][4][0] = 62; //no tech yet
    SCActionRequirements[50][4][1] = 1;
    SCActionRequirements[50][4][2] = 1;
    SCActionRequirements[50][5][0] = 70; //lair tech
    SCActionRequirements[50][5][1] = 2;
    SCActionRequirements[50][5][2] = 0;
    SCActionRequirements[50][6][0] = 61; //previos tech
    SCActionRequirements[50][6][1] = 2;
    SCActionRequirements[50][6][2] = 0;
    
    SCActionCosts[50][0][0] = 4;
    SCActionCosts[50][1][0] = 0;
    SCActionCosts[50][1][1] = 250;
    SCActionCosts[50][2][0] = 1;
    SCActionCosts[50][2][1] = 250;
    SCActionCosts[50][3][0] = 40;
    SCActionCosts[50][3][1] = -1;
    SCActionCosts[50][4][0] = 62;
    SCActionCosts[50][4][1] = -1;
    
    SCActionGains[50][0][0] = 2;
    SCActionGains[50][1][0] = 62;
    SCActionGains[50][1][1] = 1;
    SCActionGains[50][2][0] = 40;
    SCActionGains[50][2][1] = -1;
    
    SCActionTime[50] = 220;
    SCActionString[50] = "upgrade air attack L3";
    
    // 51: upgrade air defence L1
    SCActionRequirements[51][0][0] = 4;
    SCActionRequirements[51][1][0] = 0; //minerals
    SCActionRequirements[51][1][1] = 150;
    SCActionRequirements[51][1][2] = 0;
    SCActionRequirements[51][2][0] = 1; //gas
    SCActionRequirements[51][2][1] = 150;
    SCActionRequirements[51][2][2] = 0;
    SCActionRequirements[51][3][0] = 40; //spire space
    SCActionRequirements[51][3][1] = 1;
    SCActionRequirements[51][3][2] = 11;
    SCActionRequirements[51][4][0] = 63; //no tech yet
    SCActionRequirements[51][4][1] = 1;
    SCActionRequirements[51][4][2] = 1;
    
    SCActionCosts[51][0][0] = 4;
    SCActionCosts[51][1][0] = 0;
    SCActionCosts[51][1][1] = 150;
    SCActionCosts[51][2][0] = 1;
    SCActionCosts[51][2][1] = 150;
    SCActionCosts[51][3][0] = 40;
    SCActionCosts[51][3][1] = -1;
    SCActionCosts[51][4][0] = 63;
    SCActionCosts[51][4][1] = -1;
    
    SCActionGains[51][0][0] = 2;
    SCActionGains[51][1][0] = 63;
    SCActionGains[51][1][1] = 1;
    SCActionGains[51][2][0] = 40;
    SCActionGains[51][2][1] = -1;
    
    SCActionTime[51] = 160;
    SCActionString[51] = "upgrade air defence L1";
    
    // 52: upgrade air defence L2
    SCActionRequirements[52][0][0] = 6;
    SCActionRequirements[52][1][0] = 0; //minerals
    SCActionRequirements[52][1][1] = 225;
    SCActionRequirements[52][1][2] = 0;
    SCActionRequirements[52][2][0] = 1; //gas
    SCActionRequirements[52][2][1] = 225;
    SCActionRequirements[52][2][2] = 0;
    SCActionRequirements[52][3][0] = 40; //spire space
    SCActionRequirements[52][3][1] = 1;
    SCActionRequirements[52][3][2] = 11;
    SCActionRequirements[52][4][0] = 64; //no tech yet
    SCActionRequirements[52][4][1] = 1;
    SCActionRequirements[52][4][2] = 1;
    SCActionRequirements[52][5][0] = 69; //lair tech
    SCActionRequirements[52][5][1] = 2;
    SCActionRequirements[52][5][2] = 0;
    SCActionRequirements[52][6][0] = 63; //previos tech
    SCActionRequirements[52][6][1] = 2;
    SCActionRequirements[52][6][2] = 0;
    
    SCActionCosts[52][0][0] = 4;
    SCActionCosts[52][1][0] = 0;
    SCActionCosts[52][1][1] = 225;
    SCActionCosts[52][2][0] = 1;
    SCActionCosts[52][2][1] = 225;
    SCActionCosts[52][3][0] = 40;
    SCActionCosts[52][3][1] = -1;
    SCActionCosts[52][4][0] = 64;
    SCActionCosts[52][4][1] = -1;
    
    SCActionGains[52][0][0] = 2;
    SCActionGains[52][1][0] = 64;
    SCActionGains[52][1][1] = 1;
    SCActionGains[52][2][0] = 40;
    SCActionGains[52][2][1] = -1;
    
    SCActionTime[52] = 190;
    SCActionString[52] = "upgrade air defence L2";
    
    // 53: upgrade air defence L3
    SCActionRequirements[53][0][0] = 6;
    SCActionRequirements[53][1][0] = 0; //minerals
    SCActionRequirements[53][1][1] = 300;
    SCActionRequirements[53][1][2] = 0;
    SCActionRequirements[53][2][0] = 1; //gas
    SCActionRequirements[53][2][1] = 300;
    SCActionRequirements[53][2][2] = 0;
    SCActionRequirements[53][3][0] = 40; //spire space
    SCActionRequirements[53][3][1] = 1;
    SCActionRequirements[53][3][2] = 11;
    SCActionRequirements[53][4][0] = 65; //no tech yet
    SCActionRequirements[53][4][1] = 1;
    SCActionRequirements[53][4][2] = 1;
    SCActionRequirements[53][5][0] = 70; //lair tech
    SCActionRequirements[53][5][1] = 2;
    SCActionRequirements[53][5][2] = 0;
    SCActionRequirements[53][6][0] = 64; //previos tech
    SCActionRequirements[53][6][1] = 2;
    SCActionRequirements[53][6][2] = 0;
    
    SCActionCosts[53][0][0] = 4;
    SCActionCosts[53][1][0] = 0;
    SCActionCosts[53][1][1] = 300;
    SCActionCosts[53][2][0] = 1;
    SCActionCosts[53][2][1] = 300;
    SCActionCosts[53][3][0] = 40;
    SCActionCosts[53][3][1] = -1;
    SCActionCosts[53][4][0] = 65;
    SCActionCosts[53][4][1] = -1;
    
    SCActionGains[53][0][0] = 2;
    SCActionGains[53][1][0] = 65;
    SCActionGains[53][1][1] = 1;
    SCActionGains[53][2][0] = 40;
    SCActionGains[53][2][1] = -1;
    
    SCActionTime[53] = 220;
    SCActionString[53] = "upgrade air defence L3";
    
    // 54: upgrade infestor energy
    SCActionRequirements[54][0][0] = 4;
    SCActionRequirements[54][1][0] = 0; //minerals
    SCActionRequirements[54][1][1] = 150;
    SCActionRequirements[54][1][2] = 0;
    SCActionRequirements[54][2][0] = 1; //gas
    SCActionRequirements[54][2][1] = 150;
    SCActionRequirements[54][2][2] = 0;
    SCActionRequirements[54][3][0] = 39; //space
    SCActionRequirements[54][3][1] = 1;
    SCActionRequirements[54][3][2] = 10;
    SCActionRequirements[54][4][0] = 66; //no tech yet
    SCActionRequirements[54][4][1] = 1;
    SCActionRequirements[54][4][2] = 1;
    
    SCActionCosts[54][0][0] = 4;
    SCActionCosts[54][1][0] = 0;
    SCActionCosts[54][1][1] = 150;
    SCActionCosts[54][2][0] = 1;
    SCActionCosts[54][2][1] = 150;
    SCActionCosts[54][3][0] = 39;
    SCActionCosts[54][3][1] = -1;
    SCActionCosts[54][4][0] = 66;
    SCActionCosts[54][4][1] = -1;
    
    SCActionGains[54][0][0] = 2;
    SCActionGains[54][1][0] = 66;
    SCActionGains[54][1][1] = 1;
    SCActionGains[54][2][0] = 39;
    SCActionGains[54][2][1] = -1;
    
    SCActionTime[54] = 80;
    SCActionString[54] = "upgrade infestor energy";
    
    // 55: upgrade infestor mind control
    SCActionRequirements[55][0][0] = 4;
    SCActionRequirements[55][1][0] = 0; //minerals
    SCActionRequirements[55][1][1] = 150;
    SCActionRequirements[55][1][2] = 0;
    SCActionRequirements[55][2][0] = 1; //gas
    SCActionRequirements[55][2][1] = 150;
    SCActionRequirements[55][2][2] = 0;
    SCActionRequirements[55][3][0] = 39; //space
    SCActionRequirements[55][3][1] = 1;
    SCActionRequirements[55][3][2] = 10;
    SCActionRequirements[55][4][0] = 67; //no tech yet
    SCActionRequirements[55][4][1] = 1;
    SCActionRequirements[55][4][2] = 1;
    
    SCActionCosts[55][0][0] = 4;
    SCActionCosts[55][1][0] = 0;
    SCActionCosts[55][1][1] = 150;
    SCActionCosts[55][2][0] = 1;
    SCActionCosts[55][2][1] = 150;
    SCActionCosts[55][3][0] = 39;
    SCActionCosts[55][3][1] = -1;
    SCActionCosts[55][4][0] = 67;
    SCActionCosts[55][4][1] = -1;
    
    SCActionGains[55][0][0] = 2;
    SCActionGains[55][1][0] = 67;
    SCActionGains[55][1][1] = 1;
    SCActionGains[55][2][0] = 39;
    SCActionGains[55][2][1] = -1;
    
    SCActionTime[55] = 110;
    SCActionString[55] = "upgrade infestor mind control";
    
    // 56: upgrade ultralisk defence
    SCActionRequirements[56][0][0] = 4;
    SCActionRequirements[56][1][0] = 0; //minerals
    SCActionRequirements[56][1][1] = 150;
    SCActionRequirements[56][1][2] = 0;
    SCActionRequirements[56][2][0] = 1; //gas
    SCActionRequirements[56][2][1] = 150;
    SCActionRequirements[56][2][2] = 0;
    SCActionRequirements[56][3][0] = 41; //space
    SCActionRequirements[56][3][1] = 1;
    SCActionRequirements[56][3][2] = 12;
    SCActionRequirements[56][4][0] = 68; //no tech yet
    SCActionRequirements[56][4][1] = 1;
    SCActionRequirements[56][4][2] = 1;
    
    SCActionCosts[56][0][0] = 4;
    SCActionCosts[56][1][0] = 0;
    SCActionCosts[56][1][1] = 150;
    SCActionCosts[56][2][0] = 1;
    SCActionCosts[56][2][1] = 150;
    SCActionCosts[56][3][0] = 41;
    SCActionCosts[56][3][1] = -1;
    SCActionCosts[56][4][0] = 68;
    SCActionCosts[56][4][1] = -1;
    
    SCActionGains[56][0][0] = 2;
    SCActionGains[56][1][0] = 68;
    SCActionGains[56][1][1] = 1;
    SCActionGains[56][2][0] = 41;
    SCActionGains[56][2][1] = -1;
    
    SCActionTime[56] = 110;
    SCActionString[56] = "upgrade ultralisk defence";
    
    // 57: upgrade lair tech
    SCActionRequirements[57][0][0] = 5;
    SCActionRequirements[57][1][0] = 0; //minerals
    SCActionRequirements[57][1][1] = 150;
    SCActionRequirements[57][1][2] = 0;
    SCActionRequirements[57][2][0] = 1; //gas
    SCActionRequirements[57][2][1] = 100;
    SCActionRequirements[57][2][2] = 0;
    SCActionRequirements[57][3][0] = 30; //space
    SCActionRequirements[57][3][1] = 1;
    SCActionRequirements[57][3][2] = 2;
    SCActionRequirements[57][4][0] = 69; //no tech yet
    SCActionRequirements[57][4][1] = 1;
    SCActionRequirements[57][4][2] = 1;
    SCActionRequirements[57][5][0] = 5; //spawning pool
    SCActionRequirements[57][5][1] = 1;
    SCActionRequirements[57][5][2] = 0;
    
    SCActionCosts[57][0][0] = 4;
    SCActionCosts[57][1][0] = 0;
    SCActionCosts[57][1][1] = 150;
    SCActionCosts[57][2][0] = 1;
    SCActionCosts[57][2][1] = 100;
    SCActionCosts[57][3][0] = 30;
    SCActionCosts[57][3][1] = -1;
    SCActionCosts[57][4][0] = 69;
    SCActionCosts[57][4][1] = -1;
    
    SCActionGains[57][0][0] = 2;
    SCActionGains[57][1][0] = 69;
    SCActionGains[57][1][1] = 1;
    SCActionGains[57][2][0] = 30;
    SCActionGains[57][2][1] = -1;
    
    SCActionTime[57] = 80;
    SCActionString[57] = "upgrade lair";
    
    // 58: upgrade hive tech
    SCActionRequirements[58][0][0] = 5;
    SCActionRequirements[58][1][0] = 0; //minerals
    SCActionRequirements[58][1][1] = 200;
    SCActionRequirements[58][1][2] = 0;
    SCActionRequirements[58][2][0] = 1; //gas
    SCActionRequirements[58][2][1] = 150;
    SCActionRequirements[58][2][2] = 0;
    SCActionRequirements[58][3][0] = 30; //space
    SCActionRequirements[58][3][1] = 1;
    SCActionRequirements[58][3][2] = 2;
    SCActionRequirements[58][4][0] = 70; //no tech yet
    SCActionRequirements[58][4][1] = 1;
    SCActionRequirements[58][4][2] = 1;
    SCActionRequirements[58][5][0] = 69; //lair tech
    SCActionRequirements[58][5][1] = 2;
    SCActionRequirements[58][5][2] = 0;
    
    SCActionCosts[58][0][0] = 4;
    SCActionCosts[58][1][0] = 0;
    SCActionCosts[58][1][1] = 200;
    SCActionCosts[58][2][0] = 1;
    SCActionCosts[58][2][1] = 150;
    SCActionCosts[58][3][0] = 70;
    SCActionCosts[58][3][1] = -1;
    SCActionCosts[58][4][0] = 30;
    SCActionCosts[58][4][1] = -1;
    
    SCActionGains[58][0][0] = 2;
    SCActionGains[58][1][0] = 70;
    SCActionGains[58][1][1] = 1;
    SCActionGains[58][2][0] = 30;
    SCActionGains[58][2][1] = -1;
    
    SCActionTime[58] = 100;
    SCActionString[58] = "upgrade hive";
    
    // 59: upgrade greater spire tech
    SCActionRequirements[59][0][0] = 5;
    SCActionRequirements[59][1][0] = 0; //minerals
    SCActionRequirements[59][1][1] = 100;
    SCActionRequirements[59][1][2] = 0;
    SCActionRequirements[59][2][0] = 1; //gas
    SCActionRequirements[59][2][1] = 150;
    SCActionRequirements[59][2][2] = 0;
    SCActionRequirements[59][3][0] = 40; //space
    SCActionRequirements[59][3][1] = 1;
    SCActionRequirements[59][3][2] = 11;
    SCActionRequirements[59][4][0] = 71; //no tech yet
    SCActionRequirements[59][4][1] = 1;
    SCActionRequirements[59][4][2] = 1;
    SCActionRequirements[59][5][0] = 70; //lair tech
    SCActionRequirements[59][5][1] = 2;
    SCActionRequirements[59][5][2] = 0;
    
    SCActionCosts[59][0][0] = 4;
    SCActionCosts[59][1][0] = 0;
    SCActionCosts[59][1][1] = 100;
    SCActionCosts[59][2][0] = 1;
    SCActionCosts[59][2][1] = 150;
    SCActionCosts[59][3][0] = 71;
    SCActionCosts[59][3][1] = -1;
    SCActionCosts[59][4][0] = 40;
    SCActionCosts[59][4][1] = -1;
    
    SCActionGains[59][0][0] = 2;
    SCActionGains[59][1][0] = 71;
    SCActionGains[59][1][1] = 1;
    SCActionGains[59][2][0] = 40;
    SCActionGains[59][2][1] = -1;
    
    SCActionTime[59] = 100;
    SCActionString[59] = "upgrade greater spire";
}

void getResources()
{
    static int larva = 0;
    if ( (SCGameTime - larva) % 15 == 0 && SCState[2] * 3 > SCState[15])
        SCState[15] += SCState[2];
    
    int mineralGained = (SCState[16] - SCState[32]) * 7 / 10;
    if (SCGameTime % 10 < (SCState[16] - SCState[32]) * 7 % 10)
        mineralGained += 1;

    int gasGained = SCState[32] * 7 / 10;
    if (SCGameTime % 10 < SCState[32] * 7 % 10)
        gasGained += 1;

    SCState[0] += mineralGained;
    SCState[1] += gasGained;

    if (SCState[2] * 3 == SCState[15])
        larva = SCGameTime;
}

int supply()
{
    int tmp = 0;
    
    tmp += SCState[16];
    tmp += (SCState[18] + SCState[19]) / 2;
    tmp += SCState[20] * 2;
    tmp += SCState[21] * 2;
    tmp += SCState[22] * 2;
    tmp += SCState[23] * 2;
    tmp += SCState[26] * 2;
    tmp += SCState[27] * 2;
    tmp += SCState[28] * 4;
    tmp += SCState[29] * 6;
    tmp += SCState[72];
    
    return tmp;
}

int maxsupply()
{
    int tmp = SCState[17] * 8 + SCState[24] * 8 + SCState[2] * 2;
    return tmp;
}

void showInformation()
{
    printf("supply: %d/%d\n", supply(), maxsupply());
    int i;
    for (i = 0; i < 72; i++)
        if (SCState[i] > 0)
        {
            if (i >= 42)
                printf("%s: %d\n", SCStateString[i], SCState[i]/2);
            else
                printf("%s: %d\n", SCStateString[i], SCState[i]);
        }
}

void oneSecInSC()
{
    ++SCGameTime;
    
    int tmp;
    printf("\n================= Time %d ================\n",SCGameTime);

    printf("%d ongoing actions:\n",numOfActions);
    int i;
    for (i = numOfActions - 1; i >= 0; i--)
    {
        Actions[i][1]--;
        if (Actions[i][1] == 0)
        {
            numOfActions--;
            printf("%s: Done!\n", SCActionString[Actions[i][0]]);
            finishAction(Actions[i][0]);
        }
        else
        {
            printf("%s: %d seconds remained\n", SCActionString[Actions[i][0]], Actions[i][1]);
        }
    }

    getResources();
    showInformation();

    if (SCWaitTime == 0)
    {
        printf("New action: ");
        scanf(" %d",&tmp);
        while (tmp >= 0)
        {
            printf("    ==> %s\n", SCActionString[tmp]);
            if (isValidAction(tmp))
            {
                //printf("Valid action!\n");
                performAction(tmp);

                int insert = numOfActions;
                for (i = 0; i < numOfActions; i++)
                    if (SCActionTime[tmp] > Actions[i][1])
                    {
                        insert = i;
                        break;
                    }

                for (i = numOfActions; i > insert; i--)
                {
                    Actions[i][0] = Actions[i-1][0];
                    Actions[i][1] = Actions[i-1][1];
                }

                Actions[insert][0] = tmp;
                Actions[insert][1] = SCActionTime[tmp];

                numOfActions++;
            }
            else
                printf("Invalid action!\n");
            
            printf("New action: ");
            scanf(" %d",&tmp);
        }
            SCWaitTime = -1 * tmp;
    }
    SCWaitTime--;
}

int oneSecInSCNonStop()
{
    ++SCGameTime;
    
    int action = -1;
    //printf("\n================= Time %d ================\n",SCGameTime);

    //printf("%d ongoing actions:\n",numOfActions);
    int i;
    for (i = numOfActions - 1; i >= 0; i--)
    {
        Actions[i][1]--;
        if (Actions[i][1] == 0)
        {
            numOfActions--;
            //printf("%s: Done!\n", SCActionString[Actions[i][0]]);
            finishAction(Actions[i][0]);
        }
        else
        {
            //printf("%s: %d seconds remained\n", SCActionString[Actions[i][0]], Actions[i][1]);
        }
    }

    getResources();
    //showInformation();
    
    if (SCBuildOrderPointer <= SCBuildOrder[0])
    {
        action = SCBuildOrder[SCBuildOrderPointer];
        //printf("New action: %d\n", action);
    }
    
    while (action >= 0)
    {
        //printf("    ==> %s\n", SCActionString[action]);
        if (isValidAction(action))
        {
            //printf("action accepted: %d!\n", action);
            performAction(action);

            int insert = numOfActions;
            for (i = 0; i < numOfActions; i++)
                if (SCActionTime[action] > Actions[i][1])
                {
                    insert = i;
                    break;
                }

            for (i = numOfActions; i > insert; i--)
            {
                Actions[i][0] = Actions[i-1][0];
                Actions[i][1] = Actions[i-1][1];
            }

            Actions[insert][0] = action;
            Actions[insert][1] = SCActionTime[action];

            numOfActions++;
            SCBuildOrderPointer++;
        }
        else
            break;

        if (SCBuildOrderPointer <= SCBuildOrder[0])
        {
            action = SCBuildOrder[SCBuildOrderPointer];
        }
        else
        {
            action = -1;
            break;
        }
    }
    
    if (!validBO)
        return 0;
    
    if (SCBuildOrderPointer > SCBuildOrder[0] && numOfActions == 0)
        return 0;
    else 
        return 1;
}

int oneSecInSCDemo()
{
    ++SCGameTime;
    
    int action = -1;
    //printf("\n================= Time %d ================\n",SCGameTime);

    //printf("%d ongoing actions:\n",numOfActions);
    int i;
    for (i = numOfActions - 1; i >= 0; i--)
    {
        Actions[i][1]--;
        if (Actions[i][1] == 0)
        {
            numOfActions--;
            //printf("%s: Done!\n", SCActionString[Actions[i][0]]);
            finishAction(Actions[i][0]);
        }
        else
        {
            //printf("%s: %d seconds remained\n", SCActionString[Actions[i][0]], Actions[i][1]);
        }
    }

    getResources();
    //showInformation();
    
    if (SCBuildOrderPointer <= SCBuildOrder[0])
    {
        action = SCBuildOrder[SCBuildOrderPointer];
        //printf("New action: %d\n", action);
    }
    
    while (action >= 0)
    {
        //printf("    ==> %s\n", SCActionString[action]);
        if (isValidAction(action))
        {
            printf("@%02d:%02d %s\n", SCGameTime/60, SCGameTime%60, SCActionString[action]);
            performAction(action);

            int insert = numOfActions;
            for (i = 0; i < numOfActions; i++)
                if (SCActionTime[action] > Actions[i][1])
                {
                    insert = i;
                    break;
                }

            for (i = numOfActions; i > insert; i--)
            {
                Actions[i][0] = Actions[i-1][0];
                Actions[i][1] = Actions[i-1][1];
            }

            Actions[insert][0] = action;
            Actions[insert][1] = SCActionTime[action];

            numOfActions++;
            SCBuildOrderPointer++;
        }
        else
            break;

        if (SCBuildOrderPointer <= SCBuildOrder[0])
        {
            action = SCBuildOrder[SCBuildOrderPointer];
        }
        else
        {
            action = -1;
            break;
        }
    }
    
    if (!validBO)
        return 0;
    
    if (SCBuildOrderPointer > SCBuildOrder[0] && numOfActions == 0)
        return 0;
    else 
        return 1;
}

void runSCSimulation()
{
    initializeGame();
    
    while(1)
    {
        oneSecInSC();
    }
}

int testBuildOrder(int buildOrder[250])
{
    
    SCVerbose = 0;
    initializeGame();
    
    SCBuildOrderPointer = 1;
    validBO = 1;
    
    int i;
    for (i = 0; i <= SCBuildOrder[0]; i++)
        SCBuildOrder[i] = buildOrder[i];

    int SCContinue = 1;
    while(SCContinue)
    {
        SCContinue = oneSecInSCNonStop();
    }
    
    if (!validBO)
    {
        //printf("Invalid build order!\n");
        return INT_MAX;
    }
    
    return SCGameTime;
}

int demoBuildOrder(int buildOrder[250])
{
    printf("\n\n## Demonstration of BuildOrder ##\n");
    SCVerbose = 0;
    initializeGame();
    
    SCBuildOrderPointer = 1;
    validBO = 1;
    
    int i;
    for (i = 0; i <= SCBuildOrder[0]; i++)
        SCBuildOrder[i] = buildOrder[i];

    int SCContinue = 1;
    while(SCContinue)
    {
        SCContinue = oneSecInSCDemo();
    }
    
    if (!validBO)
    {
        //printf("Invalid build order!\n");
        return INT_MAX;
    }
    
    printf("\n## BuildOrder finishes at %02d:%02d ##\nNow you have:\n", SCGameTime/60, SCGameTime%60);
    showInformation();
    return SCGameTime;
}

int isValidAction(int action)
{
    int check;
    int requirementsMet = 1;
    validBO = 1;
    
    for (check = 1; check <= SCActionRequirements[action][0][0]; check++ )
    {
        if (SCActionRequirements[action][check][0] == 73)
        {
            if (SCActionRequirements[action][check][1] > maxsupply() - supply())
            {
                if (SCVerbose)
                    printf("Spawn more overlords!\n");
                requirementsMet = 0;
                if (numOfActions == 0)
                {
                    validBO = 0;
                }
            }
        }
        else if (SCActionRequirements[action][check][0] == 32 && SCActionRequirements[action][check][1] == 1)
        {
            if (SCState[32] == 0)
            {
                if (SCVerbose)
                    printf("No more drones to pull from gas");
                requirementsMet = 0;
                if (numOfActions == 0)
                    validBO = 0;
            }
        }
        else if (SCActionRequirements[action][check][0] >= 30 && SCActionRequirements[action][check][0] <= 41)
        {
            if (SCState[SCActionRequirements[action][check][0]] == SCState[SCActionRequirements[action][check][2]]*SCActionRequirements[action][check][1])
            {
                if (SCVerbose)
                    printf("No enough production space (%s)\n", SCStateString[SCActionRequirements[action][check][0]]);
                requirementsMet = 0;
                if (numOfActions == 0)
                    validBO = 0;
            } 
        }
        else
        {
            if (SCActionRequirements[action][check][2] == 0)
            {
                if (SCState[SCActionRequirements[action][check][0]] < SCActionRequirements[action][check][1])
                {
                    if (SCActionRequirements[action][check][0] == 0)
                    {
                        if (SCVerbose)
                            printf("We require more minerals!\n");
                        if (numOfActions == 0 && SCState[16] - SCState[32] <= 0)
                            validBO = 0;
                    }
                    else if (SCActionRequirements[action][check][0] == 1)
                    {
                        if (SCVerbose)
                            printf("We require more vespene gas!\n");
                        if (numOfActions == 0 && SCState[32] <= 0)
                            validBO = 0;
                    }
                    else
                    {
                        if (SCVerbose)
                            printf("We need %d %s(s)!\n",SCActionRequirements[action][check][1], SCStateString[SCActionRequirements[action][check][0]]);
                        if (numOfActions == 0)
                            validBO = 0;
                    }
                    requirementsMet = 0;
                }
            }
            else
            {
                if (SCState[SCActionRequirements[action][check][0]] >= SCActionRequirements[action][check][1]) {
                    if (SCVerbose)
                        printf("Too many %s(s)!\n", SCStateString[SCActionRequirements[action][check][0]]);
                    requirementsMet = 0;
                    if (numOfActions == 0)
                        validBO = 0;
                }  
            }
        }
    }

    return requirementsMet;
}

void performAction(int action)
{
    int i;
    for (i = 1; i <= SCActionCosts[action][0][0] ; i++)
        SCState[SCActionCosts[action][i][0]] -= SCActionCosts[action][i][1];
}

void finishAction(int action)
{
    int i;
    for (i = 1; i <= SCActionGains[action][0][0] ; i++)
        SCState[SCActionGains[action][i][0]] += SCActionGains[action][i][1];
}

void readRequest(int ActionList[60])
{
    int dependency[72][60];
    int i, j;
    for (i = 0; i < 72 ; i++)
        for (j = 0; j < 60; j++)
            dependency[i][j] = 0;
    
    
    dependency[2][2] = 1;
    
    dependency[3][3] = 1;
    
    dependency[4][4] = 1;
    
    dependency[5][5] = 1;
    
    dependency[6][6] = 1;
    dependency[6][5] = 1;
    
    dependency[7][7] = 1;
    dependency[7][5] = 1;
    
    dependency[8][8] = 1;
    dependency[8][57] = 1;
    
    dependency[9][9] = 1;
    dependency[9][57] = 1;
    
    dependency[10][10] = 1;
    dependency[10][57] = 1;
    
    dependency[11][11] = 1;
    dependency[11][57] = 1;
    
    dependency[12][12] = 1;
    dependency[12][58] = 1;
    
    dependency[13][13] = 1;
    dependency[13][5] = 1;
    
    dependency[14][14] = 1;
    dependency[14][4] = 1;
    
    dependency[16][16] = 1;
    
    dependency[17][17] = 1;
    
    dependency[18][18] = 1;
    dependency[18][5] = 1;
    
    dependency[19][19] = 1;
    dependency[19][5] = 1;
    dependency[19][7] = 1;
    
    dependency[20][20] = 1;
    dependency[20][5] = 1;
    dependency[20][6] = 1;
    
    dependency[21][21] = 1;
    dependency[21][5] = 1;
    
    dependency[22][22] = 1;
    dependency[22][5] = 1;
    dependency[22][57] = 1;
    dependency[22][8] = 1;
    
    dependency[23][23] = 1;
    dependency[23][5] = 1;
    dependency[23][57] = 1;
    dependency[23][10] = 1;
    
    dependency[24][24] = 1;
    dependency[24][5] = 1;
    dependency[24][57] = 1;
    
    dependency[25][25] = 1;
    dependency[25][5] = 1;
    dependency[25][57] = 1;
    
    dependency[26][26] = 1;
    dependency[26][5] = 1;
    dependency[26][57] = 1;
    dependency[26][11] = 1;
    
    dependency[27][27] = 1;
    dependency[27][5] = 1;
    dependency[27][57] = 1;
    dependency[27][11] = 1;
    
    dependency[28][28] = 1;
    dependency[28][5] = 1;
    dependency[28][57] = 1;
    dependency[28][11] = 1;
    dependency[28][10] = 1;
    dependency[28][58] = 1;
    dependency[28][59] = 1;
    dependency[28][27] = 1;
    
    dependency[29][29] = 1;
    dependency[29][5] = 1;
    dependency[29][57] = 1;
    dependency[29][10] = 1;
    dependency[29][58] = 1;
    dependency[29][12] = 1;
    
    dependency[42][30] = 1;
    
    dependency[43][31] = 1;
    dependency[43][5] = 1;
    dependency[43][57] = 1;
    
    dependency[44][32] = 1;
    dependency[44][5] = 1;
    dependency[44][57] = 1;
    
    dependency[45][33] = 1;
    dependency[45][4] = 1;
    
    dependency[46][34] = 1;
    dependency[46][4] = 1;
    dependency[46][33] = 1;
    dependency[46][5] = 1;
    dependency[46][57] = 1;
    
    dependency[47][35] = 1;
    dependency[47][4] = 1;
    dependency[47][33] = 1;
    dependency[47][34] = 1;
    dependency[47][5] = 1;
    dependency[47][57] = 1;
    dependency[47][10] = 1;
    dependency[47][58] = 1;
    
    dependency[48][36] = 1;
    dependency[48][4] = 1;
    
    dependency[49][37] = 1;
    dependency[49][4] = 1;
    dependency[49][36] = 1;
    dependency[49][5] = 1;
    dependency[49][57] = 1;
    
    dependency[50][38] = 1;
    dependency[50][4] = 1;
    dependency[50][36] = 1;
    dependency[50][37] = 1;
    dependency[50][5] = 1;
    dependency[50][57] = 1;
    dependency[50][10] = 1;
    dependency[50][58] = 1;
    
    dependency[51][39] = 1;
    dependency[51][4] = 1;
    
    dependency[52][40] = 1;
    dependency[52][4] = 1;
    dependency[52][39] = 1;
    dependency[52][5] = 1;
    dependency[52][57] = 1;
    
    dependency[53][41] = 1;
    dependency[53][4] = 1;
    dependency[53][39] = 1;
    dependency[53][40] = 1;
    dependency[53][5] = 1;
    dependency[53][57] = 1;
    dependency[53][10] = 1;
    dependency[53][58] = 1;
    
    dependency[54][42] = 1;
    dependency[54][5] = 1;
    
    dependency[55][43] = 1;
    dependency[55][5] = 1;
    dependency[55][57] = 1;
    dependency[55][10] = 1;
    dependency[55][58] = 1;
    
    dependency[56][44] = 1;
    dependency[56][5] = 1;
    dependency[56][57] = 1;
    dependency[56][6] = 1;
    
    dependency[57][45] = 1;
    dependency[57][5] = 1;
    dependency[57][57] = 1;
    dependency[57][6] = 1;
    
    dependency[58][46] = 1;
    dependency[58][5] = 1;
    dependency[58][57] = 1;
    dependency[58][7] = 1;
    
    dependency[59][47] = 1;
    dependency[59][5] = 1;
    dependency[59][57] = 1;
    dependency[59][8] = 1;
    
    dependency[60][48] = 1;
    dependency[60][5] = 1;
    dependency[60][57] = 1;
    dependency[60][11] = 1;
    
    dependency[61][49] = 1;
    dependency[61][11] = 1;
    dependency[61][48] = 1;
    dependency[61][5] = 1;
    dependency[61][57] = 1;
    
    dependency[62][50] = 1;
    dependency[62][11] = 1;
    dependency[62][48] = 1;
    dependency[62][49] = 1;
    dependency[62][5] = 1;
    dependency[62][57] = 1;
    dependency[62][10] = 1;
    dependency[62][58] = 1;
    
    dependency[63][51] = 1;
    dependency[63][5] = 1;
    dependency[63][57] = 1;
    dependency[63][11] = 1;
    
    dependency[64][52] = 1;
    dependency[64][11] = 1;
    dependency[64][51] = 1;
    dependency[64][5] = 1;
    dependency[64][57] = 1;
    
    dependency[65][53] = 1;
    dependency[65][11] = 1;
    dependency[65][51] = 1;
    dependency[65][52] = 1;
    dependency[65][5] = 1;
    dependency[65][57] = 1;
    dependency[65][10] = 1;
    dependency[65][58] = 1;
    
    dependency[66][54] = 1;
    dependency[66][5] = 1;
    dependency[66][57] = 1;
    dependency[66][10] = 1;
    
    dependency[67][55] = 1;
    dependency[67][5] = 1;
    dependency[67][57] = 1;
    dependency[67][10] = 1;
    
    dependency[68][56] = 1;
    dependency[68][5] = 1;
    dependency[68][57] = 1;
    dependency[68][10] = 1;
    dependency[68][58] = 1;
    dependency[68][12] = 1;
    
    dependency[69][57] = 1;
    dependency[69][5] = 1;
    
    dependency[70][58] = 1;
    dependency[70][5] = 1;
    dependency[70][57] = 1;
    dependency[70][10] = 1;
    
    dependency[71][59] = 1;
    dependency[71][58] = 1;
    dependency[71][5] = 1;
    dependency[71][57] = 1;
    dependency[71][10] = 1;
    dependency[71][11] = 1;
    
    for (i = 0; i < 60 ; i++)
        ActionList[i] = 0;
    
    ActionList[0] = -1;
    ActionList[1] = -1;
    ActionList[2] = -1;
    ActionList[3] = -1;
    ActionList[15] = -1;
    ActionList[16] = -1;
    ActionList[17] = -1;
    ActionList[21] = -1;
    
    printf("Verification Time (200 ~ 1000 recommanded): ");
    scanf("%d", &verificationTime);
    
    int numOfRequirements;
    printf("Number of requirements: ");
    scanf("%d", &numOfRequirements);
    
    int requirement, quantity;
    for(i = 0; i < numOfRequirements; i++)
    {
        printf("Requirement %d: ", i+1);
        scanf("%d", &requirement);
        printf("Quantity: ");
        scanf("%d", &quantity);
        printf(" ==> %d %s(s)\n", quantity, SCStateString[requirement]);
        
        for (j = 0; j < 13; j++)
        {
            if (dependency[requirement][j])
                ActionList[j] = -1;
        }
        
        for (j = 13; j < 30; j++)
        {
            if (dependency[requirement][j])
                ActionList[j] += quantity;
        }
        
        for (j = 30; j < 60; j++)
        {
            if (dependency[requirement][j])
                ActionList[j] = 1;
        }
    }
}


void setSCDependencies(){
    
    int i, j;
    for (i = 0; i < 60; i++)
        for (j = 0; j < 60; j++)
            actionDependency[i][j] = 0;
    
    actionDependency[0][3] = 1;
    actionDependency[1][0] = 1;
    actionDependency[6][5] = 1;
    actionDependency[7][5] = 1;
    actionDependency[8][57] = 1;
    actionDependency[9][57] = 1;
    actionDependency[10][57] = 1;
    actionDependency[11][57] = 1;
    actionDependency[12][58] = 1;
    actionDependency[13][5] = 1;
    actionDependency[14][4] = 1;
    actionDependency[15][21] = 1;
    actionDependency[18][5] = 1;
    actionDependency[19][7] = 1;
    actionDependency[19][18] = 1;
    actionDependency[20][6] = 1;
    actionDependency[21][5] = 1;
    actionDependency[22][8] = 1;
    actionDependency[23][10] = 1;
    actionDependency[24][57] = 1;
    actionDependency[25][9] = 1;
    actionDependency[26][11] = 1;
    actionDependency[27][11] = 1;
    actionDependency[28][27] = 1;
    actionDependency[28][59] = 1;
    actionDependency[29][12] = 1;
    actionDependency[31][57] = 1;
    actionDependency[32][57] = 1;
    actionDependency[34][33] = 1;
    actionDependency[34][57] = 1;
    actionDependency[35][34] = 1;
    actionDependency[35][58] = 1;
    actionDependency[37][36] = 1;
    actionDependency[37][57] = 1;
    actionDependency[38][37] = 1;
    actionDependency[38][58] = 1;
    actionDependency[40][39] = 1;
    actionDependency[40][57] = 1;
    actionDependency[41][40] = 1;
    actionDependency[41][58] = 1;
    actionDependency[42][5] = 1;
    actionDependency[43][58] = 1;
    actionDependency[44][6] = 1;
    actionDependency[44][57] = 1;
    actionDependency[45][6] = 1;
    actionDependency[45][57] = 1;
    actionDependency[46][7] = 1;
    actionDependency[46][57] = 1;
    actionDependency[47][8] = 1;
    actionDependency[48][11] = 1;
    actionDependency[49][48] = 1;
    actionDependency[50][49] = 1;
    actionDependency[50][58] = 1;
    actionDependency[51][11] = 1;
    actionDependency[52][51] = 1;
    actionDependency[53][52] = 1;
    actionDependency[53][58] = 1;
    actionDependency[54][10] = 1;
    actionDependency[55][10] = 1;
    actionDependency[56][12] = 1;
    actionDependency[57][5] = 1;
    actionDependency[58][10] = 1;
    actionDependency[59][58] = 1;
    actionDependency[59][11] = 1;
    
    for (i = 30; i < 60; i++)
        actionDependency[i][0] = 1;
    
    actionDependency[7][0] = 1;
    actionDependency[8][0] = 1;
    actionDependency[9][0] = 1;
    actionDependency[10][0] = 1;
    actionDependency[11][0] = 1;
    actionDependency[12][0] = 1;
    
    actionProb[0] = 6;
    actionProb[1] = 1;
    actionProb[2] = 1;
    actionProb[3] = 2;
    actionProb[4] = 2;
    actionProb[5] = 2;
    actionProb[6] = 2;
    actionProb[7] = 2;
    actionProb[8] = 2;
    actionProb[9] = 2;
    actionProb[10] = 2;
    actionProb[11] = 2;
    actionProb[12] = 2;
    
    actionProb[13] = 10;
    actionProb[14] = 10;
    actionProb[15] = 10;
    actionProb[16] = 30;
    actionProb[17] = 10;
    actionProb[18] = 100;
    actionProb[19] = 20;
    actionProb[20] = 100;
    actionProb[21] = 1;
    actionProb[22] = 100;
    actionProb[23] = 100;
    actionProb[24] = 100;
    actionProb[25] = 100;
    actionProb[26] = 100;
    actionProb[27] = 100;
    actionProb[28] = 50;
    actionProb[29] = 100;
    
    for (i = 30; i < 60; i++)
        actionProb[i] = 100;
}




