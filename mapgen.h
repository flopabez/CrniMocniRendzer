#ifndef mapgen_h
#define mapgen_h

#include <stdio.h>
#include<SDL.h>

void read_map(char** map, int* map_h, int* map_w);
void generate_tiles(char** map, int map_h, int map_w, int amount, int type);
void set_map_area(char** map, int x, int y, int map_h, int map_w, int type, int big);
void create_base(char** map, int map_h, int map_w);
void reset_map(char** map, int map_h, int map_w);
void print_map(char** map, int map_h, int map_w);
void deallocate_map(char** map, int map_h);
char** allocate_map(int map_h, int map_w);
void render_map(SDL_Renderer* renderer, SDL_Texture* sprites, char ** map, int map_h, int map_w, int x_brush, int y_brush, int time,int big);
void create_base(char** map, int map_h, int map_w);
int build_map(int map_height, int map_width);
int random(int max);
int generate_random_map(int map_h, int map_w, int brick_wall_amount, int metal_wall_amount, int water_amount);



#endif
