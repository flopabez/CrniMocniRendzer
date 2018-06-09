#ifndef mapgen_h
#define mapgen_h

#include <stdio.h>
#include<SDL.h>

#define BRICK 1
#define METAL 2
#define WATER 3
#define ICE 5
#define FOREST 4
#define BASE 6
#define EMPTY 0
#define BLOCK_X 48

void generate_path(char** map, int map_h, int map_w);
char** read_map( char* file_name);
void generate_tiles(char** map, int map_h, int map_w, int x,int y, int type);
void set_map_area(char** map, int x, int y, int map_h, int map_w, int type, int big);
void reset_map(char** map, int map_h, int map_w);
void print_map(char** map, int map_h, int map_w);
void deallocate_map(char** map, int map_h);
char** allocate_map(int map_h, int map_w);
char** get_map(int i, int map_h, int map_w);
void render_map(SDL_Renderer* renderer, SDL_Texture* sprites,SDL_Texture* ins, char ** map, int map_h, int map_w, int x_brush, int y_brush, int time, int big, int type);
void create_base(char** map, int map_h, int map_w);
int build_map(SDL_Window* window,SDL_Renderer* renderer,SDL_Texture* sprites,SDL_Surface* surface,int map_h, int map_w/*,char** mapx*/);
void clear_base(char **map, int map_h, int map_w);
int random(int max);
int generate_random_map(int map_height, int map_width);
void print_map_file(char** map, int map_h, int map_w, FILE* fmap);



#endif