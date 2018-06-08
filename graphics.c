#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "graphics.h"

//#include <SDL_ttf.h>
//#include "sound.h"

void doRender(struct gameState *gameState, SDL_Renderer *renderer, SDL_Texture *sprites)
{
	
	if (gameState->time == 1) PlayIt();
	int xofs = (WINDOW_H - gameState->width / 4 * BLOCK_X) / 2;
	int yofs = (WINDOW_H - gameState->height / 4 * BLOCK_X) / 2;

	//gameState->time++;
	//set the drawing color to gray
	SDL_SetRenderDrawColor(renderer, 99, 99, 99, 0);

	//Clear the screen (to gray)
	SDL_RenderClear(renderer);

	int temp = gameState->score, letter_num=0;
	while (temp) {
		temp = temp / 10;
		letter_num++;
	}

	/*/Write Score
	static TTF_Font *font;
	if (!font) {
		font = TTF_OpenFont("../resursi/RosesareFF0000.ttf", 20);
		if (!font) {
			printf("Can't find font 'RosesareFF0000.ttf\n'");
		}
	}
	char score[20];
	sprintf(score, "Score: %d", gameState->score);

	SDL_Color font_color = { 255,255,255,0 };
	int font_h = (yofs * 9 / 10 < BLOCK_X / 2) ? yofs * 9 / 10 : BLOCK_X / 2;
	int font_w = (((yofs * 9 / 10) * 5 / 7 < BLOCK_X / 2  ) ? (yofs * 9 / 10) * 5 / 7 : BLOCK_X / 2)*(8 + letter_num);
	SDL_Rect font_rect = { xofs, (yofs- font_h)/2, font_w, font_h };
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, score, font_color);
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	textSurface = NULL;
	SDL_RenderCopy(renderer, text, NULL, &font_rect);
	//*/

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
		SDL_Rect tank_mark_sloc = { 328 + 8 * (gameState->tanksleft>i), 192, 8, 8 };
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
		if (player->shield) {
			SDL_Rect shield_sprite = { 256 + (gameState->time%2)*16, 144, 16, 16 };
			SDL_RenderCopy(renderer, sprites, &shield_sprite, &rect);
		}

		playerNum++;
		tank_wrapper = tank_wrapper->next;
	}

	//Draw enemys
	struct listNode *etank_wrapper = gameState->enemyTanks;

	while (etank_wrapper->data) {
		struct Tank *enemy = (struct Tank*)etank_wrapper->data;
		if (!(gameState->time % 2)) enemy->frame = (enemy->frame + 1) % 2;
		SDL_Rect rect = { xofs + enemy->xPos, yofs + enemy->yPos, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 128 + enemy->frame * 16 + enemy->direction * 16 * 2 -128*enemy->pickup* (gameState->time % 2), 64 + (enemy->bot-1) * 16 + (enemy->hitPoints>1) * 128 - 128* (enemy->hitPoints>1)*(enemy->hitPoints<4)*!(gameState->time%enemy->hitPoints) + 128*(enemy->bot!=4)*enemy->pickup* (gameState->time % 2), 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		if (enemy->shield) {
			SDL_Rect shield_sprite = { 256 * (gameState->time % 2) * 16, 144, 16, 16 };
			SDL_RenderCopy(renderer, sprites, &shield_sprite, &rect);
		}
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

	//Draw pickup
	if (gameState->pickup) {
	SDL_Rect rect = { xofs+ gameState->pickup->xPos, yofs+ gameState->pickup->yPos, BLOCK_X, BLOCK_X };
	SDL_Rect dest = { 256 + (gameState->pickup->type)*16, 112, 16, 16};
	SDL_RenderCopy(renderer, sprites, &dest, &rect);
	}
	//*/

	//Draw explosions
	struct listNode *booms = gameState->explosions;
	while (booms->data) {
		Explosion *exp = (struct Bullet*)booms->data;
		//if (exp->time == 0 && exp->size == 1) BoomSound();
		SDL_Rect rect = { xofs+exp->xPos - (BLOCK_X/2*(exp->size + 1)), yofs+exp->yPos - (BLOCK_X / 2 * (exp->size + 1)), BLOCK_X*(exp->size + 1), BLOCK_X*(exp->size + 1) };
		SDL_Rect dest = { 256 + exp->size*48 + 16*(exp->time / 2)*(exp->size+1), 128, 16*(exp->size+1), 16*(exp->size + 1) };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);
		exp->time++;
		if ((exp->time == 6 && exp->size == 0) || (exp->time == 4 && exp->size == 1)) removeNode(booms);
		else booms = booms->next;
	}
	//*/

	//SDL_DestroyTexture(text);
	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);
}
