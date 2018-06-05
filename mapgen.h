#ifndef mapgen_h
#define mapgen_h

#include <stdio.h>
#include<SDL.h>

void generate_path(char** map, int map_h, int map_w);
char** read_map( int* map_h, int* map_w, char* file_name);
void generate_tiles(char** map, int map_h, int map_w, int x,int y, int type);
void set_map_area(char** map, int x, int y, int map_h, int map_w, int type, int big);
void create_base(char** map, int map_h, int map_w);
void reset_map(char** map, int map_h, int map_w);
void print_map(char** map, int map_h, int map_w);
void deallocate_map(char** map, int map_h);
char** allocate_map(int map_h, int map_w);
void render_map(SDL_Renderer* renderer, SDL_Texture* sprites, char ** map, int map_h, int map_w, int x_brush, int y_brush, int time,int big);
void create_base(char** map, int map_h, int map_w);
int build_map(int map_height, int map_width/*,char** mapx*/);
void clear_base(char **map, int map_h, int map_w);
int random(int max);
int generate_random_map(int map_height, int map_width);



#endif