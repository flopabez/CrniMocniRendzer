#ifndef graphics_h
#define graphics_h

#define PLAYER_SPEED 3
#define BLOCK_X 48

#include <stdlib.h>
#include <stdio.h>
#include "strukture.h"

// Functions

void loadGame(struct gameState *gameState);

void doRender(struct gameState *gameState, SDL_Renderer *renderer, SDL_Texture *sprites);


#endif /* graphics_h */
