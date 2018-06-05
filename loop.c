#include "strukture.h"
#include "graphics.h"
#include <stdio.h>
#include "mapgen.h"
#include "hook.h"

#undef main
#define BLOCK_X 48
#define FPS 60

int main() {
	
	struct gameState *state = (struct gameState*)malloc(sizeof(struct gameState));
	SDL_Window *window = NULL;                    // Declare a window
	SDL_Renderer *renderer = NULL;                // Declare a renderer

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	(*state).playerTanks = 0;
	struct Tank* player = (struct Tank*)malloc(sizeof(struct Tank));
	insertBefore(&((*state).playerTanks), player);
	(*state).width = 52;
	(*state).height = 52;
	(*state).terrain = (char**)malloc(state->height * sizeof(char*));
	for (int i = 0; i < state->height; i++) state->terrain[i] = (char*)calloc(state->width,sizeof(char));
	state->enemyBullets = 0;
	state->enemyTanks = 0;
	state->playerBullets = 0;
	player->frame = 0;
	player->xPos = 20;
	player->yPos = 0;
	player->speed = 60;
	player->width = 48;
	player->direction = 0;

	//Create an application window with the following settings:
	window = SDL_CreateWindow("Battle City",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		15 * BLOCK_X,                               // width, in pixels
		15 * BLOCK_X,                               // height, in pixels
		0                                  // flags
	);
	(*state).renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	loadGame(state);

	char ggez = 0;
	read_map(state->terrain, &(state->height), &(state->width), "bmap");
	struct movementWrapper* wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
	(*wrap).down = 0;
	(*wrap).up = 0;
	(*wrap).left = 0;
	(*wrap).right = 0;
	(*wrap).tenkic = player;

	while (ggez == 0) {

		doRender(state);
		ggez = processEvents(window, wrap);
		if (wrap->down && wrap->tenkic->direction==2) Move(state, wrap->tenkic, 2);
		if (wrap->up && wrap->tenkic->direction == 0) Move(state, wrap->tenkic, 0);
		if (wrap->left && wrap->tenkic->direction == 1) Move(state, wrap->tenkic, 1);
		if (wrap->right && wrap->tenkic->direction == 3) Move(state, wrap->tenkic, 3);

		
		system("cls");
		printf("x:%d\ny:%d", wrap->tenkic->xPos, wrap->tenkic->yPos);
		printf("up %d left %d\ndown %d right %d", wrap->up, wrap->left, wrap->down, wrap->right);
		
		SDL_Delay(20);
	}
	return 0;
}
