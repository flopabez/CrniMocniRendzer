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

#endif /* graphics_h */

/*typedef struct explosion {
	int xPos, yPos;
	char size;
	int time;
} Explosion;


void Boom(int xPos, int yPos, char size, struct listNode* lista_ekspolzija, struct gameState* gameState) {
	Explosion *temp = malloc(sizeof(Explosion));
	temp->xPos = xPos;
	temp->yPos = yPos;
	temp->size = size;
	temp->time = 0;
	insertBefore();
}*/
