#include "strukture.h"
#include "graphics.h"
#include <stdio.h>
#include "mapgen.h"
#include "hook.h"
#include <SDL.h>
#include <SDL_image.h>
#include "menu.h"

#undef main
#define BLOCK_X 48


int main() {

	struct gameState* state = (struct gameState*)malloc(sizeof(struct gameState));
	state->enemyBullets = newNode(0);
	state->enemyTanks = newNode(0);
	state->playerBullets = newNode(0);
	state->playerTanks = newNode(0);
	state->timeStop = 0;
	generate_random_map(13, 13);
	state->terrain = read_map(&(state->height), &(state->width), "random_map");
	state->time = 0;

	SDL_Event event;
	SDL_Window *window;
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer = NULL;//inicijalizacija rendera i tekstura za crtanje
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Map Builder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BLOCK_X / 4 * 52, BLOCK_X / 4 * 52, 0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);
	
	struct Tank* player = spawnTank(state, 4, 2, 0);

	struct movementWrapper* wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
	wrap->down = 0;
	wrap->up = 0;
	wrap->left = 0;
	wrap->right = 0;
	wrap->tenkic = player;

	char ggez = 0;

	while (ggez == 0) {

		state->time += 5;
		ggez = processEvents(window, wrap, state);
		updateBullets(state);
		updateBots(state);
		doRender(state, renderer, sprites);
		if (wrap->up && player->direction == 0) Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3);
		
		SDL_Delay(1. / FPS * 1000);

	}
	return 0;
}
