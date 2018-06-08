char chooseMove(Tank *T, gameState G)
{
    int dif=T->dif;
    int h=G.height,w=G.width,targetSelect;
    char stackMove=0,m,shootMod;

    if ((bulletExists(T)==0)&& (seeRivalTank(T,G)||seeObj(T,G,T->direction,w/2-4,h/2)))
         return SHOOT;

    if (dif==HARD)
    {
        if (G.playerTanks&&G.playerTanks->data)
        {
            T->mList=genMoveList(T,G,T->kamikaze);
        }
        else T->mList=genMoveList(T,G,0);
        T->pathDone=1;
    }
    else
    if (dif==MEDIUM)
    {
        if (T->pathDone==0&&T->yPos/MAP_SCALE>h/2) T->pathDone=1;
        T->mList=genMoveList(T,G,0);
    }
    else
    {
        if (T->pathDone==0) T->pathDone=1;
        T->mList=genMoveList(T,G,2);
    }

    if (T->mList)
    {
        m=moveLook(T->mList);
        stackMove=1;
    }
    else m=randMove(T,G);

    //Sometimes our tank will just randomly shoot instead of moving. We now determine the likelihood of that.
    switch(dif)
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
    if (stackMove) m=movePop(&(T->mList));
    return m;
}
