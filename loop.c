#include "strukture.h"
#include "graphics.h"
#include <stdio.h>
#include "mapgen.h"
#include "hook.h"
#include <SDL.h>
#include <SDL_image.h>
#include "menu.h"
#include "scores.h"

#undef main
#define BLOCK_X 48


int main() {


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
	if (!sprites) printf("Can't find file 'sprites.png'");

	srand(time(NULL));
	struct gameState* state = 0;
	

	char ggez = 0;
	
	while (ggez != 1) {
		ggez = doMenu(window, renderer, sprites, 0);
		switch (ggez) {
		case 1:;
			printf("%d", ggez);
			state = initGame(0);
			printf("%d", ggez);
			break;
		case 3:
			show_score();
			break;
		case 4:;
			int a, b;
			printf("visina i sirina mape: ");
			scanf("%d%d", &a, &b);
			build_map(a, b);
			break;
		case 5: break;
		case 6:;
			return 0;
			break;
		}
	}
	
	ggez = 0;

	struct Tank* player = spawnTank(state, 0, 2, 0);

	struct movementWrapper* wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
	wrap->down = 0;
	wrap->up = 0;
	wrap->left = 0;
	wrap->right = 0;
	wrap->tenkic = player;


	float delay = 1. / FPS * 1000;
	int spawnDelay = 24 * (4 - state->dif);
	char maxOnscreen = 3 + state->dif;

	while (ggez == 0) {

		char moving = wrap->up || wrap->down || wrap->left || wrap->right;
		if (!(state->time % 2) && moving) player->frame = (player->frame + 1) % 2; // animacija gusenica


		ggez = processEvents(window, wrap, state);
		if (ggez == -1) {
			while (ggez != 1) {
				ggez = doMenu(window, renderer, sprites, 1);
				switch (ggez) {
				case 1:;
					free(state);
					state = initGame(0);
					free(player);
					player = spawnTank(state, 0, 2, 0);
					free(wrap);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 2:;
					ggez = 1;
					break;
				case 3:
					show_score();
					break;
				case 4:;
					int a, b;
					printf("visina i sirina mape: ");
					scanf("%d%d", &a, &b);
					build_map(a, b);
					break;
				case 5: break;
				case 6:;
					free(state);
					return 0;
					break;
				}
			}
			ggez = 0;
		}

		if (wrap->up && player->direction == 0) Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3); // kretanje igraca

		
		powerUp(state);
		updatePowerUps(state);
		hitDetection(state);
		if (player == 0 || baseHitDetection(state)) {
			SDL_Rect dest2 = { 288, 184, 32, 16 };
			SDL_Rect rect = { state->width * 2, state->height * 2, BLOCK_X * 5 , BLOCK_X * 4 };
			SDL_RenderCopy(renderer, sprites, &dest2, &rect);
			
			SDL_Delay(5 * 1000);
			while (ggez != 1) {
				ggez = doMenu(window, renderer, sprites, 1);
				switch (ggez) {
				case 1:;
					free(state);
					state = initGame(0);
					if (player) free(player);
					player = spawnTank(state, 0, 2, 0);
					free(wrap);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 3: break;
				case 4:;
					int a, b;
					printf("visina i sirina mape: ");
					scanf("%d%d", &a, &b);
					build_map(a, b);
					break;
				case 5: break;
				case 6:;
					free(state);
					return 0;
					break;
				}
			}
			ggez = 0;
		}
		updateBullets(state);
		if (state->timeStop == 0) updateBots(state);
		doRender(state, renderer, sprites); // updateovanje gamestatea
	
		
		if ( !(state->killCount <= botCount(state) || state->killCount == 0 || spawnDelay || botCount(state) > maxOnscreen) ) spawnTank(state, 1 + random(4), random(2), 1);
		if (spawnDelay == 0) spawnDelay = 24 * (4 - state->dif);
		spawnDelay--; //spawnovanje botova

		if (state->killCount == 0 && botCount(state) == 0) {
			state->stage++;
			for (int i = 0; i < state->height; i++)
				free(state->terrain[i]);
			free(state->terrain);
			free(state->pickup);
			state->pickup = 0;
			generate_random_map(13, 13);
			state->terrain = read_map(&(state->height), &(state->width), "random_map");
			player->yPos = (state->height - 10)*MAP_SCALE;
			player->xPos = (state->width / 2 - 2)* MAP_SCALE;
		}
		//prelazenje nivoa

		state->time++;
		SDL_Delay(delay);
	}
	return 0;
}