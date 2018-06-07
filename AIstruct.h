#pragma once

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
