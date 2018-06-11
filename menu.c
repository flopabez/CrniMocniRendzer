#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "menu.h"
#include "strukture.h"
#include "sound.h"
#include "scores.h"

#define MIN_SIZE 7

void LoadMenu(Button *buttons) {
	int offset_array[BUTTON_NUM] = {5, 6, 9, 0, 2, 23};
	for (int i = 0; i < BUTTON_NUM; i++) {
		buttons[i].offset = offset_array[i];
		buttons[i].xPos = BUTTON_X + buttons[i].offset;
		buttons[i].yPos = BUTTON_Y + i * BUTTON_SPACEING;
		buttons[i].state = 0;
		buttons[i].click = 0;
	}

	buttons[6].offset = 0;
	buttons[6].xPos = (WINDOW_W + 170 * 4 + 16) / 2;
	buttons[6].yPos = BLOCK_X;
	buttons[6].state = 0;
	buttons[6].click = 0;
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

int MainMenu(SDL_Renderer *renderer, SDL_Texture *sprites, Button *buttons, char enable, char *cheats) {

	int ret = 0;
	static int time = 0;
	time++;
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 65, 106, 204, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

	int mouse_x, mouse_y;
	int mouse_press = SDL_GetMouseState(&mouse_x, &mouse_y);

	//Mouseover
	for (int i = 0; i < BUTTON_NUM; i++) {
		char first = 0;
		
		if ((i == 1 && enable==0)) {
			buttons[i].state = 2;
			buttons[i].click = 0;
		} else if ((mouse_x > buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[i].xPos + buttons[i].offset*(BUTTON_SCALE - 1) + (BUTTON_W - 2 * buttons[i].offset)*BUTTON_SCALE)) && (mouse_y > buttons[i].yPos) && (mouse_y < buttons[i].yPos + BUTTON_H * BUTTON_SCALE)) {
			if (buttons[i].state == 0 && i!=6) OverButtonSound();
			buttons[i].state = 1;
			if (buttons[i].click == 0) first = 1;
			buttons[i].click = mouse_press%8;
			if (buttons[i].click && first) {
				if (i == 6) {
					*cheats = (*cheats+1)%2;
					if (*cheats) SecretSound();
					else ClickButtonSound();
				} else {
					ret = i + 1;
					ClickButtonSound();
				}
			}
		}
		else {
			buttons[i].state = 0;
			buttons[i].click = 0;
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
	for (int i = 0; i<BUTTON_NUM-1; i++) {
		SDL_Rect location = { buttons[i].xPos+ buttons[i].offset*(BUTTON_SCALE-1)- buttons[i].click, buttons[i].yPos - buttons[i].click, (BUTTON_W-2* buttons[i].offset)*BUTTON_SCALE + 2* buttons[i].click, BUTTON_H*BUTTON_SCALE + 2 * buttons[i].click };
		SDL_Rect sprite_loc = { 0 + buttons[i].offset + buttons[i].state*BUTTON_W, 256 + i* BUTTON_H, BUTTON_W - 2 * buttons[i].offset, BUTTON_H };
		SDL_RenderCopy(renderer, sprites, &sprite_loc, &location);
	}

	//We are done drawing, "present" or show to the screen what we've drawn
	SDL_RenderPresent(renderer);

	return ret;
}

int doMenu(SDL_Window * window, SDL_Renderer *renderer, SDL_Texture *sprites, char enable, char *cheats) {
	Button *buttons = malloc(7 * sizeof(Button));
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
		ret = MainMenu(renderer, sprites, buttons, enable, cheats);
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
	ret->height = ret->height/4;
	ret->width = ret->width / 4;

	
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
			
			if ((i == 0 && ret->dif <=0) || (i == 1 && ret->dif >= 2) || (i == 2 && ret->height >= 15) || (i == 3 && ret->height <= MIN_SIZE) || (i == 4 && ret->width <= MIN_SIZE) || (i == 5 && ret->width >= 15)) {
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

		static TTF_Font *font;
		if (!font) {
			font = TTF_OpenFont("./resursi/RosesareFF0000.ttf", 40);
			if (!font) {
				printf("Error: %s\n",TTF_GetError());
			}
		}
		if (font) {
			char* string1[20];
			sprintf(string1, "Map Size %d x %d",ret->width,ret->height);
			SDL_Color font_color = { 0,0,0,0 };
			SDL_Rect font_rect = { (WINDOW_W - BLOCK_X / 2 - 16 * 7 * BUTTON_SCALE - 8)+82 + (ret->height<10) * 10 + (ret->width<10)*10,  BUTTON_Y + 5 - 2 * BUTTON_SPACEING + 6, 20*(13+(ret->height>9)+ (ret->width>9)), 30 };
			SDL_Surface *textSurface = TTF_RenderText_Solid(font, string1, font_color);
			SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
			SDL_FreeSurface(textSurface);
			textSurface = NULL;
			SDL_RenderCopy(renderer, text, NULL, &font_rect);

			SDL_Rect font_rect2 = { 98 , BUTTON_Y + 5 - 2 * BUTTON_SPACEING+6 , 48 * BUTTON_SCALE, 30 };
			SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, "Bot Difficulty", font_color);
			SDL_Texture *text2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
			SDL_FreeSurface(textSurface2);
			textSurface2 = NULL;
			SDL_RenderCopy(renderer, text2, NULL, &font_rect2);
		}

		//We are done drawing, "present" or show to the screen what we've drawn
		SDL_RenderPresent(renderer);

		SDL_Delay(1. / FPS * 1000);
	}
	ret->height = ret->height * 4;
	ret->width = ret->width * 4;
}

void Highscore(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture *sprites)
{
	Button *buttons = malloc(sizeof(Button));

	buttons[0].offset = 23;
	buttons[0].xPos = BUTTON_X + buttons[0].offset;
	buttons[0].yPos = BUTTON_Y + 6 * BUTTON_SPACEING-14;
	buttons[0].state = 0;
	buttons[0].click = 0;

	SDL_Event event;
	int done = 0;

	struct score** list;
	list = read_score();
	
	if (TTF_Init() < 0) {
		return 0;
	}

	//SDL_Renderer* renderer = NULL;
	SDL_Texture	*text = NULL;
	TTF_Font* font = NULL;
	font = TTF_OpenFont("resursi\\RosesareFF0000.ttf", 40);
	SDL_Color foreground = { 255,255, 255 };
	SDL_Rect dest;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Surface* text_surf = TTF_RenderText_Solid(font, "Highscores:", foreground);
	text = SDL_CreateTextureFromSurface(renderer, text_surf);
	dest.x = 10;// - (text_surf->w / 2.0f);
	dest.y = 10;
	dest.w = text_surf->w;
	dest.h = text_surf->h;
	SDL_RenderCopy(renderer, text, NULL, &dest);



	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 65, 106, 204, 0);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);

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

	int i = 0;
	for (i = 0; i < 10 && list[i] != NULL; i++)
	{
		char* string;
		string = malloc(sizeof(char) * 30);
		//strcpy(string, list[i]->name);
		//strcat(string, itoa(list[i]->score));
		SDL_Color foreground = { 0,0,0};
		if (i == 0) {
			foreground.r = 255;
			foreground.g = 215;
			foreground.b = 0;
		} else if (i == 1) {
			foreground.r = 192;
			foreground.g = 192;
			foreground.b = 192;
		} else if (i == 2) {
			foreground.r = 205;
			foreground.g = 127;
			foreground.b = 50;
		}
		snprintf(string, 30, "%d.%s:%d", i + 1, list[i]->name, list[i]->score);
		SDL_Surface* text_surf = TTF_RenderText_Solid(font, string, foreground);
		text = SDL_CreateTextureFromSurface(renderer, text_surf);
		dest.x = BLOCK_X / 2+10;// - (text_surf->w / 2.0f); 
		dest.y = BLOCK_X * 4 + 10 + 50 * (i);
		dest.w = text_surf->w;
		dest.h = text_surf->h;
		SDL_RenderCopy(renderer, text, NULL, &dest);
		SDL_FreeSurface(text_surf);
		SDL_DestroyTexture(text);
	}

	// Update window
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

		int mouse_x, mouse_y;
		int mouse_press = SDL_GetMouseState(&mouse_x, &mouse_y);

		//Mouseover

		buttons[0].click = 0;
		if ((mouse_x > buttons[0].xPos + buttons[0].offset*(BUTTON_SCALE - 1)) && (mouse_x < (buttons[0].xPos + buttons[0].offset*(BUTTON_SCALE - 1) + (BUTTON_W - 2 * buttons[0].offset)*BUTTON_SCALE)) && (mouse_y > buttons[0].yPos) && (mouse_y < buttons[0].yPos + BUTTON_H * BUTTON_SCALE)) {
			if (buttons[0].state == 0) OverButtonSound();
			buttons[0].state = 1;
			buttons[0].click = mouse_press % 8;
			if (buttons[0].click) {
				done = 1;
				ClickButtonSound();
			}
		}
		else {
			buttons[0].state = 0;
		}

		//Draw buttons
		
		SDL_Rect location = { buttons[0].xPos + buttons[0].offset*(BUTTON_SCALE - 1) - buttons[0].click, buttons[0].yPos - buttons[0].click, (BUTTON_W - 2 * buttons[0].offset)*BUTTON_SCALE + 2 * buttons[0].click, BUTTON_H*BUTTON_SCALE + 2 * buttons[0].click };
		SDL_Rect sprite_loc = { 0 + buttons[0].offset + buttons[0].state*BUTTON_W, 256 + 5 * BUTTON_H, BUTTON_W - 2 * buttons[0].offset, BUTTON_H };
		SDL_RenderCopy(renderer, sprites, &sprite_loc, &location);
		

		SDL_RenderPresent(renderer);
		SDL_Delay(1. / FPS * 1000);
	}

	// Update window
	TTF_CloseFont(font);
	free_list(list, i);
	//SDL_DestroyRenderer(renderer);
}