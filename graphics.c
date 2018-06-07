#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "graphics.h"

void doRender(struct gameState *gameState, SDL_Renderer *renderer, SDL_Texture *sprites)
{
	int xofs = (WINDOW_H - gameState->width / 4 * BLOCK_X) / 2;
	int yofs = (WINDOW_H - gameState->height / 4 * BLOCK_X) / 2;
	/*
	stavi ovo u struct gameState:
	int xoffset, yoffset;

	stavi ovo u main:
	state->yoffset = (WINDOW_H-state->height/4*BLOCK_X)/2;
	state->xoffset = (WINDOW_H -state->width / 4 * BLOCK_X) / 2;
	*/


	//gameState->time++;
	//set the drawing color to gray
	SDL_SetRenderDrawColor(renderer, 99, 99, 99, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	//Draw map background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_Rect bg = { xofs , yofs , gameState->width / 4 * BLOCK_X, gameState->height / 4 * BLOCK_X };
	SDL_RenderFillRect(renderer, &bg);

	//Draw GUI///////////////////////////////////////////////////////////////////
	int xGUI = ((xofs + gameState->width / 4 * BLOCK_X) + WINDOW_W - BLOCK_X) / 2;
	int yGUI = (WINDOW_H - BLOCK_X * 10.5) / 2;
	SDL_Rect tank_count_loc = { xGUI, yGUI, BLOCK_X, BLOCK_X * 5 };
	SDL_Rect tank_count_sloc = { 376, 24, 16, 80 };
	SDL_RenderCopy(renderer, sprites, &tank_count_sloc, &tank_count_loc);

	for (int i = 0; i < 20; i++) {
		SDL_Rect tank_mark_loc = { xGUI + (BLOCK_X / 2)*(i % 2), yGUI + (i / 2)*(BLOCK_X / 2), BLOCK_X / 2, BLOCK_X / 2 };
		SDL_Rect tank_mark_sloc = { 328 + 8 * 1/*(gameState->tanksleft>i)*/, 192, 8, 8 };
		SDL_RenderCopy(renderer, sprites, &tank_mark_sloc, &tank_mark_loc);
	}

	SDL_Rect P1_loc = { xGUI, yGUI + BLOCK_X * 6, BLOCK_X, BLOCK_X };
	SDL_Rect P1_sloc = { 376, 136, 16, 16 };
	SDL_RenderCopy(renderer, sprites, &P1_sloc, &P1_loc);

	struct Tank *p1 = (struct Tank*)gameState->playerTanks->data;

	SDL_Rect num_loc = { xGUI + BLOCK_X / 2, yGUI + BLOCK_X * 6 + BLOCK_X / 2, BLOCK_X / 2, BLOCK_X / 2 };
	SDL_Rect num_sloc = { 289 + 8 * (p1->lives), 200, 8, 8 };
	SDL_RenderCopy(renderer, sprites, &num_sloc, &num_loc);

	if (gameState->playerTanks->next->data) {
		SDL_Rect P2_loc = { xGUI, yGUI + BLOCK_X * 7.5, BLOCK_X, BLOCK_X };
		SDL_Rect P2_sloc = { 376, 160, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &P2_sloc, &P2_loc);

		struct Tank *p2 = (struct Tank*)gameState->playerTanks->data;

		SDL_Rect num2_loc = { xGUI + BLOCK_X / 2, yGUI + BLOCK_X * 7.5 + BLOCK_X / 2, BLOCK_X / 2, BLOCK_X / 2 };
		SDL_Rect num2_sloc = { 289 + 8 * (p2->lives), 200, 8, 8 };
		SDL_RenderCopy(renderer, sprites, &num2_sloc, &num2_loc);
	}

	SDL_Rect lvl_loc = { xGUI, yGUI + BLOCK_X * 9, BLOCK_X, BLOCK_X*1.5 };
	SDL_Rect lvl_sloc = { 376, 184, 16, 24 };
	SDL_RenderCopy(renderer, sprites, &lvl_sloc, &lvl_loc);

	SDL_Rect lvl_dnum_loc = { xGUI, yGUI + BLOCK_X * 10, BLOCK_X / 2, BLOCK_X / 2 };
	SDL_Rect lvl_dnum_sloc = { 289 + 8 * (0/*gameState->Stage/10*/), 200, 8, 8 };
	SDL_RenderCopy(renderer, sprites, &lvl_dnum_sloc, &lvl_dnum_loc);
	SDL_Rect lvl_num_loc = { xGUI + BLOCK_X / 2, yGUI + BLOCK_X * 10, BLOCK_X / 2, BLOCK_X / 2 };
	SDL_Rect lvl_num_sloc = { 289 + 8 * (1/*gameState->Stage%10*/), 200, 8, 8 };
	SDL_RenderCopy(renderer, sprites, &lvl_num_sloc, &lvl_num_loc);
	////////////////////////////END OF GUI////////////////////////////////

	//Draw map
	for (int i = 0; i<gameState->height; i++) {
		for (int j = 0; j<gameState->width; j++) {
			if (!gameState->terrain[i][j] || gameState->terrain[i][j] == 4) continue;
			SDL_Rect location = { xofs + j * BLOCK_X / 4, yofs + i * BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
			SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + (gameState->terrain[i][j] == 3)*((gameState->time % 18) / 3) * 16, (gameState->terrain[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
			SDL_RenderCopy(renderer, sprites, &block, &location);
		}
	}

	//Draw player
	struct listNode *tank_wrapper = gameState->playerTanks;
	char playerNum = 0;
	while (tank_wrapper->data) {
		struct Tank *player = (struct Tank*)tank_wrapper->data;
		SDL_Rect rect = { xofs + player->xPos, yofs + player->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 0 + player->frame * 16 + player->direction * 16 * 2, 0 + (playerNum * 128) + player->upgrade * 16, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		tank_wrapper = tank_wrapper->next;
	}

	//Draw enemys
	struct listNode *etank_wrapper = gameState->enemyTanks;

	while (etank_wrapper->data) {
		struct Tank *enemy = (struct Tank*)etank_wrapper->data;
		if (!(gameState->time % 2)) enemy->frame = (enemy->frame + 1) % 2;
		SDL_Rect rect = { xofs + enemy->xPos, yofs + enemy->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 128 + enemy->frame * 16 + enemy->direction * 16 * 2, 0 + enemy->upgrade * 16, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		etank_wrapper = etank_wrapper->next;
	}

	//Draw player bullets
	struct listNode *pbullet_wrapper = gameState->playerBullets;
	while (pbullet_wrapper->data) {
		struct Bullet *bullet = (struct Bullet*)pbullet_wrapper->data;
		SDL_Rect rect = { xofs + bullet->xPos, yofs + bullet->yPos, BLOCK_X / 4, BLOCK_X / 4 };
		SDL_Rect dest = { 322 + bullet->direction * 5, 102, 5, 4 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		pbullet_wrapper = pbullet_wrapper->next;
	}
	//Draw enemy bullets
	struct listNode *ebullet_wrapper = gameState->enemyBullets;
	while (ebullet_wrapper->data) {
		struct Bullet *bullet = (struct Bullet*)ebullet_wrapper->data;
		SDL_Rect rect = { xofs + bullet->xPos, yofs + bullet->yPos, BLOCK_X / 4, BLOCK_X / 4 };
		SDL_Rect dest = { 322 + bullet->direction * 5, 102, 5, 4 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		playerNum++;
		ebullet_wrapper = ebullet_wrapper->next;
	}


	//Draw trees
	for (int i = 0; i<gameState->height; i++) {
		for (int j = 0; j<gameState->width; j++) {
			if (gameState->terrain[i][j] != 4) continue;
			SDL_Rect location = { xofs + j * BLOCK_X / 4,yofs + i * BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
			SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + ((gameState->time % 120) / 30) * 16, (gameState->terrain[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
			SDL_RenderCopy(renderer, sprites, &block, &location);
		}
	}
	// Powerup moze da postoji samo jedan na ekranu tako da necu ni da vidim da ga stavljate
	// u listu! Dajte ili struct ili samo stavite POW, POWX, i POWY u gameState
	// meni je ovde pretpostavljen struct u kome je num br. powerupa (0 znaci nema ga)
	// a xPos i yPos vec znate
	/*/Draw upgrade
	if (gameState->pow.num) {
	SDL_Rect rect = { xofs+gameState->pow.xPos, yofs+gameState->pow.yPos, BLOCK_X, BLOCK_X };
	SDL_Rect dest = { 256 + (gameState->pow.num-1)*16, 112, 16, 16};
	SDL_RenderCopy(renderer, sprites, &dest, &rect);
	}
	//*/

	/* Pretpostavio sam da eksplozija izgleda ovako i da se nalaze u listi unutar gameState-a

	typedef struct explosion {
	int xPos, yPos;
	char size;
	int time;
	} Explosion;

	*/

	//Draw explosions
	struct listNode *booms = gameState->explosions;
	while (booms->data) {
		Explosion *exp = (struct Bullet*)booms->data;
		SDL_Rect rect = { xofs+exp->xPos - (BLOCK_X/2*(exp->size + 1)), yofs+exp->yPos - (BLOCK_X / 2 * (exp->size + 1)), BLOCK_X*(exp->size + 1), BLOCK_X*(exp->size + 1) };
		SDL_Rect dest = { 256 + exp->size*48 + 16*(exp->time / 2)*(exp->size+1), 128, 16*(exp->size+1), 16*(exp->size + 1) };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		exp->time++;
		if ((exp->time == 6 && exp->size == 0) || (exp->time == 4 && exp->size == 1)) removeNode(booms);
		else booms = booms->next;
	}
	//*/


	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}
