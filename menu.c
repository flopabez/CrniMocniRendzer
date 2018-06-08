#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "strukture.h"
#include "sound.h"

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

int MainMenu(SDL_Renderer *renderer, SDL_Texture *sprites, Button *buttons, char enable) {

	int ret = 0;
	static int time = 0;
	time++;
	//set the drawing color to gray
	SDL_SetRenderDrawColor(renderer, 65, 106, 204, 0);

	//Clear the screen (to gray)
	SDL_RenderClear(renderer);

	int mouse_x, mouse_y;
	int mouse_press = SDL_GetMouseState(&mouse_x, &mouse_y);

	//Mouseover
	for (int i = 0; i < BUTTON_NUM; i++) {
		buttons[i].click = 0;
		if (i == 1 && enable==0) {
			buttons[i].state = 2;
		} else if ((mouse_x > buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) + (BUTTON_W - 2 * buttons[i].offset)*BUTTON_SCALE)) && (mouse_y > buttons[i].yPos) && (mouse_y < buttons[i].yPos + BUTTON_H * BUTTON_SCALE)) {
			if (buttons[i].state == 0) OverButtonSound();
			buttons[i].state = 1;
			buttons[i].click = mouse_press%8;
			if (buttons[i].click) {
				ret = i + 1;
				ClickButtonSound();
			}
		}
		else {
			buttons[i].state = 0;
		}
	}

	//Draw Logo
	SDL_Rect logo_loc = { (WINDOW_W- 170*4)/2, BLOCK_X, 170*4, 25 *4 };
	SDL_Rect logo_sloc = { 230, 264, 170, 25 };
	SDL_RenderCopy(renderer, sprites, &logo_sloc, &logo_loc);

	SDL_Rect pow_loc = { (WINDOW_W + 170*4 +16)/2, BLOCK_X, 16 * 3, 32 * 3 };
	SDL_Rect pow_sloc = { 384, 302, 16, 32 };
	SDL_RenderCopy(renderer, sprites, &pow_sloc, &pow_loc);

	//Draw background
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_Rect bg = { BLOCK_X/2 , BLOCK_X*4 , WINDOW_W-BLOCK_X, WINDOW_H- BLOCK_X *4.5 };
	SDL_RenderFillRect(renderer, &bg);

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

int doMenu(SDL_Window * window, SDL_Renderer *renderer, SDL_Texture *sprites, char enable) {
	Button *buttons = malloc(6 * sizeof(Button));
	LoadMenu(buttons);
	SDL_Event event;
	int ret = 0;
	PlayMenuMusic();
	while (ret == 0) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);
					window = NULL;
					ret = 6;
				}
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) ret = 6;
				break;
			case SDL_QUIT:
				ret = 6;
				break;
			}
		}
		ret = MainMenu(renderer, sprites, buttons, enable);
		SDL_Delay(1. / FPS * 1000);
	}
	PlayMenuMusic();
	return ret;
}
