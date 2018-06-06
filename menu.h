#pragma once

#define BUTTON_NUM 6
#define BUTTON_SCALE 4
#define BUTTON_W 76
#define BUTTON_H 13
#define BUTTON_X ((48 / 4 * 52) - BUTTON_W*BUTTON_SCALE) / 2
#define BUTTON_Y 64*3
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