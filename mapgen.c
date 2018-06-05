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

void generate_tiles(char** map, int map_h, int map_w, int x,int y, int type);
void generate_path(char** map, int map_h, int map_w);
void set_map_area(char** map, int x, int y, int map_h, int map_w, int type,int big);
void create_base(char** map, int map_h, int map_w);
void clear_base(char **map,int map_h,int map_w);

char** read_map(int* map_h, int* map_w, char* file_name)//fja za ucitavanje mape iz fajla u matricu
{
	char** map=NULL;
	char* extension = ".bin";
	char* folder = "Maps\\";
	char* filename[50];
	snprintf(filename, sizeof(filename), "%s%s%s", folder, file_name, extension);
	FILE* fmap = fopen(filename, "rb");
	if (fmap == NULL)
		return 0;
	fscanf(fmap, "%d", map_h);
	fscanf(fmap, "%d", map_w);
	map = allocate_map(*map_h, *map_w);
	for (int i = 0;i < (*map_h);i++)
		for (int j = 0;j <(*map_w);j++)
			fscanf(fmap, "%d",(int)&map[i][j]);
	fclose(fmap);
	return map;
}

void reset_map(char** map, int map_h, int map_w)//funkcija za resetovanje matrice na 0
{
	for (int i = 0;i < map_h;i++)
		for (int j = 0;j < map_w;j++)
			map[i][j] = 0;

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
	return;
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

void render_map(SDL_Renderer* renderer, SDL_Texture* sprites, char ** map, int map_h, int map_w, int x_brush, int y_brush,int time,int big)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0;i<map_h;i++) {
		for (int j = 0;j<map_w;j++) {
			if (!map[i][j]) continue;//nista za prazan blok
			if (map[i][j] == 6)
			{
				if ((i == map_h - 4) && (map[i][j + 3] == 6))
				{
					SDL_Rect brect = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X, BLOCK_X };
					SDL_Rect bdest = { 256, 80, 16, 16 };
					SDL_RenderCopy(renderer, sprites, &bdest, &brect);//dodavanje brusha u renderer
				}
			}
			else
				if (map[i][j]==11)
				{
					if ((i == map_h - 10) && (map[i][j + 3] == 11))
					{
						SDL_Rect brect = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X, BLOCK_X };
						SDL_Rect bdest = { 144, 0, 16, 16 };
						SDL_RenderCopy(renderer, sprites, &bdest, &brect);//dodavanje brusha u renderer
					}
				}
				else
			{
					SDL_Rect location = { j*BLOCK_X / 4, i*BLOCK_X / 4, BLOCK_X / 4, BLOCK_X / 4 };
					SDL_Rect block = { 256 + ((j % 4) * 16 / 4) + (map[i][j] == 3)*((time % 90) / 15) * 16 , (map[i][j] - 1) * 16 + ((i % 4) * 16 / 4), 16 / 4, 16 / 4 };
					SDL_RenderCopy(renderer, sprites, &block, &location);//dodavanje bloka na lokaciju u rendereru
				}
		}
	}
	if (big)
	{
		SDL_Rect rect = { y_brush*BLOCK_X / 4, x_brush*BLOCK_X / 4, BLOCK_X, BLOCK_X };
		SDL_Rect dest = { 272, 208, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);//dodavanje brusha u renderer
	}
	else
	{
		SDL_Rect rect = { y_brush*BLOCK_X / 4, x_brush*BLOCK_X / 4, BLOCK_X/2, BLOCK_X/2 };
		SDL_Rect dest = { 272, 208, 16, 16 };
		SDL_RenderCopy(renderer, sprites, &dest, &rect);//dodavanje brusha u renderer
	}

	SDL_RenderPresent(renderer);//iscrtavanje, odnosno renderovanje
}

void create_base(char** map,int map_h, int map_w)//fja za pravljenje baze
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

}

void clear_base(char **map, int map_h, int map_w)
{
	set_map_area(map, map_h - 10, map_w / 2 +4, map_h, map_w, FOREST* (map[map_h - 10][map_w / 2 + 4]==FOREST)+ICE* (map[map_h - 10][map_w / 2 + 4]==ICE), 1);
	set_map_area(map, map_h - 10, map_w / 2 - 8, map_h, map_w, FOREST * (map[map_h - 10][map_w / 2 -8] == FOREST) + ICE * (map[map_h - 10][map_w / 2 - 8] == ICE), 1);
	set_map_area(map, map_h - 10, map_w / 2 - 4, map_h, map_w, FOREST * (map[map_h - 10][map_w / 2 -4] == FOREST) + ICE * (map[map_h - 10][map_w / 2 - 4] == ICE), 1);
	set_map_area(map, map_h - 10, map_w / 2, map_h, map_w, FOREST * (map[map_h - 10][map_w / 2 ] == FOREST) + ICE * (map[map_h - 10][map_w / 2] == ICE), 1);
	set_map_area(map, map_h - 6, map_w / 2 + 4, map_h, map_w, FOREST * (map[map_h - 6][map_w / 2 + 4] == FOREST) + ICE * (map[map_h - 6][map_w / 2 + 4] == ICE), 1);
	set_map_area(map, map_h - 6, map_w / 2 - 8, map_h, map_w, FOREST * (map[map_h - 6][map_w / 2 -8] == FOREST) + ICE * (map[map_h - 6][map_w / 2 - 8] == ICE), 1);
	set_map_area(map, map_h - 4, map_w / 2 + 4, map_h, map_w, FOREST * (map[map_h - 4][map_w / 2 +4] == FOREST) + ICE * (map[map_h - 4][map_w / 2 + 4] == ICE), 1);
	set_map_area(map, map_h - 4, map_w / 2 - 8, map_h, map_w,  FOREST * (map[map_h - 4][map_w / 2 -8] == FOREST) + ICE * (map[map_h - 4][map_w / 2 - 8] == ICE), 1);
}



int build_map(int map_height, int map_width/*,char** mapx*/)
{
	int map_h = 4 * map_height;
	int map_w = 4 * map_width;//sirina i duzina se salju u blokovima od 4x4
	SDL_Event event;
	SDL_Window *window;    
	SDL_Texture* sprites = NULL;
	SDL_Renderer* renderer=NULL;//inicijalizacija rendera i tekstura za crtanje
	int time = 0;//vreme sluzi za animaciju vode
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Map Builder",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,BLOCK_X/4*map_w, BLOCK_X/4*map_h,0);//kreiranje prozora
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//kreiranje renderera i biranje prozora u koji renderuje
	SDL_Surface *surface = NULL;//slika sa koje ce se uzimati teksture
	surface = IMG_Load("sprites.png");//nece biti ista lokacija fajla vrvtno na kraju
	sprites = SDL_CreateTextureFromSurface(renderer, surface);//od slike pravi teksturu
	SDL_FreeSurface(surface);
	if (surface == NULL)
	{
		printf("Cannot find sprites.png!\n\n");

		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		exit(1);
		return 0;
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
	set_map_area(map, 0, 0, map_h, map_w, 7, 1);
	set_map_area(map, 0, map_w - 4, map_h, map_w, 7, 1);
	set_map_area(map, map_h - 10, map_w / 2 - 2, map_h, map_w, 11, 1);//postavljanje vrednosti za spawn pointove neprijateljskih i igracevog tenka
	while (done!=1)//petlja za input
	{
		time = (time + 1) % 1000;
		set_map_area(map, 0, 0, map_h, map_w, 7, 1);
		set_map_area(map, 0, map_w - 4, map_h, map_w, 7, 1);
		set_map_area(map, map_h - 10, map_w / 2 - 2, map_h, map_w, 11, 1);
		SDL_Delay(15);//delay za normalnu animaciju vode
		render_map(renderer, sprites, map, map_h, map_w, x, y,time,big);
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
					create_base(map, map_h, map_w);
					set_map_area(map, 0, 0, map_h, map_w, 7, 1);
					set_map_area(map, 0, map_w - 4, map_h, map_w, 7, 1);
					set_map_area(map, map_h - 10, map_w / 2 - 2, map_h, map_w, 11, 1);
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
				case SDLK_f:if (big==1) big = 0;//menjanje velicine cetkice
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
	set_map_area(map, 0, 0, map_h, map_w, EMPTY, 1);
	set_map_area(map, 0, map_w - 4, map_h, map_w, EMPTY, 1);
	set_map_area(map, map_h - 10, map_w / 2 - 2, map_h, map_w, EMPTY, 1);//skidanje vrednosti spawna jer su to realno prazna polja
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

int generate_random_map(int map_height, int map_width)
{	
	int map_h = 4 * map_height;
	int map_w = 4 * map_width;
	srand(time(NULL));
	FILE* fmap = fopen("Maps\\random_map.bin", "wb");
	if (fmap == NULL)
	{
		printf("Error creating file!\n");
		return 0;
	}
	char** map;
	map = allocate_map(map_h, map_w);
	if (map == NULL)
	{
		printf("Error allocating memory for map!\n");
		return 0;
	}
	int tiles;
	tiles = (map_height + map_width) * 2;//koliko polja ce da izabere
	reset_map(map, map_h, map_w);
	int tile_counter = 0;
	int xr,yr ;
	xr = yr = 0;
	int type = 0;
	while (tile_counter < tiles)
	{
		xr = random(map_height);
		yr = random(map_width);//biraju se blokovi 4x4 da mapa ne bi izgledala uzasno
			xr =xr* 4;
			yr =yr* 4;
			type =1+random(6);
			if (type > 5) type = 1;//da bi bila malo veca sansa za cigle
			generate_tiles(map,map_h, map_w, xr, yr, type);
			tile_counter++;

	}
	set_map_area(map, 0, 0, map_h, map_w, EMPTY, 1);
	set_map_area(map, 0, map_w - 4, map_h, map_w, EMPTY, 1);
	generate_path(map, map_h, map_w);
	create_base(map, map_h, map_w);
	clear_base(map, map_h, map_w);
	set_map_area(map, map_h - 10, map_w / 2 - 2, map_h, map_w, EMPTY, 1);//oslobadjanje spawn pointova
	//build_map(map_height, map_width,map);
	print_map_file(map, map_h, map_w, fmap);
	fclose(fmap);
	deallocate_map(map, map_h);
	return 1;
}

void set_map_area(char** map, int x, int y, int map_h, int map_w, int type,int big)
{//fja za postavljanje datog polja na dati tip, kao i susedna polja, da bi se formirao 2x2 kvadrat ili 4x4 ako je velika cetkica
	if (x < map_h - 1 && y < map_w - 1)
	{
		if(map[x][y]!=BASE)
		map[x][y] = type;
		if (x + 1 < map_h && map[x+1][y] != BASE)
			map[x + 1][y] = type;
		if (y + 1 < map_w && map[x ][y+1] != BASE)
			map[x][y + 1] = type;
		if ((y + 1 < map_w) && (x + 1 < map_h) && map[x + 1][y+1] != BASE)
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

void generate_path(char** map, int map_h, int map_w)
{
	int x, y;
	x = 0, y = 0;
	int xpath, ypath;
	int direction;
	xpath = map_h-10 ;
	ypath = (int)map_w / 2 - 2;
	while (x < xpath || y < ypath)
	{
		direction = random(3);
		if (direction && x <xpath)
		{
			if (map[x][y] == FOREST) set_map_area(map, x, y, map_h, map_w, FOREST, 1);
			else if (map[x][y] == ICE) set_map_area(map, x, y, map_h, map_w, ICE, 1);
			else set_map_area(map, x, y, map_h, map_w, EMPTY, 1);
			x += 4;
		}
		else if (y < ypath)
		{
			if (map[x][y] == FOREST) set_map_area(map, x, y, map_h, map_w, FOREST, 1);
			else if (map[x][y] == ICE) set_map_area(map, x, y, map_h, map_w, ICE, 1);
			else set_map_area(map, x, y, map_h, map_w, EMPTY, 1);
			y += 4;
		}
	}
	x = 0;
	y = map_w - 4;
	xpath = map_h - 12;
	ypath = (int)map_w / 2 - 2;
	while (x < xpath || y > ypath)
	{
		direction = random(3);
		if (direction && x <xpath)
		{
			if (map[x][y] == FOREST) set_map_area(map, x, y, map_h, map_w, FOREST, 1);
			else if (map[x][y] == ICE) set_map_area(map, x, y, map_h, map_w, ICE, 1);
			else set_map_area(map, x, y, map_h, map_w, EMPTY, 1);
			x += 4;
		}
		else if (y > ypath)
		{
			if (map[x][y] == FOREST) set_map_area(map, x, y, map_h, map_w, FOREST, 1);
			else if (map[x][y] == ICE) set_map_area(map, x, y, map_h, map_w, ICE, 1);
			else set_map_area(map, x, y, map_h, map_w, EMPTY, 1);
			y -= 4;
		}
	}
	return;
}

void generate_tiles(char** map, int map_h, int map_w, int x, int y, int type)
{
	int u,d, l,r;//up down i left right
	int val = 0;
	set_map_area(map, x, y, map_h, map_w, type, 1);
		u = random(2);
		l = random(2);
		r = random(2);
		d = random(2);//flegovi da li ce ici gore desno levo i dole
		val =1+ random((int)(map_h/8));//koliko polja ce ici gore dole levo ili desno
		if (u==1 && x>0)
		{
			for (int i = 1;i<=val;i++)
				if(x-4*i>=0)
			set_map_area(map, x-4*i, y, map_h, map_w, type,1);
		}
		val = 1 + random((int)(map_h / 8));
		if(d==1)
		{
			for (int i = 1;i<=val;i++)
			set_map_area(map, x+4*i, y, map_h, map_w, type, 1);
		}
		val =1+ random((int)(map_w/8));
		if (l==1 && y>0)
		{
			for(int i=1;i<=val;i++)
				if (y - 4 * i >= 0)
			set_map_area(map, x , y-4*i, map_h, map_w, type, 1);
		}
		val = 1 + random((int)(map_w / 8));
		if(r==1)
		{
			for (int i = 1;i<=val;i++)
			set_map_area(map, x, y+4*i, map_h, map_w, type, 1);
		}
	return;
}

