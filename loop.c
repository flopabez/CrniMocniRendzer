#include "strukture.h"
#include "graphics.h"
#include <stdio.h>
#include "mapgen.h"
#include "hook.h"
#include <SDL.h>
#include <SDL_image.h>

#undef main
#define BLOCK_X 48


void render_map2(SDL_Renderer* renderer, SDL_Texture* sprites, char ** map, int map_h, int map_w, struct Tank* player, int time)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i<map_h; i++) {
		for (int j = 0; j<map_w; j++) {
			if (!map[i][j]) continue;//nista za prazan blok
			if (map[i][j] == 6)
			{
				if ((i == map_h - 4) && (map[i][j + 3] == 6))
				{
					SDL_Rect brect = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X, BLOCK_X };
					SDL_Rect bdest = { 256, 80, 16, 16 };
					SDL_RenderCopy(renderer, sprites, &bdest, &brect);//dodavanje brusha u renderer
				}
			}
			else if (map[i][j] != FOREST)
			{
				SDL_Rect location = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
				SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + (map[i][j] == 3)*((time % 90) / 15) * 16 , (map[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
				SDL_RenderCopy(renderer, sprites, &block, &location);//dodavanje bloka na lokaciju u rendereru
			}
		}
	}

	SDL_Rect rect = { player->xPos, player->yPos, BLOCK_X, BLOCK_X };
	SDL_Rect dest = { 144 + 32 * player->direction, 0, 16, 16 };
	SDL_RenderCopy(renderer, sprites, &dest, &rect);//dodavanje tenka u renderer

	for (int i = 0; i<map_h; i++) {
		for (int j = 0; j<map_w; j++) {
			if (map[i][j] == FOREST)
			{
				SDL_Rect location = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
				SDL_Rect block = { 256 + ((j % 4) * 16 / 4)  , (map[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
				SDL_RenderCopy(renderer, sprites, &block, &location);//dodavanje bloka na lokaciju u rendereru

			}
		}
	}


	SDL_RenderPresent(renderer);//iscrtavanje, odnosno renderovanje
}

int main() {



	struct gameState* state = (struct gameState*)malloc(sizeof(struct gameState));
	state->enemyBullets = 0;
	state->enemyTanks = 0;
	state->playerBullets = 0;
	state->playerTanks = 0;

	struct Tank* player = (struct Tank*)malloc(sizeof(struct Tank));
	player->bot = 0;
	player->bulletPower = 1;
	player->bulletSpeed = 0;
	player->direction = 0;
	player->frame = 0;
	player->hitPoints = 1;
	player->lives = 1;
	player->inAir = 1;
	player->speed = 24*5;
	player->team = 0;
	player->score = 0;
	player->upgrade = 0;
	player->xPos = 10;
	player->yPos = 10;
	player->width = 42;

	insertBefore(&(state->playerTanks), player);
	//state->playerTanks = player;
	SDL_Event event;
	SDL_Window *window;
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer = NULL;//inicijalizacija rendera i tekstura za crtanje
	int time = 0;//vreme sluzi za animaciju vode
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Map Builder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BLOCK_X / 4 * 52, BLOCK_X / 4 * 52, 0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);
	state->renderer = &renderer;
	state->sprites = &sprites;
	state->timeStop = 0;
	generate_random_map(13, 13);
	state->terrain = read_map(&(state->height), &(state->width), "random_map");
	state->time = 0;

	struct movementWrapper* wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
	wrap->down = 0;
	wrap->up = 0;
	wrap->left = 0;
	wrap->right = 0;
	wrap->tenkic = player;
	
	struct Bullet* temp = (struct Bullet*)malloc(sizeof(struct Bullet));
	temp->direction = 3;
	temp->power = 1;
	temp->source = player;
	temp->speed = 60;
	temp->width = 12;
	temp->xPos = 20;
	temp->yPos = 100;

	char ggez = 0;
	while (ggez == 0) {


		render_map2(renderer, sprites, state->terrain, state->height, state->width, player, state->time);

		//state->time++;
		//doRender(state);
		ggez = processEvents(window, wrap);
		time++;
		if (wrap->up && player->direction == 0) Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3);
		
		SDL_Delay(1./FPS * 1000);
	}
	return 0;
}
