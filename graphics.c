#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>

#define PLAYER_SPEED 3
#define BLOCK_X 48

#include "graphics.h"
#include "strukture.h"


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
    //Draw map
    for (int i =0;i<13*4;i++){
        for (int j =0;j<13*4;j++){
            if (!gameState->terrain[i][j]) continue;
            SDL_Rect location = {BLOCK_X + j*BLOCK_X/4, BLOCK_X + i*BLOCK_X/4, BLOCK_X/4, BLOCK_X/4};
            SDL_Rect block = {256 + ((j%4)*16/4) + (gameState->terrain[i][j]==3)*((gameState->time%90)/15)*16, (gameState->terrain[i][j]-1)*16 +((i%4)*16/4), 16/4, 16/4};
            SDL_RenderCopy(gameState->renderer, gameState->sprites, &block, &location);
        }
    }
    
    
    //We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(gameState->renderer);
}
