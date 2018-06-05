#ifndef STRUKTURE_H
#define STRUKTURE_H
#include<SDL.h>
#define FPS 60
#define MAP_SCALE 12

struct listNode {
	void* data;
	struct listNode* next;
};

struct Tank {
	int xPos, yPos, width;
	char direction, speed;
	char upgrade, lives, hitPoints, bulletSpeed, bulletPower, inAir, score;
	char team, bot, frame;
};

struct movementWrapper {
	char up, left, down, right;
	struct Tank* tenkic;
};

struct Bullet {
	struct Tank* source;
	int xPos, yPos, width;
	char direction, speed, power;
};

struct gameState {
	struct listNode *playerTanks, *enemyTanks;
	struct listNode *playerBullets, *enemyBullets;
	char **terrain;
	char height, width;
	char timeStop; //clock pickup timer

	SDL_Renderer** renderer;
	SDL_Texture** sprites;
	int time;

};

struct Level {
	struct gameState* state;
	struct listNode* enemies;
};


struct listNode* newNode(void* data);
void insertBefore(struct listNode** stack, void* data);
void removeNode(struct listNode** stack);
char squareCollision(int Ax, int Ay, int Awidth, int Bx, int By, int Bwidth);
struct Tank* tankCollision(struct gameState* state, struct Tank* tenkic);
void Move(struct gameState* state, struct Tank* tenkic, char direction);
void updateBullets(struct gameState* state, struct listNode* bulletlist);
void fireBullet(struct gameState* state, struct Tank* tenkic);
void respawn(struct Tank* tenkic);
void hitDetection(struct gameState* state);

#endif
