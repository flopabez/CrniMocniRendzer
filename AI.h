#pragma once
#include "strukture.h"

#define SEE_DIST 30
#define BRICK 1
#define METAL 2
#define WATER 3
#define ICE 5
#define FOREST 4
#define BASE 6
#define EMPTY 0
#define BLOCK_X 48

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define SHOOT 4
#define SIT 5

#include "AIstruct.h"
#include "strukture.h"

typedef struct Tank Tank;
typedef struct gameState gameState;




tankMoves *newState(tankMoves *P, int x, int y, int len, char dir);
void movePush(tankMovesStack **S, char m);
char movePop(tankMovesStack **S);
char moveLook(tankMovesStack *S);
void enq(moveQueue **F, moveQueue **R, tankMoves *P);
tankMoves* deq(moveQueue **F, moveQueue **R);
int bulletExists(Tank *T);
int isBounded(int x, int y, int h, int w);
int isBaseBrick(char c, int x, int y, int h, int w);
int seeLine(int sx,int sy, char orientation, gameState G, int tx, int ty);
int seeObj(Tank *T, gameState G, char dir, int tx, int ty);
int tileFree(int x,int y,gameState G);
tankMovesStack *genMoveList(Tank *T, gameState G);
int canPathfind(Tank *T, gameState G, char dif);
int brickInFront(int x, int y, gameState G,char orient);
char chooseMove(Tank *T, gameState G);
char randMove(Tank *T, gameState G);
void tankPushPQ (heap_t *h, int priority, coPair data);
coPair tankPopPQ (heap_t *h);
char chooseMoveDJ(Tank *T, gameState G);
tankMovesStack *genMoveListDJ(Tank *T, gameState G);
char pickMove(Tank *T, gameState G);
