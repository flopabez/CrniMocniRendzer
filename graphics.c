#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "graphics.h"

void doRender(struct gameState *gameState, SDL_Renderer *renderer, SDL_Texture *sprites)
{
	int ofs = gameState->offset;
	//gameState->time++;
	//set the drawing color to gray
	SDL_SetRenderDrawColor(renderer, 99, 99, 99, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//Draw map background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_Rect bg = { ofs , ofs , gameState->width/4*BLOCK_X, gameState->height/4*BLOCK_X };
	SDL_RenderFillRect(renderer, &bg);

	//Draw GUI
	int xGUI = ((ofs + gameState->width / 4 * BLOCK_X) + WINDOW_W - BLOCK_X)/2;
	int yGUI = (WINDOW_H - BLOCK_X * 10.5) / 2;
	SDL_Rect tank_count_loc = { xGUI, yGUI, BLOCK_X, BLOCK_X*5 };
	SDL_Rect tank_count_sloc = {376, 24, 16, 80};
	SDL_RenderCopy(renderer, sprites, &tank_count_sloc, &tank_count_loc);

	SDL_Rect P1_loc = { xGUI, yGUI+BLOCK_X*6, BLOCK_X, BLOCK_X };
	SDL_Rect P1_sloc = { 376, 136, 16, 16 };
	SDL_RenderCopy(renderer, sprites, &P1_sloc, &P1_loc);

	if (gameState->playerTanks->next) {
		SDL_Rect P2_loc = { xGUI, yGUI + BLOCK_X * 7.5, BLOCK_X, BLOCK_X };
		SDL_Rect P2_sloc = { 376, 160, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &P2_sloc, &P2_loc);
	}

	SDL_Rect lvl_loc = { xGUI, yGUI + BLOCK_X * 9, BLOCK_X, BLOCK_X*1.5 };
	SDL_Rect lvl_sloc = { 376, 184, 16, 24 };
	SDL_RenderCopy(renderer, sprites, &lvl_sloc, &lvl_loc);

	//Draw map
	for (int i = 0; i<13 * 4; i++) {
		for (int j = 0; j<13 * 4; j++) {
			if (!gameState->terrain[i][j] || gameState->terrain[i][j] == 4) continue;
			SDL_Rect location = { ofs+j*BLOCK_X / 4, ofs + i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
			SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + (gameState->terrain[i][j] == 3)*((gameState->time % 90) / 15) * 16, (gameState->terrain[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
			SDL_RenderCopy(renderer, sprites, &block, &location);
		}
	}

	//Draw player
	struct listNode *tank_wrapper = gameState->playerTanks;
	char playerNum = 0;
	while (tank_wrapper) {
		struct Tank *player = (struct Tank*)tank_wrapper->data;
		SDL_Rect rect = { ofs + player->xPos, ofs + player->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 0 + player->frame * 16 + player->direction * 16 * 2, 0 + (playerNum * 128) + player->upgrade * 16, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		tank_wrapper = tank_wrapper->next;
	}

	//Draw enemys
	struct listNode *etank_wrapper = gameState->enemyTanks;

	while (etank_wrapper) {
		struct Tank *enemy = (struct Tank*)etank_wrapper->data;
		SDL_Rect rect = { ofs + enemy->xPos, ofs + enemy->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 0 + enemy->frame * 16 + enemy->direction * 16 * 2, 0 + enemy->upgrade * 16, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		etank_wrapper = etank_wrapper->next;
	}

	//Draw player bullets
	struct listNode *pbullet_wrapper = gameState->playerBullets;
	while (pbullet_wrapper) {
		struct Bullet *bullet = (struct Bullet*)pbullet_wrapper->data;
		SDL_Rect rect = { ofs + bullet->xPos, ofs + bullet->yPos, BLOCK_X / 4, BLOCK_X / 4 };
		SDL_Rect dest = { 322 + bullet->direction * 5, 102, 5, 4 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		pbullet_wrapper = pbullet_wrapper->next;
	}
	//Draw enemy bullets
	struct listNode *ebullet_wrapper = gameState->enemyBullets;
	while (ebullet_wrapper) {
		struct Bullet *bullet = (struct Bullet*)ebullet_wrapper->data;
		SDL_Rect rect = { ofs + bullet->xPos, ofs + bullet->yPos, BLOCK_X / 4, BLOCK_X / 4 };
		SDL_Rect dest = { 322 + bullet->direction * 5, 102, 5, 4 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		ebullet_wrapper = ebullet_wrapper->next;
	}


	//Draw trees
	for (int i = 0; i<13 * 4; i++) {
		for (int j = 0; j<13 * 4; j++) {
			if (gameState->terrain[i][j] != 4) continue;
			SDL_Rect location = { ofs + j*BLOCK_X / 4,ofs + i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
			SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + ((gameState->time % 600) / 150) * 16, (gameState->terrain[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
			SDL_RenderCopy(renderer, sprites, &block, &location);
		}
	}
	
	/*/Draw upgrade
	if (gameState->powerup) {
		SDL_Rect rect = { ofs+pow->xPos, ofs+pow->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 256 + gameState->powerup*16, 112, 16, 16};
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
	}
	//*/

	/*/Draw explosions
	struct listNode *booms = gameState->explosions;
	while (booms) {
		struct Explosion *exp = (struct Bullet*)booms->data;
		SDL_Rect rect = { ofs+exp->xPos - (BLOCK_X/2*(exp->size + 1)), ofs+exp->yPos - (BLOCK_X / 2 * (exp->size + 1)), BLOCK_X*(exp->size + 1), BLOCK_X*(exp->size + 1) };
		SDL_Rect dest = { 256 + exp->size*48 + 16*(exp->time / 15)*(exp->size+1), 128, 16*(exp->size+1), 16*(exp->size + 1) };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		exp->time++;
		if ((exp->time = 45 && exp->size = 0) || (exp->time = 30 && exp->size = 1)) removeNode(&booms);
		booms = booms->next;
	}
	//*/


	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}

//*/
