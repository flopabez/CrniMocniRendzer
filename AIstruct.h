#pragma once
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


typedef struct tankMovesStack
{
    char stackMove;
    struct tankMovesStack *next;
} tankMovesStack;

typedef struct coPair
{
    int x,y;
} coPair;


typedef struct moveQueue
{
    struct moveQueue *next;
    struct coPair d;
} moveQueue;
