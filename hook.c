
#include "strukture.h"
#include < SDL.h >
#include < SDL_image.h >

char processEvents(SDL_Window * window, struct movementWrapper * wrap, struct gameState* state) {

	SDL_Event event;
	char done = 0;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE:
			if (window) {
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {

			case SDLK_1:
				done = 'y';
				break;
			case SDLK_2:
				done = 'u';
				break;
			case SDLK_3:
				done = 'i';
				break;
			case SDLK_4:
				done = 'o';
				break;
			case SDLK_5:
				done = 'p';
				break;
			case SDLK_6:
				done = '[';
				break;
			case SDLK_7:
				done = ']';
				break;
			case SDLK_8:
				done = 'h';
				break;
			case SDLK_SPACE:
				fireBullet(state, wrap->tenkic);
				done = ' ';
				break;
			case SDLK_ESCAPE:
				done = 2;
				break;
			case SDLK_w:
			case SDLK_UP:
				wrap->up = 1;
				wrap->tenkic->direction = 0;
				done = 'w';
				break;
			case SDLK_a:
			case SDLK_LEFT:
				wrap->left = 1;
				wrap->tenkic->direction = 1;
				done = 'a';
				break;
			case SDLK_s:
			case SDLK_DOWN:
				wrap->down = 1;
				wrap->tenkic->direction = 2;
				done = 's';
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				wrap->right = 1;
				wrap->tenkic->direction = 3;
				done = 'd';
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_w:
			case SDLK_UP:
				wrap->up = 0;
				if (wrap->left) wrap->tenkic->direction = 1;
				if (wrap->down) wrap->tenkic->direction = 2;
				if (wrap->right) wrap->tenkic->direction = 3;
				break;
			case SDLK_a:
			case SDLK_LEFT:
				wrap->left = 0;
				if (wrap->up) wrap->tenkic->direction = 0;
				if (wrap->down) wrap->tenkic->direction = 2;
				if (wrap->right) wrap->tenkic->direction = 3;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				wrap->down = 0;
				if (wrap->left) wrap->tenkic->direction = 1;
				if (wrap->up) wrap->tenkic->direction = 0;
				if (wrap->right) wrap->tenkic->direction = 3;
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				wrap->right = 0;
				if (wrap->left) wrap->tenkic->direction = 1;
				if (wrap->down) wrap->tenkic->direction = 2;
				if (wrap->up) wrap->tenkic->direction = 0;
				break;
			}
			break;
		case SDL_QUIT:
			done = 1;
			break;
		}

	}

	return done;
}
