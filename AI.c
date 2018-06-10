#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AI.h"


void destroyMoves(Tank *T)
{
    while (T->mList) movePop(T->mList);
}

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
    if (T->inAir<1) return 0;
    else return 1;
}
//Checks if we haven't left the map boundaries
int isBounded(int x, int y, int h, int w)
{
    return (y>=0&&y<h&&x>=0&&x<w);
}
//Checks if the given tile is part of the brick wall surrounding the base.
int isBaseBrick(char c, int x, int y, int h, int w, char shovel)
{
    return (isBounded(x,y,h,w)&&(c==BRICK||(shovel&&c==METAL))&&y<h&&y>=h-6&&x>=(w/2-6)&&x<=(w/2+5));
}

//Checks if it's possible to see point [ty][tx] from point [sy][sx], assuming a limited line of sight distance and ignoring the base's brick wall.
int seeLine(int sx,int sy, char orientation, gameState G, int tx, int ty)
{
    int i;
    int x=sx,y=sy;
    int h=G.height,w=G.width;
    //int limit=h;
    //if (limit<w) limit=w;
    int  limit=20;
    for (i=0;i<limit&&isBounded(x,y,h,w);i++)
    {
        char c=(G.terrain)[y][x];
        if (y>=ty&&y<=ty+3&&x>=tx&&x<=tx+3) return 1;
        if (!(c==EMPTY||c==WATER||c==FOREST||c==BASE||c==ICE||isBaseBrick(c,x,y,h,w,G.shovel))) return 0;
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
    return (seeLine(x1,y1,dir,G,tx,ty)||seeLine(x2,y2,dir,G,tx,ty));
}


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
        baseWall=isBaseBrick(c,x+j,y+i,h,w,G.shovel);
        if (c!=EMPTY&&c!=FOREST&&c!=ICE&&c!=BASE&&baseWall==0) return 0;
    }
    return 1;
}

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
    char R[4]={0};
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

tankMovesStack* genMoveList(Tank *T,gameState G, char chaseMode)
{
    int h=G.height,w=G.width,x=T->xPos/MAP_SCALE,y=T->yPos/MAP_SCALE;
    int i,j;
    int nx,ny;
    char **vis,*randM;
    coPair **pred,P,Q,targetPair,predPair;
    moveQueue *front=NULL,*rear=NULL;

    vis=(char**)(malloc(h*sizeof(char*)));
    for (i=0;i<h;i++) vis[i]=(char*)(calloc(w,1));

    pred=(coPair**)(malloc(h*sizeof(coPair*)));
    for (i=0;i<h;i++) pred[i]=(coPair*)(malloc(w*sizeof(coPair)));

    targetPair.x=-1,targetPair.y=-1;
    for (i=0;i<h;i++)
        for (j=0;j<w;j++)
            pred[i][j].x=-1,pred[i][j].y=-1;

    P.x=x,P.y=y;
    enq(&front,&rear,P);

    while (front)
    {
        P=deq(&front,&rear);
        vis[P.y][P.x]=1;
        randM=randarr_m();
        for (j=0;j<4;j++)
        {
            i=randM[j];
            //i=j;
            nx=dCoord[i][1]+P.x;
            ny=dCoord[i][0]+P.y;
            if (tileFree(nx,ny,G)&&vis[ny][nx]==0&&pred[ny][nx].x==-1)
            {
                pred[ny][nx].x=P.x;
                pred[ny][nx].y=P.y;
                Q.x=nx; Q.y=ny;
                enq(&front,&rear,Q);
            }
        }
    }

    int plX,plY;

    switch(chaseMode)
    {
    case 0:
        if (pred[h-4][w-2/2].x!=-1&&pred[h-4][w-2/2].y!=-1)
        {
            targetPair.x=h-4;
            targetPair.y=w/2-2;
        }
        break;
    case 1:
        if (G.playerTanks&&G.playerTanks->data)
        {
            plX=((Tank*)(G.playerTanks->data))->xPos/MAP_SCALE;
            plY=((Tank*)(G.playerTanks->data))->yPos/MAP_SCALE;
            if (isBounded(plX,plY,h,w)) targetPair=pred[plY][plX];
        }
        break;
    }

     while ((targetPair.x==-1)||(targetPair.y==-1)||(pred[targetPair.y][targetPair.x].x==-1)||(pred[targetPair.y][targetPair.x].y==-1)||targetPair.y<h/2)
    {
        targetPair.x=rand()%w;
        targetPair.y=rand()%h;
    }
    //printf("Tank coords: %d %d\nTarget coords: %d %d\nTank difficulty: %d\nFirst move: %d\nPaths left: %d\n\n",T->yPos/MAP_SCALE,T->xPos/MAP_SCALE,targetPair.y,targetPair.x,T->dif,moveLook(T->mList),T->pathDone-1);


    #define VERT 1
    #define HORI 2
    tankMovesStack *S=NULL;
    int difx,dify,mode;
    predPair.x=-1,predPair.y=-1;
    while (predPair.x!=T->xPos/MAP_SCALE||predPair.y!=T->yPos/MAP_SCALE)
    {
        predPair=pred[targetPair.y][targetPair.x];
        difx=targetPair.x-predPair.x,dify=targetPair.y-predPair.y;
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
        targetPair=predPair;
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
        return (1-T->pathDone);
    case MEDIUM:
        return ((0.1*T->yPos/MAP_SCALE)>(0.1*G.height/2)||T->mList);
    case HARD:
        return 1;
    default:
        return 0;
    }
}

/*int tileFreeTank(int x, int y, gameState G, Tank *T)
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
                return 0;
            }
            tankList=tankList->next;
        }
        return 1;

    }
    return 0;
}*/




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
    int nx,ny;
    char m;
    do
    {
        m=getMove(T);
        nx=T->xPos/MAP_SCALE+dCoord[m][1];
        ny=T->yPos/MAP_SCALE+dCoord[m][0];
    } while (tileFree(nx,ny,G)==0&&tankCollision(&G,T)==NULL);
    return m;
}

#define EASY_SHOOTCHANCE 20
#define MEDIUM_SHOOTCHANCE 10
#define HARD_SHOOTCHANCE 5
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

char seeRivalTank(Tank *T, gameState G)
{
    int ty,tx;
    Tank *cT;

    struct listNode *tList;
    if (T->team) tList=G.playerTanks;
    else tList=G.enemyTanks;

    while (tList&&tList->data)
    {
        cT=(Tank*)(tList->data);
        ty=cT->yPos/MAP_SCALE;
        tx=cT->xPos/MAP_SCALE;
        if (seeObj(T,G,T->direction,tx,ty)) return 1;
        tList=tList->next;
    }
    return 0;
}

char seeRivalBullet(Tank *T, gameState G)
{
    if (T->dif<MEDIUM)  return 0;
    int by,bx;
    Bullet *cB;
    struct listNode *bList;
    if (T->team) bList=G.playerBullets;
    else bList=G.enemyBullets;

    while (bList&&bList->data)
    {
        cB=(Bullet*)(bList->data);
        by=cB->yPos/MAP_SCALE;
        bx=cB->xPos/MAP_SCALE;
        if (seeObj(T,G,T->direction,bx,by)) return 1;
        bList=bList->next;
    }
    return 0;
}

char tanksCollide(Tank* T, gameState G, char dir)
{
    int r=0;
    int speed=T->speed;
    if (speed<0) speed=0-speed;
    int x=T->xPos;
    int y=T->yPos;
    int newx=x+speed*dCoord[dir][1];
    int newy=y+speed*dCoord[dir][0];
    T->xPos=newx;
    T->yPos=newy;
    if (tankCollision(&G,T)) r=1;
    if (isBounded(newx/MAP_SCALE,newy/MAP_SCALE,G.height,G.width)==0) r=1;
    if (tileFree(newx/MAP_SCALE,newy/MAP_SCALE,G)==0) r=1;
    T->xPos=x;
    T->yPos=y;
    return r;
}

#define EASY_PATH_NUM 3
#define MEDIUM_PATH_NUM 2
#define HARD_PATH_NUM 1

char chooseMove(Tank *T, gameState G)
{
    //if (T->inAir<0) printf("%d %d %d\n",T->yPos/MAP_SCALE,T->xPos/MAP_SCALE,T->inAir),T->inAir=0;
    int dif=T->dif;
    int h=G.height,w=G.width,targetSelect;
    int by=h-4;
    int bx=w/2-2;



    char stackMove=0,m,shootMod,chaseMode=0,pathnum;

    if (T->inAir==0)
        {
            if (seeRivalTank(T,G))
                return SHOOT;
            if (seeRivalBullet(T,G))
                return SHOOT;
            if ((seeObj(T,G,T->direction+UP,w/2-2,h-4))||(seeObj(T,G,T->direction+UP,w/2+1,h-4))||(seeObj(T,G,T->direction+UP,w/2+1,h-4))||(seeObj(T,G,T->direction+UP,w/2+1,h-1)))
              return SHOOT;
        }

    switch (dif)
    {
    case HARD:
        if (G.playerTanks&&G.playerTanks->data) chaseMode=T->kamikaze;
        /*T->mList=genMoveList(T,G,chaseMode);
        T->pathDone=1;
        */
        pathnum=HARD_PATH_NUM;
        shootMod=HARD_SHOOTCHANCE;
        break;
    case MEDIUM:
        /*if (T->pathDone==0&&T->yPos/MAP_SCALE>h/3)
        {
        T->pathDone=1;
        T->mList=genMoveList(T,G,0);
        }*/

        pathnum=MEDIUM_PATH_NUM;
        shootMod=MEDIUM_SHOOTCHANCE;
        break;
    default:
        /*if (T->pathDone==0)
        {
        T->pathDone=1;
        T->mList=genMoveList(T,G,2);
        }*/
        pathnum=EASY_PATH_NUM;
        shootMod=EASY_SHOOTCHANCE;
        break;
    }

    if (T->mList&&tanksCollide(T,G,T->mList->stackMove))
    {
        destroyMoves(T);
        T->pathDone=0;
    }

    if (T->pathDone==0) T->pathDone=pathnum;


    if (T->mList==NULL)
    {
        if (T->pathDone==1)
        {
            T->mList=genMoveList(T,G,chaseMode);
            T->pathDone=pathnum;
            //stackMove=1;
        }
        else
        {
            T->mList=genMoveList(T,G,2);
            T->pathDone--;
        }
    }

    //We attempt to shoot randomly.
    if (rand()%shootMod==0&&bulletExists(T)==0&&isBounded(T->xPos/MAP_SCALE+dCoord[T->direction][1],T->yPos/MAP_SCALE+dCoord[T->direction][0],h,w)) return SHOOT;
    //if we got here that means we didn't fire a bullet, so now we finally return the move we're supposed to make.
    m=movePop(&(T->mList));
    return m;
}


char pickMove(Tank *T, gameState G)
{
    //if (T->kamikaze) return chooseMoveDJ(T,G);
      //  else return chooseMove(T,G);
      if (T->inAir<0) printf("%d %d\n",T->yPos/MAP_SCALE,T->xPos/MAP_SCALE),T->inAir=0;
      return chooseMove(T,G);
}
