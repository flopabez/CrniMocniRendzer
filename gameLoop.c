#include "strukture.h"
#include "graphics.h"
#include "mapgen.h"
#include "hook.h"
#include "menu.h"
#include "scores.h"
#include "sound.h"
#include <SDL_ttf.h>
#include <time.h>

#undef main

int main() {

	SDL_Window* window;
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer = NULL;//inicijalizacija rendera i tekstura za crtanje
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("./resursi/sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);
	if (!sprites) printf("Can't find file 'sprites.png'");
	//inicijalizacija prozora

	TTF_Init();

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
			state = initGame(settings);
			player = spawnTank(state, 0, 2, 0);
			wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
			wrap->down = 0;
			wrap->up = 0;
			wrap->left = 0;
			wrap->right = 0;
			wrap->tenkic = player;
			break;
		case 2:;
			PlayIt();
			done = 1;
			break;
		case 3:
			doOptions(settings, window, renderer, sprites);
			break;
		case 4:
			build_map(window, renderer, sprites, surface, settings->height, settings->width);
			break;
		case 5:
			highscore(window, renderer);
			break;
		case 6:
			freeGame(state);
			return 0;
		}
		
	}
	//MainMenu


	float delay = 1. / FPS * 1000 - 8;
	int spawnDelay = 0;
	int maxDelay = 24 * (4 - settings->dif);
	char maxOnscreen = 3 + settings->dif;
	done = 0;
	//promenljive igre


	while (done != 1) {
		
		char moving = wrap->up || wrap->down || wrap->left || wrap->right;
		if (!(state->time % 2) && moving) player->frame = (player->frame + 1) % 2;
		//animacija igracevog tenka

		done = processEvents(window, wrap, state);
		if (done == 2) {
			PlayMenuMusic();
			while (done != 1) {

				done = doMenu(window, renderer, sprites, state != 0);

				switch (done) {
				case 1:;
					maxDelay = 24 * (4 - settings->dif);
					maxOnscreen = 3 + settings->dif;
					spawnDelay = 0;
					freeGame(state);
					//free(player);
					//free(wrap);
					state = initGame(settings);
					player = spawnTank(state, 0, 2, 0);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 2:;
					PlayIt();
					done = 1;
					break;
				case 3:
					doOptions(settings, window, renderer, sprites);
					break;
				case 4:
					build_map(window, renderer, sprites, surface, settings->height, settings->width);
					break;
				case 5:
					highscore(window, renderer);
					break;
				case 6:
					freeGame(state);
					return 0;
				}

			}
			maxOnscreen = 3 + settings->dif;
			done = 0;
		}
		//hvatanje inputa i paljenje menija

		switch (done) {
		case 'y':;
			spawnDelay = 0;
			state->killCount = 0;
			while (state->enemyTanks->next) removeNode(state->enemyTanks);
			break;
		case 'u':;
			player->shield = 24 * 10; break;
		case 'i':;
			state->timeStop = 24 * 10; break;
		case 'o':;
			state->shovel = 24 * 20;
			setBase(state, 2);
			break;
		case 'p':;
			player->upgrade += player->upgrade == 3 ? 0 : 1; break;
		case '[':;
			struct listNode* temp = player->team ? state->playerTanks : state->enemyTanks;
			while (temp->data) {
				Explosion* boom = (Explosion*)malloc(sizeof(Explosion));
				boom->size = 1;
				boom->time = 0;
				boom->yPos = ((struct Tank*)temp->data)->yPos;
				boom->xPos = ((struct Tank*)temp->data)->xPos;
				insertBefore(&state->explosions, boom);

				state->killCount--;
				free(temp->data);
				temp->data = 0;
				removeNode(temp);
			}
			break;
		case ']':;
			player->lives++; break;
		case 'h':;
			maxDelay = 0;
			maxOnscreen = 100;
			break;
		default: break;
		}


		if (wrap->up && player->direction == 0)	Move(state, player, 0);
		if (wrap->left && player->direction == 1) Move(state, player, 1);
		if (wrap->down && player->direction == 2) Move(state, player, 2);
		if (wrap->right && player->direction == 3) Move(state, player, 3);
		//kretanje igracevog tenka
		if (state->timeStop == 0) updateBots(state); //trosenje poteza botova


		powerUp(state); //kolizija sa pickup-ovima i njihovo dodeljivanje
		updatePowerUps(state); //regulise pravila pickup-ova
		hitDetection(state); //kolizija metkova sa okolinom
		updateBullets(state); //kretanje metkova
		doRender(state, renderer, sprites); //renderovanje
		if (state->playerTanks->data == 0 || baseHitDetection(state)) {
			//ovde treba render gameOver
			if (baseHitDetection(state)) BaseBoomSound();
			GameOver(renderer, sprites);
			update_score(100*player->score,window,renderer);
			done = 0;
			PlayMenuMusic();
			while (done != 1) {

				done = doMenu(window, renderer, sprites, state != 0);

				switch (done) {
				case 1:;
					maxDelay = 24 * (4 - settings->dif);
					maxOnscreen = 3 + settings->dif;
					spawnDelay = 0;
					freeGame(state);
					//free(player);
					//free(wrap);
					state = initGame(settings);
					player = spawnTank(state, 0, 2, 0);
					wrap = (struct movementWrapper*)malloc(sizeof(struct movementWrapper));
					wrap->down = 0;
					wrap->up = 0;
					wrap->left = 0;
					wrap->right = 0;
					wrap->tenkic = player;
					break;
				case 2:;
					PlayIt();
					done = 1;
					break;
				case 3:
					doOptions(settings, window, renderer, sprites);
					break;
				case 4:
					build_map(window, renderer, sprites, surface, settings->height, settings->width);
					break;
				case 5:
					highscore(window, renderer);
					break;
				case 6:
					freeGame(state);
					return 0;
				}

			}
			maxOnscreen = 3 + settings->dif;
			done = 0;
		}
		//provera za game over
		
		
		struct Tank* check = 0;
		if (!(state->killCount <= botCount(state) || state->killCount == 0 || spawnDelay || state->timeStop || botCount(state) > maxOnscreen))
				check = spawnTank(state, 1 + ((random(5) <= settings->dif) ? random(4) : 0), random(2), 1);
		if (spawnDelay == 0 && check) spawnDelay = maxDelay;
		if (spawnDelay) spawnDelay--;
		//uslovno spawnovanje tenkova


		if (state->killCount == 0 && botCount(state) == 0) {
			NextStage(state, renderer);
			state->stage++;
			//freeMap(state);
			free(state->pickup);
			state->pickup = 0;
			state->terrain = get_map(state->stage, settings->height, settings->width);
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
