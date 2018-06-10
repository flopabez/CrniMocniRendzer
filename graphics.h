#ifndef graphics_h
#define graphics_h

#define PLAYER_SPEED 3
#define BLOCK_X 48
#define WINDOW_W BLOCK_X * (16+2)
#define WINDOW_H BLOCK_X * 16

#include <stdlib.h>
#include <stdio.h>
#include "strukture.h"

// Functions

void loadGame(struct gameState *gameState);

void doRender(struct gameState *gameState, SDL_Renderer *renderer, SDL_Texture *sprites);

void Loading(SDL_Renderer *renderer, SDL_Texture *sprites);

void GameOver(SDL_Renderer *renderer, SDL_Texture *sprites);

void NextStage(struct gameState *gameState, SDL_Renderer *renderer);

#endif /* graphics_h */
