#include "strukture.h"
#include "graphics.h"
#include "mapgen.h"
#include "hook.h"
#include "menu.h"
#include "scores.h"
#include "sound.h"

#undef main

int main() {

	SDL_Window* window;
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
	//inicijalizacija prozora


	srand(time(0));

	struct gameState* state = 0;
	OptionsReturnStructure* settings = (OptionsReturnStructure*)malloc(sizeof(OptionsReturnStructure));
	settings->dif = 1;
	settings->height = 13 * 4;
	settings->width = 13 * 4;

	struct Tank* player = 0;
	struct movementWrapper* wrap = 0;

	char done = 0;
	while (done != 1) {

		done = doMenu(window, renderer, sprites, state != 0);

		switch (done) {
		case 1:;
			freeGame(state);
			state = initGame(settings->dif);
			player = spawnTank(state, 0, 2, 0);
			wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
			wrap->down = 0;
			wrap->up = 0;
			wrap->left = 0;
			wrap->right = 0;
			wrap->tenkic = player;
			break;
		case 2:;
			done = 1;
			break;
		case 3:
			doOptions(settings, window, renderer, sprites);
			break;
		case 4:
			build_map(window, renderer, sprites, surface, settings->height, settings->width);
			break;
		case 5:
			show_score();
			break;
		case 6:
			freeGame(state);
			return 0;
		}
		
	}
	//MainMenu


	float delay = 1. / FPS * 1000;
	int spawnDelay = 0; // 24 * (4 - state->dif);
	char maxOnscreen = 3 + state->dif;
	done = 0;
	//promenljive igre


	while (done == 0) {

		char moving = wrap->up || wrap->down || wrap->left || wrap->right;
		if (!(state->time % 2) && moving) player->frame = (player->frame + 1) % 2;
		//animacija igracevog tenka

		done = processEvents(window, wrap, state);
		if (done == -1) {
			while (done != 1) {

				done = doMenu(window, renderer, sprites, state != 0);

				switch (done) {
				case 1:;
					freeGame(state);
					//free(player);
					//free(wrap);
					state = initGame(settings->dif);
					player = spawnTank(state, 0, 2, 0);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 2:;
					done = 1;
					break;
				case 3:
					doOptions(settings, window, renderer, sprites);
					break;
				case 4:
					build_map(window, renderer, sprites, surface, settings->height, settings->width);
					break;
				case 5:
					show_score();
					break;
				case 6:
					freeGame(state);
					return 0;
				}

			}
			done = 0;
		}
		//hvatanje inputa i paljenje menija

		if (wrap->up && player->direction == 0) Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3);
		//kretanje igracevog tenka
		if (state->timeStop == 0) updateBots(state); //trosenje poteza botova

		powerUp(state); //kolizija sa pickup-ovima i njihovo dodeljivanje
		updatePowerUps(state); //regulise pravila pickup-ova
		hitDetection(state); //kolizija metkova sa okolinom
		updateBullets(state); //kretanje metkova
		if (state->playerTanks->data == 0 || baseHitDetection(state)) {
			//ovde treba render gameOver
			SDL_Delay(5 * 1000);
			done = 0;
			freeGame(state);
			state = 0;
			while (done != 1) {

				done = doMenu(window, renderer, sprites, state != 0);

				switch (done) {
				case 1:;
					freeGame(state);
					//free(player);
					//free(wrap);
					state = initGame(settings->dif);
					player = spawnTank(state, 0, 2, 0);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 2:;
					done = 1;
					break;
				case 3:
					doOptions(settings, window, renderer, sprites);
					break;
				case 4:
					build_map(window, renderer, sprites, surface, settings->height, settings->width);
					break;
				case 5:
					show_score();
					break;
				case 6:
					freeGame(state);
					return 0;
				}

			}
			done = 0;
		}
		//provera za game over
		doRender(state, renderer, sprites); //renderovanje
		
		struct Tank* check = 0;
		if (!(state->killCount <= botCount(state) ||state->killCount == 0 || spawnDelay || state->timeStop || botCount(state) > maxOnscreen))
			check = spawnTank(state, 1 + random(4), random(2), 1);
		if (spawnDelay == 0 && check) spawnDelay = 24 * (4 - state->dif);
		spawnDelay--;
		//uslovno spawnovanje tenkova

		if (state->killCount == 0 && botCount(state) == 0) {
			state->stage++;
			//freeMap(state);
			free(state->pickup);
			state->pickup = 0;
			generate_random_map(13, 13);
			state->terrain = read_map(&(state->height), &(state->width), "random_map");
			player->yPos = (state->height - 10)*MAP_SCALE;
			player->xPos = (state->width / 2 - 2)* MAP_SCALE;
			state->killCount = 20;
		}
		//prelazenje mape

		state->time++;
		SDL_Delay(delay);

	}
	return 0;

}