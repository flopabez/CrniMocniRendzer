#pragma once

#include "graphics.h"

#define BUTTON_NUM 6
#define BUTTON_SCALE 4
#define BUTTON_W 76
#define BUTTON_H 13
#define BUTTON_X (WINDOW_W - BUTTON_W*BUTTON_SCALE) / 2
#define BUTTON_Y WINDOW_H - (BUTTON_NUM +1) * BUTTON_SPACEING
#define BUTTON_SPACEING (BUTTON_H+3)*BUTTON_SCALE

#define BLOCK_X 48

typedef struct button {
	int xPos, yPos;
	int offset;
	char state, click;
	// States:
	// 0 - Idle
	// 1 - MouseOver
	// 2 - Disabled
} Button;

void LoadMenu(Button *buttons);

int MainMenu(SDL_Renderer *renderer, SDL_Texture *sprites, Button *buttons);

int doMenu(SDL_Window * window, SDL_Renderer *renderer, SDL_Texture *sprites, Button *buttons);
