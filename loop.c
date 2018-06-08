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
	state->explosions = newNode(0);
	state->enemyBullets = newNode(0);
	state->enemyTanks = newNode(0);
	state->playerBullets = newNode(0);
	state->playerTanks = newNode(0);
	state->timeStop = 0;
	generate_random_map(13, 13);
	state->terrain = read_map(&(state->height), &(state->width), "random_map");
	state->time = 0;
	state->pickup = 0;
	state->shovel = 0;
	state->dif = 2;
	state->killCount = 20;
	state->stage = 0;
	

	SDL_Event event;
	SDL_Window *window;
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer = NULL;//inicijalizacija rendera i tekstura za crtanje
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Map Builder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);

	if(!sprites) printf("Can't find file 'sprites.png'");

	struct Tank* player = spawnTank(state, 0, 2, 0);

	struct movementWrapper* wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
	wrap->down = 0;
	wrap->up = 0;
	wrap->left = 0;
	wrap->right = 0;
	wrap->tenkic = player;

	doMenu(window, renderer, sprites);
	char ggez = 0;
	float delay = 1. / FPS * 1000;
	struct Tank* bot = spawnTank(state, 1, 0, 1);
	bot->pickup = 1;
	state->timeStop = 10 * 24;

	while (ggez == 0) {

		char moving = wrap->up || wrap->down || wrap->left || wrap->right;
		if (!(state->time % 2) && moving) player->frame = (player->frame + 1) % 2; // animacija gusenica


		ggez = processEvents(window, wrap, state);
		if (wrap->up && player->direction == 0) Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3); // kretanje igraca

		
		powerUp(state);
		updatePowerUps(state);
		hitDetection(state);
		updateBullets(state);
		if (state->timeStop == 0) updateBots(state);
		doRender(state, renderer, sprites); // updateovanje gamestatea
	


		state->time++;
		SDL_Delay(delay);

	}
	return 0;
}
