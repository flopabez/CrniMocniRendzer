#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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

void LoadOptions(Button *buttons) {
	int i=0;
	for (i; i < 2; i++) {
		buttons[i].offset = 0;
		buttons[i].xPos = (16 * 7 * BUTTON_SCALE + 8 - BLOCK_X*5 / 2- 2 * (13 * BUTTON_SCALE))/2 + (i%2)*2*(13*BUTTON_SCALE);
		buttons[i].yPos = BUTTON_Y + 5 - BUTTON_SPACEING + (i / 2) * 4 * BUTTON_H * BUTTON_SCALE + BLOCK_X;
		buttons[i].state = 0;
		buttons[i].click = 0;
	}
	for (i; i < 4; i++) {
		buttons[i].offset = 0;
		buttons[i].xPos = ((WINDOW_W - BLOCK_X / 2 - 16 * 7 * BUTTON_SCALE - 8) - BUTTON_H * BUTTON_SCALE - 8)+20;
		buttons[i].yPos = (WINDOW_H - BUTTON_H * BUTTON_SCALE)/2 + ((i % 2) *2 * 13 * BUTTON_SCALE)-6* BUTTON_SCALE + BLOCK_X;
		buttons[i].state = 0;
		buttons[i].click = 0;
	}
	for (i; i < 6; i++) {
		buttons[i].offset = 0;
		buttons[i].xPos = (WINDOW_W - BLOCK_X / 2 - 16 * 7 * BUTTON_SCALE - 8) + (16 * 7 * BUTTON_SCALE - 38 * BUTTON_SCALE) / 2 + (i % 2) * 2 * (13 * BUTTON_SCALE);
		buttons[i].yPos = BUTTON_Y + 5 - BUTTON_SPACEING + (i / 2) * 4 * BUTTON_H * BUTTON_SCALE-11* BUTTON_SCALE+BLOCK_X;
		buttons[i].state = 0;
		buttons[i].click = 0;
	}
	for (i; i < 7; i++) {
		buttons[i].offset = 23;
		buttons[i].xPos = (16 * 7 * BUTTON_SCALE + 8 - BLOCK_X * 5 / 2 - 2 * (13 * BUTTON_SCALE)) / 2-50;
		buttons[i].yPos = BUTTON_Y + 5 - BUTTON_SPACEING + 2 * 4 * BUTTON_H * BUTTON_SCALE - 11 * BUTTON_SCALE + BLOCK_X;;
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
		if ((i == 1 && enable==0)) {
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
	SDL_Rect logo_sloc = { 228, 256, 170, 25 };
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
	if (!Mix_PlayingMusic()) PlayMenuMusic();
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
	if (ret ==1 || ret == 2 || ret == 6) PlayMenuMusic();
	return ret;
}

void doOptions(OptionsReturnStructure* ret, SDL_Window * window, SDL_Renderer *renderer, SDL_Texture *sprites) {
	Button *buttons = malloc(7 * sizeof(Button));

	LoadOptions(buttons);
	SDL_Event event;
	int done=0;

	
	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);
					window = NULL;
				}
				done = 1;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) done = 1;
				break;
			case SDL_QUIT:
				done = 1;
				break;
			}
		}
		//set the drawing color to gray
		SDL_SetRenderDrawColor(renderer, 65, 106, 204, 0);

		//Clear the screen (to gray)
		SDL_RenderClear(renderer);

		int mouse_x, mouse_y;
		int mouse_press = SDL_GetMouseState(&mouse_x, &mouse_y);

		//Mouseover
		
		for (int i = 0; i < 6; i++) {
			char first = 0;
			
			if ((i == 0 && ret->dif ==0) || (i == 1 && ret->dif == 2) || (i == 2 && ret->height == 15) || (i == 3 && ret->height == 3) || (i == 4 && ret->width == 3) || (i == 5 && ret->width == 15)) {
				buttons[i].state = 2;
				buttons[i].click = 0;
			}
			else if ((mouse_x > buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) + (BUTTON_H - 2 * buttons[i].offset)*BUTTON_SCALE)) && (mouse_y > buttons[i].yPos) && (mouse_y < buttons[i].yPos + BUTTON_H * BUTTON_SCALE)) {
				if (buttons[i].state == 0) OverButtonSound();
				buttons[i].state = 1;
				if (buttons[i].click == 0) first = 1;

				buttons[i].click = mouse_press % 8;
				if ((buttons[i].click !=0) && (first != 0)) {
					switch (i) {
					case 0:
						ret->dif--;
						break;
					case 1:
						ret->dif++;
						break;
					case 2:
						ret->height++;
						break;
					case 3:
						ret->height--;
						break;
					case 4:
						ret->width--;
						break;
					case 5:
						ret->width++;
						break;
					case 6:
						done = 1;
						break;
					}
					ClickButtonSound();
				}
			}
			else {
				buttons[i].click = 0;
				buttons[i].state = 0;
			}
		}

		for (int i = 6; i < 7; i++) {
			buttons[i].click = 0;
			if ((mouse_x > buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) + (BUTTON_W - 2 * buttons[i].offset)*BUTTON_SCALE)) && (mouse_y > buttons[i].yPos) && (mouse_y < buttons[i].yPos + BUTTON_H * BUTTON_SCALE)) {
				if (buttons[i].state == 0) OverButtonSound();
				buttons[i].state = 1;
				buttons[i].click = mouse_press % 8;
				if (buttons[i].click) {
					done = 1;
					ClickButtonSound();
				}
			}
			else {
				buttons[i].state = 0;
			}
		}

		//Draw Logo
		SDL_Rect logo_loc = { (WINDOW_W - 170 * 4) / 2, BLOCK_X, 170 * 4, 25 * 4 };
		SDL_Rect logo_sloc = { 228, 256, 170, 25 };
		SDL_RenderCopy(renderer, sprites, &logo_sloc, &logo_loc);

		SDL_Rect pow_loc = { (WINDOW_W + 170 * 4 + 16) / 2, BLOCK_X, 16 * 3, 32 * 3 };
		SDL_Rect pow_sloc = { 384, 302, 16, 32 };
		SDL_RenderCopy(renderer, sprites, &pow_sloc, &pow_loc);

		//Draw background
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_Rect bg = { BLOCK_X / 2 , BLOCK_X * 4 , WINDOW_W - BLOCK_X, WINDOW_H - BLOCK_X * 4.5 };
		SDL_RenderFillRect(renderer, &bg);

		//Draw buttons
		for (int i = 0; i<6; i++) {
			SDL_Rect location = { buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) - buttons[i].click, buttons[i].yPos - buttons[i].click, (BUTTON_H - 2 * buttons[i].offset)*BUTTON_SCALE + 2 * buttons[i].click, BUTTON_H*BUTTON_SCALE + 2 * buttons[i].click };
			SDL_Rect sprite_loc = { 228+ (i%4/2)*3*BUTTON_H +buttons[i].state*BUTTON_H, 294 + (i%2) * BUTTON_H, BUTTON_H, BUTTON_H };
			SDL_RenderCopy(renderer, sprites, &sprite_loc, &location);
		}

		for (int i = 6; i<7; i++) {
			SDL_Rect vlocation = { buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) - buttons[i].click, buttons[i].yPos - buttons[i].click, (BUTTON_W - 2 * buttons[i].offset)*BUTTON_SCALE + 2 * buttons[i].click, BUTTON_H*BUTTON_SCALE + 2 * buttons[i].click };
			SDL_Rect vsprite_loc = { 0 + buttons[i].offset + buttons[i].state*BUTTON_W, 256 + 5 * BUTTON_H, BUTTON_W - 2 * buttons[i].offset, BUTTON_H };
			//SDL_Rect vsprite_loc = { 23,321,30,13 };
			SDL_RenderCopy(renderer, sprites, &vsprite_loc, &vlocation);
		}

		int difoffset[3] = { 6,0,5 };
		SDL_Rect diflocation = { 98 , BUTTON_Y+5 - 2*BUTTON_SPACEING + BLOCK_X, 48*BUTTON_SCALE, BUTTON_H*BUTTON_SCALE};
		SDL_Rect difsprite_loc = { 228 + ret->dif *48, 281, 48, BUTTON_H };
		SDL_RenderCopy(renderer, sprites, &difsprite_loc, &diflocation);

		char map[15][15] = { 0 };
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				if ((i >= ret->height) || (j >= ret->width)) map[i][j] = 1;
				SDL_Rect square_loc = { (WINDOW_W- BLOCK_X/2-16* 7 * BUTTON_SCALE - 8)+j*7 * BUTTON_SCALE + 20, BUTTON_Y + 20 - 3 * BUTTON_SPACEING + (16 - i) *7 * BUTTON_SCALE + BLOCK_X,7 * BUTTON_SCALE,7*BUTTON_SCALE};
				SDL_Rect square_sloc = { 272 + map[i][j] * 7, 0, 7, 7 };
				SDL_RenderCopy(renderer, sprites, &square_sloc, &square_loc);
			}
		}

		TTF_Font *font=NULL;
		if (font==NULL) {
			font = TTF_OpenFont("slkscr.ttf", 20);
			if (!font) {
				printf("Error: %s\n",TTF_GetError());
			}
		}

		SDL_Color font_color = { 255,255,255,0 };
		SDL_Rect font_rect = { 500, 500, 100, 20 };
		SDL_Surface *textSurface = TTF_RenderText_Solid(font, "IT'S ALIVE!", font_color);
		SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
		SDL_RenderCopy(renderer, text, NULL, &font_rect);

		//We are done drawing, "present" or show to the screen what we've drawn
		SDL_RenderPresent(renderer);
		SDL_Delay(1. / FPS * 1000);
	}
	
}
/*

*/