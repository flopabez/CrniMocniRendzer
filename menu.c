#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "strukture.h"

void LoadMenu(Button *buttons) {
	int offset_array[BUTTON_NUM] = {5, 6, 9, 0, 2, 23};
	for (int i = 0; i < BUTTON_NUM; i++) {
		buttons[i].offset = offset_array[i];
		buttons[i].xPos = BUTTON_X + buttons[i].offset;
		buttons[i].yPos = BUTTON_Y + i * BUTTON_SPACEING;
		buttons[i].state = 0;
		buttons[i].click = 0;
	}
}

int MainMenu(SDL_Renderer *renderer, SDL_Texture *sprites, Button *buttons) {
	
	int ret = 0;
	static int time = 0;
	time++;
	//set the drawing color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);


	int mouse_x, mouse_y;
	int mouse_press = SDL_GetMouseState(&mouse_x, &mouse_y);

	//Mouseover
	for (int i = 0; i < BUTTON_NUM; i++) {
		buttons[i].click = 0;
		if (i == 1) {
			buttons[i].state = 2;
		} else if ((mouse_x > buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) + (BUTTON_W - 2 * buttons[i].offset)*BUTTON_SCALE)) && (mouse_y > buttons[i].yPos) && (mouse_y < buttons[i].yPos + BUTTON_H * BUTTON_SCALE)) {
			buttons[i].state = 1;
			buttons[i].click = mouse_press%8;
			if (buttons[i].click) ret = i+1;
		}
		else {
			buttons[i].state = 0;
		}
	}

	//Draw buttons
	for (int i = 0; i<BUTTON_NUM; i++) {
		SDL_Rect location = { buttons[i].xPos+ buttons[i].offset*(BUTTON_SCALE-1)- buttons[i].click, buttons[i].yPos - buttons[i].click, (BUTTON_W-2* buttons[i].offset)*BUTTON_SCALE + 2* buttons[i].click, BUTTON_H*BUTTON_SCALE + 2 * buttons[i].click };
		SDL_Rect sprite_loc = { 0 + buttons[i].offset + buttons[i].state*BUTTON_W, 256 + i* BUTTON_H, BUTTON_W - 2 * buttons[i].offset, BUTTON_H };
		SDL_RenderCopy(renderer, sprites, &sprite_loc, &location);
	}

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);

	return ret;
}
