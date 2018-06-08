#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "strukture.h"

void PlayIt() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) printf("Error: %s\n", Mix_GetError);
	static Mix_Music *music=NULL;
	static Mix_Chunk *pause =NULL;
	static Mix_Chunk *unpause = NULL;
	if (!music) {
		music = Mix_LoadMUS("../resursi/Spider_Dance.mp3");
		if (!music) {
			printf("Can't find file 'Spider_Dance.mp3'!\n");
			return;
		}
	}
	if (!pause) {
		pause = Mix_LoadWAV("../resursi/pause.wav");
		if (!pause) {
			printf("Can't find file 'pause.wav'!\n");
			return;
		}
	}
	if (!unpause) {
		unpause = Mix_LoadWAV("../resursi/unpause.wav");
		if (!unpause) {
			printf("Can't find file 'unpause.wav'!\n");
			return;
		}
	}
	if (!Mix_PlayingMusic()) Mix_PlayMusic(music, -1);
	else if (Mix_PausedMusic()) {
		Mix_PlayChannel(-1, unpause,0);
		Mix_ResumeMusic();
	}
	else {
		Mix_PlayChannel(-1, pause, 0);
		Mix_PauseMusic();
	}
}

void Bang() {
	static Mix_Chunk *bang = NULL;
	if (!bang) {
		bang = Mix_LoadWAV("../resursi/bang.wav");
		if (!bang) {
			printf("Can't find file 'bang.wav'!\n");
			return;
		}
	}
	Mix_PlayChannel(-1, bang, 0);
	printf("BANG!\n");
}

void Boom() {
	static Mix_Chunk *boom = NULL;
	if (!boom) {
		boom = Mix_LoadWAV("../resursi/big_boom.wav");
		if (!boom) {
			printf("Can't find file 'big_boom.wav'!\n");
			return;
		}
	}
	Mix_PlayChannel(-1, boom, 0);
	printf("BOOM!\n");
}
/*void MakeSound() {

}*/

/*

| SDL_INIT_AUDIO

///// AUDIO ////////////////

if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) printf("Error: %s\n", Mix_GetError);

////////////////////////////

SDL_DestroyWindow(window);
SDL_DestroyTexture(sprites);
SDL_DestroyRenderer(renderer);

window = sprites = renderer = NULL;

Mix_Quit();
SDL_Quit();
*/