#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

#include "graphics.h"

#define START_POSX 7*BLOCK_X
#define START_POSY 7*BLOCK_X
 
 void loadGame(struct gameState *gameState)
 {
 SDL_Surface *surface = NULL;
    
     //Load images and create ndering textures from them
     surface = IMG_Load("/Users/mac/Documents/Programiranje/Tenkici/sdl2_testing/resursi/sprites.png");
     if (surface == NULL)
     {
         printf("Cannot find sprites.png!\n\n");
        
         SDL_DestroyRenderer(gameState->renderer);
         SDL_Quit();
         exit(1);
     }
     
     gameState->sprites = SDL_CreateTextureFromSurface(gameState->renderer, surface);
     SDL_FreeSurface(surface);

    
     gameState->time = 0;
     gameState->enemyBullets=NULL;
     gameState->playerBullets=NULL;
     gameState->enemyTanks=NULL;
     gameState->playerTanks=malloc(sizeof(struct listNode));
     gameState->playerTanks->next=NULL;
     gameState->playerTanks->data=malloc(sizeof(struct Tank));
     
     struct Tank *player= (struct Tank*)gameState->playerTanks->data;
     
     player->xPos = START_POSX;
     player->yPos = START_POSY;
     player->upgrade = 0;
     player->direction = 0;
     player->frame = 0;
          
     //Ovde ce mozda biti renderovanje mape
    
}

void doRender(struct gameState *gameState)
{
    //set the drawing color to blue
    SDL_SetRenderDrawColor(gameState->renderer, 0, 0, 0, 0);
    
    //Clear the screen (to blue)
    SDL_RenderClear(gameState->renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, 255);
    
    //Draw map
    for (int i =0;i<13*4;i++){
        for (int j =0;j<13*4;j++){
            if (!gameState->terrain[i][j] || gameState->terrain[i][j]==4) continue;
            SDL_Rect location = {BLOCK_X + j*BLOCK_X/4, BLOCK_X + i*BLOCK_X/4, BLOCK_X/4, BLOCK_X/4};
            SDL_Rect block = {256 + ((j%4)*16/4) + (gameState->terrain[i][j]==3)*((gameState->time%90)/15)*16, (gameState->terrain[i][j]-1)*16 +((i%4)*16/4), 16/4, 16/4};
            SDL_RenderCopy(gameState->renderer, gameState->sprites, &block, &location);
        }
    }
    
    //Draw player
    struct listNode *tank_wrapper= gameState->playerTanks;
    char playerNum=0;
    while (tank_wrapper){
        struct Tank *player = (struct Tank*)tank_wrapper->data;
        SDL_Rect rect = {player->xPos, player->yPos, BLOCK_X, BLOCK_X};
        SDL_Rect dest = {0 + player->frame*16 + player->direction*16*2, 0 + (playerNum*128) + player->upgrade*16, 16, 16};
        SDL_RenderCopy(gameState->renderer, gameState->sprites, &dest, &rect);
        playerNum++;
        tank_wrapper=tank_wrapper->next;
    }
    
    //Draw enemys
    struct listNode *etank_wrapper= gameState->enemyTanks;
    char enemyNum=0;
    while (etank_wrapper){
        struct Tank *enemy = (struct Tank*)etank_wrapper->data;
        SDL_Rect rect = {enemy->xPos, enemy->yPos, BLOCK_X, BLOCK_X};
        SDL_Rect dest = {0 + enemy->frame*16 + enemy->direction*16*2, 0 + (enemyNum*128) + enemy->upgrade*16, 16, 16};
        SDL_RenderCopy(gameState->renderer, gameState->sprites, &dest, &rect);
        enemyNum++;
        etank_wrapper=etank_wrapper->next;
    }
    /*
    //Draw player bullets
    struct listNode *pbullet_wrapper= gameState->playerBullets;
    while (pbullet_wrapper){
        struct Bullet *bullet = (struct Bullet*)pbullet_wrapper->data;
        SDL_Rect rect = {bullet->xPos, bullet->yPos, BLOCK_X/4, BLOCK_X/4};
        SDL_Rect dest = {322 + bullet->direction*5, 102, 5, 4};
        SDL_RenderCopy(gameState->renderer, gameState->sprites, &dest, &rect);
        playerNum++;
        pbullet_wrapper=pbullet_wrapper->next;
    }
    //Draw enemy bullets
    struct listNode *ebullet_wrapper= gameState->enemyBullets;
    while (ebullet_wrapper){
        struct Bullet *bullet = (struct Bullet*)ebullet_wrapper->data;
        SDL_Rect rect = {bullet->xPos, bullet->yPos, BLOCK_X/4, BLOCK_X/4};
        SDL_Rect dest = {322 + bullet->direction*5, 102, 5, 4};
        SDL_RenderCopy(gameState->renderer, gameState->sprites, &dest, &rect);
        playerNum++;
        ebullet_wrapper=ebullet_wrapper->next;
    }*/
    
    
    //Draw trees
    for (int i =0;i<13*4;i++){
        for (int j =0;j<13*4;j++){
            if (gameState->terrain[i][j]!=4) continue;
            SDL_Rect location = {BLOCK_X + j*BLOCK_X/4, BLOCK_X + i*BLOCK_X/4, BLOCK_X/4, BLOCK_X/4};
            SDL_Rect block = {256 + ((j%4)*16/4) + ((gameState->time%600)/150)*16, (gameState->terrain[i][j]-1)*16 +((i%4)*16/4), 16/4, 16/4};
            SDL_RenderCopy(gameState->renderer, gameState->sprites, &block, &location);
        }
    }
    

    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(gameState->renderer);
}
//*/
