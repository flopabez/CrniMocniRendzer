#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<SDL.h>
#include<SDL_image.h>
#include "mapgen.h"

#define BRICK 1
#define METAL 2
#define WATER 3
#define ICE 5
#define FOREST 4
#define BASE 6
#define EMPTY 0
#define BLOCK_X 48

void generate_tiles(char** map, int map_h, int map_w, int amount, int type);
void set_map_area(char** map, int x, int y, int map_h, int map_w, int type,int big);
void create_base(char** map, int map_h, int map_w);

void reset_map(char** map, int map_h, int map_w)//funkcija za resetovanje matrice na 0
{
	for (int i = 0;i < map_h;i++)
		for (int j = 0;j < map_w;j++)
			map[i][j] = 0;
	create_base(map, map_h, map_w);
	return;
}

void print_map(char** map, int map_h, int map_w)//funkcija za ispis matrice na standardni izlaz
{
	for (int i = 0;i < map_h;i++)
	{
		for (int j = 0;j < map_w;j++)
			printf("%d ", map[i][j]);
		printf("\n");
	}
}

void print_map_file(char** map, int map_h, int map_w,FILE* fmap)//funkcija za ispis matrice u binarnu datoteku
{
	fprintf(fmap, "%d %d ", map_h, map_w);
	for (int i = 0;i < map_h;i++)
		for (int j = 0;j < map_w;j++)
			fprintf(fmap,"%d ", map[i][j]);
	return;
}

void deallocate_map(char** map, int map_h)//funkcija za dealokaciju memorije mape
{
	for (int i = 0;i < map_h;i++)
		free(map[i]);
	free(map);
	return;
}

char** allocate_map(int map_h, int map_w)//funkcija za alokaciju memorije za mapu
{
	char** map;
	map = (char**)malloc(sizeof(char*)*map_h);
	if (map == NULL)
	{
		printf("Error allocating memory for map!\n");
		return NULL;
	}
	for (int i = 0;i < map_h;i++)
	{
		map[i] = (char*)malloc(sizeof(char)*map_w);
		if (map[i] == NULL)
		{
			printf("Error allocating memory for map!\n");
			return NULL;
		}
	}
	return map;
}

void render_map(SDL_Renderer* renderer, SDL_Texture* sprites, char ** map, int map_h, int map_w, int x_brush, int y_brush,int time)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0;i<map_h;i++) {
		for (int j = 0;j<map_w;j++) {
			if (!map[i][j]) continue;//nista za prazan blok
			SDL_Rect location = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
			SDL_Rect block = { 256 + ((j % 4) * 16 / 4)+ (map[i][j] == 3)*((time % 90) / 15) * 16 , (map[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
			SDL_RenderCopy(renderer, sprites, &block, &location);//dodavanje bloka na lokaciju u rendereru
		}
	}

	SDL_Rect rect = { y_brush*BLOCK_X / 4, x_brush*BLOCK_X / 4, BLOCK_X, BLOCK_X };
	SDL_Rect dest = { 272, 208, 16, 16 };
	SDL_RenderCopy(renderer, sprites, &dest, &rect);//dodavanje brusha u renderer

	SDL_RenderPresent(renderer);//iscrtavanje, odnosno renderovanje
}

void create_base(char** map,int map_h, int map_w)
{
	set_map_area(map, map_h - 4, map_w / 2 - 2, map_h, map_w, BASE, 1);
	set_map_area(map, map_h - 6, map_w / 2 - 4, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 6, map_w / 2 - 2, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 6, map_w / 2, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 6, map_w / 2 + 2, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 4, map_w / 2 + 2, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 2, map_w / 2 + 2, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 4, map_w / 2 - 4, map_h, map_w, BRICK, 0);
	set_map_area(map, map_h - 2, map_w / 2 - 4, map_h, map_w, BRICK, 0);
	set_map_area(map, 0,0, map_h, map_w, 7, 1);
	set_map_area(map, 0, map_w  - 4, map_h, map_w, 7, 1);
	set_map_area(map, map_h-10, map_w/2 - 2, map_h, map_w, 11, 1);
}



int build_map(int map_height, int map_width)
{
	int map_h = 4 * map_height;
	int map_w = 4 * map_width;
	SDL_Event event;
	SDL_Window *window;    
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer=NULL;//inicijalizacija rendera i tekstura za crtanje
	int time = 0;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Map Builder",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,BLOCK_X/4*map_w, BLOCK_X/4*map_h,0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("C:\\Users\\Nikola\\Desktop\\sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);
	if (surface == NULL)
	{
		printf("Cannot find sprites.png!\n\n");

		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(1);
		return;
	}
	FILE* fmap = fopen("Maps\\bmap.bin", "wb");
	int x = 0;//pozicije "brusha"
	int y = 0;
	char** map;
	if (fmap == NULL)
	{
		printf("Error creating file!\n");
		return 0;
	}
	map = allocate_map(map_h, map_w);
	if (map == NULL)
		return 0;
	int done = 0;
	int big = 0;
	int draw = 0;
	int type = 0;;
	reset_map(map,map_h,map_w);
	create_base(map, map_h, map_w);
	
	while (done!=1)//petlja za input
	{
		time = (time + 1) % 1000;
		SDL_Delay(15);//delay za normalnu animaciju vode
		render_map(renderer, sprites, map, map_h, map_w, x, y,time);
		while (SDL_PollEvent(&event)) //petlja koja se aktivira tek kada naidje komanda
		{
			switch (event.type) //tip komande, ovde nam treba pritisnuto dugme
			{
			case SDL_WINDOWEVENT_CLOSE:
				if (window)
				{
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) //koje dugme je pritisnuto
				{
				case SDLK_SPACE:if (draw == 0) {
					draw = 1;
					set_map_area(map, x, y, map_h, map_w, type, big);
				}
								else draw = 0;break;
				case SDLK_UP:case SDLK_w://pomeranje gore,dole,levo i desno u matrici mape
					if (x > 0)
						x--;
					if (draw)
						set_map_area(map, x, y, map_h, map_w,type, big);
					break;
				case SDLK_DOWN:case SDLK_s:
					if (x <( map_h - 2-2*big))
						x++;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_LEFT:case SDLK_a:
					if (y > 0)
						y--;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_RIGHT:case SDLK_d:
					if (y <( map_w - 2-2*big))
						y++;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_r:
					reset_map(map, map_h, map_w);
					break;
				case SDLK_1:
					type = BRICK;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_2:
					type = METAL;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_3:
					type = WATER;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_4:
					type = FOREST;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_5:
					type = ICE;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_f:if (big==1) big = 0;
							else {
								if (y > (map_w - 4))
									y = map_w - 4;
								if (x > (map_h - 4))
									x = map_h - 4;
								big = 1;
							}
							break;
				case SDLK_0:
					type = EMPTY;
					if (draw)
						set_map_area(map, x, y, map_h, map_w, type, big);
					break;
				case SDLK_ESCAPE: done = 1;
				default:break;
				}break;
			
			default:break;
			}
		}
	}
	SDL_DestroyWindow(window);//gasenje prozora
	SDL_Quit();
	print_map_file(map, map_h, map_w, fmap);
	deallocate_map(map, map_h);
	fclose(fmap);
	return 1;
}



int random(int max)//random broj >=0 a <max
{
	int res;
	res = (rand() % max);
	return res;
}

int generate_random_map(int map_h, int map_w,int brick_wall_amount,int metal_wall_amount,int water_amount)
{	//amount su vrednosti koje bira korisnik od 0-4 kao nema do mnogo npr
	srand(time(NULL));
	FILE* fmap = fopen("Maps\\random_map.bin", "wb");
	if (fmap == NULL)
	{
		printf("Error creating file!\n");
		return 0;
	}
	int surface = map_h * map_w;//povrsina mape
	int brick, metal, water;
	char** map;
	map = allocate_map(map_h, map_w);
	if (map == NULL)
		return 0;
	reset_map(map, map_h, map_w);
	brick = surface * 0.01*brick_wall_amount;
	water= surface * 0.01*water_amount;
	metal = surface * 0.01*metal_wall_amount;//broj polja odredjenog tipa(bice pomnozeno sa 4)
	generate_tiles(map, map_h, map_w, water, WATER);
	generate_tiles(map, map_h, map_w, brick, BRICK);
	generate_tiles(map, map_h, map_w, metal, METAL);
	print_map_file(map, map_h, map_w, fmap);
	deallocate_map(map, map_h);
	
	return 1;
}

void set_map_area(char** map, int x, int y, int map_h, int map_w, int type,int big)
{//fja za postavljanje datog polja na dati tip, kao i susedna polja, da bi se formirao 4x4 kvadrat
	if (x < map_h - 1 && y < map_w - 1)
	{
		if(map[x][y]<BASE)
		map[x][y] = type;
		if (x + 1 < map_h && map[x+1][y] < BASE)
			map[x + 1][y] = type;
		if (y + 1 < map_w && map[x ][y+1] < BASE)
			map[x][y + 1] = type;
		if ((y + 1 < map_w) && (x + 1 < map_h) && map[x + 1][y+1] < BASE)
			map[x + 1][y + 1] = type;
	}
	if (big == 1 && (x<map_h - 1 && y<map_w - 1))
	{
		set_map_area(map, x + 2, y, map_h, map_w, type, 0);
		set_map_area(map, x + 2, y+2, map_h, map_w, type, 0);
		set_map_area(map, x , y+2, map_h, map_w, type, 0);
	}
	return;
}

void generate_tiles(char** map, int map_h, int map_w, int amount, int type)
{//fja za generisanje zadatog broja polja odredjenog tipa
	int counter = 0;
	int x, y;
	while (counter < amount)
	{
		x = random(map_h);
		y = random(map_w);
		if (map[x][y] == 0)
		{
			set_map_area(map, x, y, map_h, map_w, type,1);
			counter++;
		}
	}
	return;
}

int main()
{
	build_map(14, 14);
	//generate_random_map(52, 52, 3, 1, 2);

}