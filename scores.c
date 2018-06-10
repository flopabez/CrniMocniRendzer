#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<SDL.h>
#include "scores.h"
#include <SDL_ttf.h>
#include<SDL_image.h>
//122

char* string_input() {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	//SDL_Texture *texture = NULL;
	SDL_Texture	*text = NULL;
	TTF_Font* font = NULL;
	char* input;
	input = malloc(sizeof(char) * 20);
	input[0] = '\0';
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}

	// Initialize SDL_ttf
	if (TTF_Init() < 0) {
		return 0;
	}

	window = SDL_CreateWindow("Unesite ime", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 380,60, 0);
	if (!window) {

		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		return 0;
	}

	//texture = SDL_CreateTextureFromSurface(renderer, buffer);

	// Load font
	font = TTF_OpenFont("resursi\\RosesareFF0000.ttf", 24);
	if (!font) {
		return 0;
	}

	// Start sending SDL_TextInput events
	SDL_StartTextInput();
	loop(input, renderer, text, font);
		// wait before processing the next frame
		


	SDL_StopTextInput();

	TTF_CloseFont(font);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	return input;
}

int loop(char* input,SDL_Renderer* renderer,  SDL_Texture* text,TTF_Font* font) {

	char* keys = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	SDL_Rect dest;

	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 200);
	
	int run = 1;
	while (run) {
		// Event loop
		while (SDL_PollEvent(&e) != 0) {
			SDL_RenderClear(renderer);
			switch (e.type)
			{
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE) && strlen(input) > 0)
					run = 0;
				if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(input) > 0)
				{
					input[strlen(input) - 1] = '\0';
				}
				break;
			case SDL_TEXTINPUT:
				if(strlen(input)<20)
				strcat(input, e.text.text);
				//input += e.text.text;
				break;
			}
		}


		// Render texture
		//SDL_RenderCopy(renderer, texture, NULL, NULL);

		SDL_Color foreground = { 0, 0, 0 };

		if (strlen(input) > 0) {
			SDL_Surface* text_surf = TTF_RenderText_Solid(font, input, foreground);
			text = SDL_CreateTextureFromSurface(renderer, text_surf);

			dest.x = 10;// - (text_surf->w / 2.0f);
			dest.y = 20;
			dest.w = text_surf->w;
			dest.h = text_surf->h;
			SDL_RenderCopy(renderer, text, NULL, &dest);

			SDL_DestroyTexture(text);
			SDL_FreeSurface(text_surf);
		}
		
		// Update window
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
	return 1;
}


void generate_new_key()
{
	FILE* fkey = fopen("resursi\\key.bin", "rb");
	char key[10000];
	for (int i = 0;i < 9999;i++)
		key[i] = 1+ random(20);
	key[9999] = '\0';
	fprintf(fkey, "%s", key);
	fclose(fkey);
}

char* read_key()
{
	FILE* key = fopen("resursi\\key.bin", "rb");
	char* k=(char*)malloc(sizeof(char)*10000);
	fscanf(key, "%s", k);
	fclose(key);
	return k;
}
int encrypt()
{
	FILE *fp1 = fopen("resursi\\highscores.txt", "r");
	if (fp1 == NULL)
	{
		return 0;
	}
	FILE *fp2 = fopen("resursi\\pp2.bin", "wb");
	if (fp2 == NULL)
	{
		return 0;
	}
	char ch;
	while (1)
	{
		ch = fgetc(fp1);
		if (ch == EOF)
		{
			break;
		}
		else
		{
			ch = ch-37;
			fputc(ch, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 1;
}

int decrypt()
{
	FILE* fp1 = fopen("resursi\\pp2.bin", "rb");
	if (fp1 == NULL)
	{
		return 0;
	}
	FILE* fp2 = fopen("resursi\\highscores.txt", "w");
	if (fp2 == NULL)
	{
		return 0;
	}
	char ch;
	while (1)
	{
		ch = fgetc(fp1);
		if (ch == EOF)
		{
			break;
		}
		else
		{
			ch = ch +37;
			fputc(ch, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 1;
}



void insert_score(struct score** list, char* name,int score,int pos)
{
	struct score* temp;
	temp = (struct score*)malloc(sizeof(struct score));
	if (temp == NULL)
		return;
	strcpy(temp->name, name);
	temp->score = score;
	temp->next = NULL;
	list[pos] = temp;
}

void free_list(struct score** list, int size)
{
	int pos = 0;
	for(int pos=0;pos<size;pos++)
	{
		free(list[pos]);
	}
}

void update_score(int score)
{
	//char* key = read_key();
	char* new_name;
	char number[20];
	char name[30];
	struct score* list[20];
	int val;
	new_name = string_input();
	int not_inserted = 1;
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	int i = 0;
	char c;
	while( (c=fscanf(fscore,"%s %s ",name ,number))!=EOF && i<19)
	{
		val = atoi(number);
			if ( score < val)
			{
				insert_score(list, name,val, i);i++;
			}
			else { insert_score(list, new_name,score, i);i++;insert_score(list, name,val, i);i++; not_inserted = 0;score = -1; }
	}
	if (not_inserted)
	{
		insert_score(list, new_name,score, i);i++;
	}

	fclose(fscore);
	FILE * new_fscore = fopen("resursi\\highscores.txt", "w");
	for(int j=0;j<i;j++)
		fprintf(new_fscore, "%s %d ", list[j]->name, list[j]->score);
	free_list(list,i);
	fclose(new_fscore);
	encrypt();
}


void show_score()
{
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	char c;
	char name[30], number[20];
	int i = 1;
	while ((c = fscanf(fscore, "%s %s ", name, number)) != EOF)
	{
		printf("%d-%s:%s\n", i, name, number);
		i++;
	}
	fclose(fscore);
	//system("pause");
}

struct score** read_score()
{
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	struct score* list[20];
	char c;
	char name[30], number[20];
	int i = 0;
	while ((c = fscanf(fscore, "%s %s ", name, number)) != EOF)
	{
		insert_score(&list, name, number,i);
		i++;
	}
	fclose(fscore);
	return list;
}