#ifndef scores_h
#define scores_h

#include<SDL.h>
#include<SDL_ttf.h>

struct score
{
	int score;
	char name[30];
	struct score* next;
};
int encrypt();
int decrypt();
char* read_key();
void generate_new_key();
void insert_score(struct score** list, char* name, int score, int pos);
void update_score(int score, SDL_Window* window, SDL_Renderer* renderer);
void show_score();
void free_list(struct score** list,int size);
struct score** read_score();
void highscore(SDL_Window* window, SDL_Renderer* renderer);
char* string_input(SDL_Window* window, SDL_Renderer* renderer, char* string);
int loop(char* input, SDL_Renderer* renderer, SDL_Texture* text, TTF_Font* font, char* string);

#endif
