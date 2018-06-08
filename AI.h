#pragma once
#include "strukture.h"

#include "AIstruct.h"
#include "strukture.h"

typedef struct Tank Tank;
typedef struct gameState gameState;




tankMoves *newState(tankMoves *P, int x, int y, int len, char dir);
void movePush(tankMovesStack **S, char m);
char movePop(tankMovesStack **S);
char moveLook(tankMovesStack *S);
void enq(moveQueue **F, moveQueue **R, coPair P);
coPair deq(moveQueue **F, moveQueue **R);
int bulletExists(Tank *T);
int isBounded(int x, int y, int h, int w);
int isBaseBrick(char c, int x, int y, int h, int w);
int seeLine(int sx,int sy, char orientation, gameState G, int tx, int ty);
int seeObj(Tank *T, gameState G, char dir, int tx, int ty);
int tileFree(int x,int y,gameState G);
tankMovesStack *genMoveList(Tank *T, gameState G, int PlayerX, int PlayerY, int chasePlayer);
int canPathfind(Tank *T, gameState G, char dif);
int brickInFront(int x, int y, gameState G,char orient);
char chooseMove(Tank *T, gameState G);
char randMove(Tank *T, gameState G);
void tankPushPQ (heap_t *h, int priority, coPair data);
coPair tankPopPQ (heap_t *h);
char chooseMoveDJ(Tank *T, gameState G);
tankMovesStack *genMoveListDJ(Tank *T, gameState G, char priority);
char pickMove(Tank *T, gameState G);
char *randarr_m();
