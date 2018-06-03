#pragma once
#include "strukture.h"
struct tankPath
{
    char dir;
    int x,y,len;
    struct tankPath *parent;
};

struct tankPathNode
{
    struct tankPath *P;
    struct tankPathNode* next;
};

struct tankPath* addPath(struct tankPath *P, char dir, int x, int y, int len);
void enqPathNode(struct tankPathNode **frontNode,struct tankPathNode **rearNode,struct tankPath* P);
struct tankPath* deqPathNode(struct tankPathNode **frontNode,struct tankPathNode **rearNode);
int tileFree(int x,int y,struct gameState G);
char* easyPath(struct Tank *T,struct gameState G, int xt, int yt);
char* mediumPath(struct Tank *T, struct gameState G, int xt, int yt, char *resetVM);

char* choosePath(struct Tank *T, struct gameState G, int difficulty, char newCycle);

