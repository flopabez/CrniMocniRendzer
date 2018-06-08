#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AI.h"

/*tankMoves is an auxiliary struct used to store the path between A and B for BFS (we use a predecessor matrix for Dijkstra instead). When opening new nodes, our BFS queue priority queue enqueues a new tankMoves* state
 whose parent is is the current tankMoves* state under consideration - the one we just dequeued. In essence, we use this approach since we can easily reconstruct all of a single state's ancestors.*/
tankMoves *newState(tankMoves *P, int x, int y, int len, char dir)
{
    tankMoves *R=(tankMoves*)(malloc(sizeof(tankMoves)));
    R->x=x;
    R->y=y;
    R->dir=dir;
    R->len=len;
    R->par=P;
    return R;
}
/*tankMovesStack is the struct our search algorihms actually return - it's essentially the winning tankMoves* path, whose elements are in order removed and placed onto the stack. This is necessary
because the first element of a tankMoves list is actually the last move we need to make.*/
void movePush(tankMovesStack **S, char m)
{
    tankMovesStack *tS=(tankMovesStack*) (malloc(sizeof(tankMovesStack)));
    tS->stackMove=m;
    tS->next=(*S);
    (*S)=tS;
}

char movePop(tankMovesStack **S)
{
    tankMovesStack *tS=*S;
    char rC;
    if (tS==NULL) return 25;
    (*S)=(*S)->next;
    rC=tS->stackMove;
    free(tS);
    return rC;
}

char moveLook(tankMovesStack *S)
{
    if (S==NULL) return 25;
    else return (S->stackMove);
}

//Queue used for BFS
void enq(moveQueue **F, moveQueue **R, coPair P)
{
    moveQueue *N=(moveQueue*)(malloc(sizeof(moveQueue)));
    N->d=P;
    N->next=NULL;
    if (*F==NULL)
        (*F)=N;
    else (*R)->next=N;
    (*R)=N;
}

coPair deq(moveQueue **F, moveQueue **R)
{
    moveQueue *tQueue;
    coPair tCP;
    tCP.x=-1, tCP.y=-1;
    if (*F==NULL) return tCP;
    if (*F==*R) *R=NULL;

    tQueue=*F;
    tCP=tQueue->d;

    (*F)=(*F)->next;
    free(tQueue);
    return tCP;
}

//Matrix that allows easy lookup of how a tank's coordinates change if it moves in any direction. The directions are in order: UP LEFT DOWN RIGHT
int dCoord[4][2]={{-1,0},{0,-1},{1,0},{0,1}};

//Simple function that tells us if a bullet fired by T exists
int bulletExists(Tank *T)
{
    if (T->inAir>0) return 1;
    else return 0;
}
//Checks if we haven't left the map boundaries
int isBounded(int x, int y, int h, int w)
{
    return (y>=0&&y<h&&x>=0&&x<w);
}
//Checks if the given tile is part of the brick wall surrounding the base.
int isBaseBrick(char c, int x, int y, int h, int w)
{
    return (isBounded(x,y,h,w)&&(c==BRICK&&y<h&&y>=h-6&&x>=(w/2-6)&&x<=(w/2+5)));
}

//Checks if it's possible to see point [ty][tx] from point [sy][sx], assuming a limited line of sight distance and ignoring the base's brick wall.
int seeLine(int sx,int sy, char orientation, gameState G, int tx, int ty)
{
    int i;
    int x=sx,y=sy;
    int h=G.height,w=G.width;
    for (i=0;i<SEE_DIST&&isBounded(x,y,h,w);i++)
    {
        char c=(G.terrain)[y][x];
        if (y>=ty&&y<=ty+3&&x>=tx&&x<=tx+3) return 1;
        if (!(c==EMPTY||c==WATER||c==FOREST||c==BASE||c==ICE||isBaseBrick(c,x,y,h,w))) return 0;
        x+=dCoord[orientation][1];
        y+=dCoord[orientation][0];
    }
    return 0;
}
//Determines the two tiles a tank's bullet would initially occupy and then runs a line of sight check for each.
int seeObj(Tank *T, gameState G, char dir, int tx, int ty)
{
    int x1,y1,x2,y2,h,w;
    int xP=T->xPos/MAP_SCALE;
    int yP=T->yPos/MAP_SCALE;
    x1=x2=xP;
    y1=y2=yP;
    h=G.height;
    w=G.width;
    switch(dir)
    {
    case UP:
        x1++; x2+=2;
        y1=y2=(y2-1);
        break;
    case RIGHT:
        x1=x2=(x2+3);
        y1=y1+1; y2=y2+2;
        break;
    case DOWN:
        x1++; x2+=2;
        y1=y2=(y2+3);
        break;
    case LEFT:
        x1=x2=(x2-1);
        y1++; y1+=2;
        break;
    }
    if ((isBounded(x1,y1,h,w)&&isBounded(x2,y2,h,w))==0) return 0;
    return (seeLine(x1,y1,dir,G,tx,ty)||seeLine(y2,x2,dir,G,tx,ty));
}



/*int tileFree(int x,int y,gameState G)
{
    //If the 4x4 square whose top left square is (y,x) consists entirely of passable tiles OR brick tiles surrounding the base, return 1.
    //If the square includes at least one brick tile not surrounding the base and all of its other tiles are either passable or the base's brick walls, return 2.
    //Otherwise return 3;
    int h=G.height;
    int w=G.width;
    int r=1;
    if (isBounded(x,y,h,w)==0) return 3;
    if (isBounded(x+3,y+3,h,w)==0) return 3;
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
        {
            char c=G.terrain[y+i][x+j];
            if (!(c==EMPTY||c==FOREST||c==ICE||isBaseBrick(c,x+i,y+j,h,w)))
            {
                if (c==BRICK) r=2;
                else return 3;
            }
        }
    return r;
}*/

int tileFree(int x, int y, gameState G)
{
    int h=G.height;
    int w=G.width;
    int i,j;
    char baseWall=0;
    for (i=0;i<4;i++)
        for (j=0;j<4;j++)
    {
        if (isBounded(x+j,y+i,h,w)==0) return 0;
        char c=G.terrain[y+i][x+j];
        baseWall=isBaseBrick(c,x+j,y+i,h,w);
        //if (baseWall==1) printf("Yeet\n");
        if (c!=EMPTY&&c!=FOREST&&c!=ICE&&c!=BASE&&baseWall==0) return 0;
    }
    return 1;
}


//BFS
/*tankMovesStack *genMoveList(Tank *T, gameState G)
{
    int h=G.height,w=G.width,x=(T->xPos)/MAP_SCALE,y=(T->yPos)/MAP_SCALE;
    int i,tempx,tempy;
    char **vis,notDone=1;
    moveQueue *frontQ,*rearQ,*tQ;

    vis=(char**)(malloc(h*sizeof(char*)));
    for (i=0;i<h;i++) vis[i]=(char*)(calloc(w,1));

    tankMoves *P=newState(NULL,x,y,0,255),*R=NULL;
    enq(&frontQ,&rearQ,P);
    while (frontQ&&notDone)
    {
        P=deq(&frontQ,&rearQ);
        vis[P->y][P->x]=1;
        if (isBounded(x,y,h,w)&&(G.terrain)[y][x]==BASE) R=P,notDone=0;
        else for (i=0;i<4;i++)
        {
            tempx=x+dCoord[i][1];
            tempy=y+dCoord[i][0];
            if (tileFree(tempx,tempy,G)&&vis[tempy][tempx]==0) enq(&frontQ,&rearQ,newState(P,tempx,tempy,P->len+1,UP+i));
        }
    }

    tankMovesStack *S=NULL;
    while (R&&R->par)
    {
        movePush(&S,P->dir);
        P=P->par;
    }

    for (i=0;i<h;i++) free(vis[i]);
    free(vis);
    while(frontQ)
    {
        tQ=frontQ;
        frontQ=frontQ->next;
        free(tQ);
    }
    return S;
}*/

char getDir(int x1, int y1, int x2, int y2)
{
    int dx=x2-x1;
    int dy=y2-y1;
    if (dx==0&&dy==1) return DOWN;
    if (dx==0&&dy==-1) return UP;
    if (dx==1&&dy==0) return RIGHT;
    if (dx==-1&&dy==0) return LEFT;
    return SIT;
}

char *randarr_m()
{
    //srand(time(NULL));
    char *tArr=(char*)(malloc(4));
    char R[4]={};
    int n=0;
    while (n<4)
    {
        char c=rand()%4;
        if (R[c]==0)
        {
            R[c]=1;
            (tArr[n++])=c;
        }
    }
    return tArr;
}

tankMovesStack* genMoveList(Tank *T,gameState G, int PlayerX, int PlayerY, int chasePlayer)
{
    int h=G.height,w=G.width,x=T->xPos/MAP_SCALE,y=T->yPos/MAP_SCALE;
    int i,j;
    int nx,ny,whileDone=0;
    char **vis,*randM;
    coPair **pred,P,Q,R,O;
    moveQueue *front=NULL,*rear=NULL;
    O.x=-1; O.y=-1;

    vis=(char**)(malloc(h*sizeof(char*)));
    for (i=0;i<h;i++) vis[i]=(char*)(calloc(w,1));

    pred=(coPair**)(malloc(h*sizeof(coPair*)));
    for (i=0;i<h;i++) pred[i]=(coPair*)(malloc(w*sizeof(coPair)));

    for (i=0;i<h;i++)
        for (j=0;j<w;j++)
        {
            pred[i][j].x=-1,pred[i][j].y=-1;
        }
    Q.x=x,Q.y=y;
    enq(&front,&rear,Q);

    while (front&&whileDone==0)
    {
        P=deq(&front,&rear);
        vis[P.y][P.x]=1;
        randM=randarr_m();
        for (j=0;j<4;j++)
        {
            i=randM[j];
            nx=dCoord[i][1]+P.x;
            ny=dCoord[i][0]+P.y;

            if (tileFree(nx,ny,G)&&vis[ny][nx]==0&&pred[ny][nx].x==-1)
            {
                //if (G.terrain[y][x]==BASE) printf("BBBBB%d %d\n",P.y,P.x);
                //printf("%d %d %d %d\n",P.y,P.x,ny,nx);
                //printf("%d %d %d %d %d\n",dCoord[i][0],dCoord[i][1],pred[P.y][P.x].y,pred[P.y][P.x].x);
                pred[ny][nx].x=P.x;
                pred[ny][nx].y=P.y;
                Q.x=nx; Q.y=ny;
                if ((chasePlayer==1&&ny==PlayerY&&nx==PlayerX)||(chasePlayer==0&&G.terrain[ny][nx]==BASE))
                    O=Q,whileDone=1;
                enq(&front,&rear,Q);
            }

        }

    }


    #define VERT 1
    #define HORI 2
    tankMovesStack *S=NULL;
    int difx,dify,mode;
    if (O.x!=-1)
    Q=O;
    R.x=-1,R.y=-1;
    while (R.x!=T->xPos/MAP_SCALE||R.y!=T->yPos/MAP_SCALE)
    {
        R=pred[Q.y][Q.x];
        difx=Q.x-R.x,dify=Q.y-R.y;
        switch (difx)
        {
            case 0: mode=VERT; break;
            default: mode=HORI; break;
        }
        char k=SIT;
        switch(mode)
        {
            case VERT:
                if (dify==1) k=DOWN;
                else if (dify==-1) k=UP;
                break;
            case HORI:
                if (difx==1) k=RIGHT;
                else if (difx==-1) k=LEFT;
                break;
        }
        movePush(&S,k);
        //printf("%d\n",k);
        Q=R;
    }

    for (i=0;i<h;i++) free(vis[i]), free(pred[i]);
    free(vis); free(pred);

    return S;

}

//Determines tank behavior: easy tanks always move randomly, medium tanks start pathfinding towards the base once they are in the bottom half of the map, hard tanks pathfind immediately.
#define EASY 0
#define MEDIUM 1
#define HARD 2
int canPathfind(Tank *T, gameState G, char dif)
{
    switch(dif)
    {
    case EASY:
        return 0;
    case MEDIUM:
        return ((0.1*T->yPos/MAP_SCALE)>(0.1*G.height/2));
    case HARD:
        return 1;
    default:
        return 0;
    }
}

//Determines if the tank's movement is obstructed by a brick wall that it could destroy. Returns 1 if there's no obstruction, 2 if there's a brick wall obstruction and 0 if it's impassable.
int brickInFront(int x,int y, gameState G,char orient)
{
    int h=G.height,w=G.width,i,ret=1;
    char movedir,c[4];

    switch(orient)
    {
    case UP:
        y-=1;
        movedir=RIGHT;
        break;
    case RIGHT:
        x+=3;
        movedir=DOWN;
        break;
    case DOWN:
        y+=3;
        movedir=RIGHT;
        break;
    case LEFT:
        x-=1;
        movedir=DOWN;
        break;
    }

    for (i=0;i<4;i++)
    {
        if (isBounded(x,y,h,w)==0) return 0;
        c[i]=(G.terrain)[y][x];
        if (c[i]==BRICK) ret=2;
        else if (c[i]==METAL) return 0;
        x+=dCoord[movedir][1];
        y+=dCoord[movedir][0];
    }
    if (ret==2)
    {
        if (c[0]==BRICK&&c[1]!=BRICK) return 0;
        if (c[3]==BRICK&&c[2]!=BRICK) return 0;
    }
    return ret;

}

int tileFreeTank(int x, int y, gameState G, Tank *T)
{
    int tx,ty;
    struct listNode* tankList;
    if (tileFree(x,y,G))
    {
        tankList=G.enemyTanks;
        while (tankList&&tankList->data)
        {
            tx=((Tank*)(tankList->data))->xPos/MAP_SCALE;
            ty=((Tank*)(tankList->data))->yPos/MAP_SCALE;
            if (tx>=x&&tx<=x+3&&ty>=y&&ty<=y+3&&((Tank*)(tankList->data))!=T)
            {
                printf("Tank conflict: %d %d | %d %d\n",y,x,ty,tx);
                return 0;
            }
            tankList=tankList->next;
        }


       /* tankList=G.playerTanks;
        while (tankList&&tankList->data)
        {
            tx=((Tank*)(tankList->data))->xPos/MAP_SCALE;
            ty=((Tank*)(tankList->data))->yPos/MAP_SCALE;
            if (tx>=x&&tx<=x+3&&ty>=y&&ty<=y+3&&((Tank*)(tankList->data))!=T) return 0;
            tankList=tankList->next;
        }*/
        return 1;

    }
    return 0;
}

char getMove(Tank *T)
{
    int r=rand()%100;
    int upw=5,leftw=15,downw=65;
    if (T->direction==RIGHT) leftw=5;
    else leftw=25;
    if (r<upw) return UP;
    if (r<upw+leftw) return LEFT;
    if (r<upw+leftw+downw) return DOWN;
    return RIGHT;
}

//Generates a random legal move.
char randMove(Tank *T, gameState G)
{
    char mArr[4],cdir,m;
    int x,y;
    int i,numChoices=0,keepDirection;

    /*cdir=T->direction;
    if (cdir>=0&&cdir<4)
    {
        x=T->xPos/MAP_SCALE+dCoord[cdir][1];
        y=T->yPos/MAP_SCALE+dCoord[cdir][0];
        keepDirection=rand()%100;
        if (keepDirection<=95) if (tileFreeTank(x,y,G,T)) return cdir;
    }*/

    /*for (i=0;i<4;i++)
    {
        x=T->xPos/MAP_SCALE+dCoord[i][1];
        y=T->yPos/MAP_SCALE+dCoord[i][0];
        if (tileFreeTank(x,y,G)==1) mArr[numChoices++]=UP+i;
    }
    if (rand()%3==0)
    {
        if (tileFreeTank(T->xPos/MAP_SCALE+dCoord[DOWN][1],T->yPos/MAP_SCALE+dCoord[i][0])&&rand()%3==0)
    }
   // printf("Broj poteza: %d\n");
    if (numChoices==0) return 25;
    return mArr[rand()%numChoices];*/
    do
    {
        m=getMove(T);
    } while (tileFreeTank(T->xPos/MAP_SCALE+dCoord[m][1],T->yPos/MAP_SCALE+dCoord[m][0],G,T)==0);
    return m;
}

#define EASY_SHOOTCHANCE 20
#define MEDIUM_SHOOTCHANCE 7
#define HARD_SHOOTCHANCE 4
/*
How AI works:
First, the tank checks if it sees either the player or the base and has a free bullet. If yes, it fires.
If not, it checks whether it's supposed to currently pathfind. If it's not, or if it is but it has exhausted its movelist, it generates a random new move. Otherwise, it grabs the first move it's supposed to make.
Next, it checks if it's supposed to pathfind AND its next move would attempt to take it through brick. If yes, it tries to fire a bullet. If it fails, it sits in place and does nothing.
Finally, if nothing has happened so far that would force it to shoot or sit in place and it has a free bullet, it does a difficulty-dependent random check. If it succeeds, it fires a bullet. If not,
it executes the move it's supposed to make.

Regarding pathfinding algorithms, medium tanks will not attempt to pathfind through walls, except base walls, and will just use BFS. Hard tanks are divided into regular tanks, essentially medium tanks
that start pathfinding immediately and fire randomly more often, and kamikaze tanks which use a different routine and use Dijsktra to find the quickest path to the base taking into consideration breakable brick walls.
Kamikaze tanks will only fire if their path requires it or they can destroy the base or player.
*/
char chooseMove(Tank *T, gameState G)
{
    //srand(time(NULL));
    //printf("%d %d\n",T->xPos/MAP_SCALE,T->yPos/MAP_SCALE);
    //Init parameters

    int h=G.height;
    int w=G.width;
    int by=h-4;
    int bx=w/2-2;
    int px,py;
    char chosenMove=0,m,shootMod;
    struct listNode* pTanks=G.playerTanks;
    int chasePl;
    if (T->bot==3) chasePl=1;
    else chasePl=0;
    //Check if there's a player or base in front of us. We ignore base walls since we're supposed to try to destroy them.
    if (bulletExists(T)==0)
    {
        if (seeObj(T,G,T->direction+UP,bx,by)) return SHOOT;
        while (pTanks&&pTanks->data)
        {
            px=(((Tank*)(pTanks->data))->xPos)/MAP_SCALE;
            py=(((Tank*)(pTanks->data))->yPos)/MAP_SCALE;
            if (seeObj(T,G,T->direction+UP,px,py)) return SHOOT;
            pTanks=pTanks->next;
        }
    }

    px=(((Tank*)(G.playerTanks->data))->xPos)/MAP_SCALE;
    py=(((Tank*)(G.playerTanks->data))->yPos)/MAP_SCALE;

    //Should our tank pathfind or move randomly? This function checks that and generates a movelist if needed.
    if (T->pathDone==0&&canPathfind(T,G,G.dif))
    {
        (T->mList)=genMoveList(T,G,px,py,chasePl);
        T->pathDone=1;
    }

    //We now determine our move. chosenMove distinguishes random moves and those determined via pathfinding algorithms. We're only peeking the top move of our stack since we don't necessarily want to consume it
    // - we might want to shoot instead and save the move for later.
    if (T->mList)
    {
        m=moveLook(T->mList),chosenMove=1;
        if (G.dif==HARD)
        T->pathDone--;

    }
    else m=randMove(T,G);


    //Now we check if our tank is supposed to pathfind through brick walls, and if yes we attempt to shoot that wall down. For normal tanks, this usually means only base walls since BFS avoids all other walls.
    /*if (brickInFront(T->xPos/MAP_SCALE,T->yPos/MAP_SCALE,G,m)==2&&chosenMove)
    {
        T->direction=m-UP;
        if (bulletExists(T)==0) return SHOOT;
        else return SIT;
    }*/
    //Sometimes our tank will just randomly shoot instead of moving. We now determine the likelihood of that.
    switch(G.dif)
    {
    case MEDIUM:
        shootMod=MEDIUM_SHOOTCHANCE;
        break;
    case HARD:
        shootMod=HARD_SHOOTCHANCE;
        break;
    default:
        shootMod=EASY_SHOOTCHANCE;
        break;
    }
    //We attempt to shoot randomly.
    if (rand()%shootMod==0&&bulletExists(T)==0&&isBounded(T->xPos/MAP_SCALE+dCoord[T->direction][1],T->yPos/MAP_SCALE+dCoord[T->direction][0],h,w)) return SHOOT;
    //if we got here that means we didn't fire a bullet, so now we finally return the move we're supposed to make.
    if (chosenMove) m=movePop(&(T->mList));
    return m;
}
/*
void tankPushPQ (heap_t *h, int priority, coPair data) {
    if (h->len + 1 >= h->size) {
        h->size = h->size ? h->size * 2 : 4;
        h->nodes = (node_t *)realloc(h->nodes, h->size * sizeof (node_t));
    }
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].priority > priority) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i].priority = priority;
    h->nodes[i].data = data;
    h->len++;
}

coPair tankPopPQ (heap_t *h) {
    int i, j, k;
    coPair nullRet;
    nullRet.x=-1;
    nullRet.y=-1;
    if (!h->len) {
        return nullRet;
    }
    coPair data = h->nodes[1].data;

    h->nodes[1] = h->nodes[h->len];

    h->len--;

    i = 1;
    while (i!=h->len+1) {
        k = h->len+1;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].priority < h->nodes[k].priority) {
            k = j;
        }
        if (j + 1 <= h->len && h->nodes[j + 1].priority < h->nodes[k].priority) {
            k = j + 1;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    return data;
}

char chooseMoveDJ(Tank *T, gameState G)
{
    //Init parameters
    int px=((Tank*)(G.playerTanks->data))->xPos/MAP_SCALE;
    int py=((Tank*)(G.playerTanks->data))->yPos/MAP_SCALE;
    int h=G.height;
    int w=G.width;
    int by=h-4;
    int bx=w/2-2;
    char chosenMove=0,m;

    //Check if there's a player or base in front of us. We ignore base walls since we're supposed to try to destroy them. Same as previous.
    if (bulletExists(T)==0)
        if (seeObj(T,G,T->direction+UP,px,py)||seeObj(T,G,T->direction+UP,bx,by)) return SHOOT;
    //Same as previous, only we use a different algorithm.
    if (T->pathDone==0&&canPathfind(T,G,G.dif))
    {
        (T->mList)=genMoveListDJ(T,G,2);
        T->pathDone=20;
        printf("Got new path!\n");
    }
    //Same as previous.
    if (T->mList)
        m=moveLook(T->mList),chosenMove=1;
    else m=randMove(T,G);


    //Now we check if our tank is supposed to go through a wall, and if yes we attempt to shoot that wall down. If no, we will wait until we have a free bullet. In this routine this can happen to all brick walls, not just those that guard the base.
    if (brickInFront(T->xPos/MAP_SCALE,T->yPos/MAP_SCALE,G,m)==2)
    {
        T->direction=m-UP;
        if (bulletExists(T)==0)
            return SHOOT;
        else return SIT;
    }

    if (chosenMove) m=movePop(&(T->mList)),T->pathDone--;
    return m;
}


//Dijsktra
tankMovesStack *genMoveListDJ(Tank *T, gameState G, char priority)
{
    int h=G.height,w=G.width,x=T->xPos/MAP_SCALE,y=T->yPos/MAP_SCALE;
    int i,j,dist;
    int nx,ny;
    int **weights;
    char **vis;
    coPair **pred,P,Q,R;
    heap_t *hp = (heap_t *)calloc(1, sizeof (heap_t));

    weights=(int**)(malloc(h*sizeof(int*)));
    for (i=0;i<h;i++) weights[i]=(int*)(malloc(w*sizeof(int)));

    vis=(char**)(malloc(h*sizeof(char*)));
    for (i=0;i<h;i++) vis[i]=(char*)(malloc(w));

    pred=(coPair**)(malloc(h*sizeof(coPair*)));
    for (i=0;i<h;i++) pred[i]=(coPair*)(malloc(w*sizeof(coPair)));

    for (i=0;i<h;i++)
        for (j=0;j<w;j++)
        {
            weights[i][j]=200000000;
            pred[i][j].x=-1,pred[i][j].y=-1;
            vis[i][j]=0;
        }
    weights[y][x]=0;
    P.x=x,P.y=y;
    tankPushPQ(hp,0,P);

    while (hp&&vis[h-4][w/2-2]==0)
    {
        P=tankPopPQ(hp);
        x=P.x,y=P.y;
        vis[y][x]=1;
        for (i=0;i<4;i++)
        {
            nx=x+dCoord[i][1],ny=y+dCoord[i][0];
            Q.x=nx,Q.y=ny;
            dist=brickInFront(x,y,G,i+UP);
            //if (dist==2&&isBaseBrick(G.terrain[ny][nx],nx,nx,h,w)==0) dist=priority;
            if (dist&&vis[ny][nx]==0&&weights[y][x]+dist<weights[ny][nx])
            {
                weights[ny][nx]=weights[y][x]+dist;
                pred[ny][nx]=P;
                tankPushPQ(hp,weights[ny][nx],Q);
            }
        }
    }

    #define VERT 1
    #define HORI 2
    tankMovesStack *S=NULL;
    int difx,dify,mode;
    Q=pred[h-4][w/2-4];
    R.x=-1,R.y=-1;
    while (R.x!=T->xPos/MAP_SCALE||R.y!=T->yPos/MAP_SCALE)
    {
        R=pred[Q.y][Q.x];
        difx=Q.x-R.x,dify=Q.y-R.y;
        switch (difx)
        {
            case 0: mode=VERT; break;
            default: mode=HORI; break;
        }
        char k;
        switch(mode)
        {
            case VERT:
                if (dify==1) k=DOWN;
                else if (dify==-1) k=UP;
                break;
            case HORI:
                if (difx==1) k=RIGHT;
                else if (difx==-1) k=LEFT;
                break;
        }
        movePush(&S,k);
        //printf("%d\n",k);
        Q=R;
    }

    //while (hp) tankPopPQ(hp);
    for (i=0;i<h;i++) free(vis[i]),free(weights[i]),free(pred[i]);
    free(vis); free(weights); free(pred);
    return S;
}*/


char pickMove(Tank *T, gameState G)
{
    //if (T->kamikaze) return chooseMoveDJ(T,G);
      //  else return chooseMove(T,G);
      return chooseMove(T,G);
}
