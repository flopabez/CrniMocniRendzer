#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "strukture.h"

typedef struct Tank Tank;
typedef struct listNode listNode;
typedef struct gameState gameState;

typedef struct tankMoves
{
    struct tankMoves *par;
    char dir;
    int x,y,len;
} tankMoves;

typedef struct tankMovesStack
{
    char stackMove;
    struct tankMovesStack *next;
} tankMovesStack;

typedef struct moveQueue
{
    struct moveQueue *next;
    tankMoves *d;
} moveQueue;

typedef struct coPair
{
    int x,y;
} coPair;

typedef struct {
    int priority;
    coPair data;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
    int size;
} heap_t;

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

#endif // AI2_H_INCLUDED
