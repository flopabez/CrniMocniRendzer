//
//  graphics.h
//  sdl2_testing
//
//  Created by Mac on 6/1/18.
//  Copyright © 2018 Mike Farrell. All rights reserved.
//

#ifndef graphics_h
#define graphics_h

#include <stdio.h>
#include "strukture.h"

// Structs

/*typedef struct
{
    char **terrain;
    char x, y; // Dimenzije mape u velikim kvadratima (tenk je 1 kvadrat)
} Map;

typedef struct
{
    //Players
    Tank player;
    
    //Map
    Map map;
    
    //Images
    SDL_Texture *sprites;
    
    int time;
    
    //Renderer
    SDL_Renderer *renderer;
} GameState;
*/
// Functions

void loadGame(struct gameState *gameState);

void doRender(struct gameState *gameState);


#endif /* graphics_h */
