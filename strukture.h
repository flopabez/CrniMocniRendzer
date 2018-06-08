#ifndef STRUKTURE_H
#define STRUKTURE_H
#include<SDL.h>
#include "AIstruct.h"
#define FPS 24
#define MAP_SCALE 12



struct Pickup {
	int yPos, xPos;
	char type;
};

typedef
struct explosion {
	char size;
	int yPos, xPos;
	unsigned int time;
}Explosion;

struct listNode {
	void* data;
	struct listNode* next;
};

struct Tank {
	int xPos, yPos, width, score;
	char earnedLives, pickup;
	char direction, speed;
	char upgrade, lives, hitPoints, bulletSpeed, inAir;
	char team, bot, frame;
	char move, moveDone, pathDone, kamikaze;
	struct tankMovesStack* mList;
	unsigned int shield;
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
	unsigned int shovel;
	char killCount;
	char stage;
	struct Pickup* pickup;
	char dif;
	struct listNode *explosions;
	struct listNode *playerTanks, *enemyTanks;
	struct listNode *playerBullets, *enemyBullets;
	char **terrain;
	char height, width;
	unsigned int timeStop; //clock pickup timer
	unsigned int time;
};

struct Level {
	struct gameState* state;
	struct listNode* enemies;
};



int baseHitDetection(struct gameState* state);
int botCount(struct gameState* state);
struct gameState* initGame();
void freeGame(struct gameState* gameState);
struct gameState* initGame(int difficulty);
void updatePowerUps(struct gameState* state);
void powerUp(struct gameState* state);
void setBase(struct gameState* state, char type);
struct listNode* newNode(void* data);
void insertBefore(struct listNode** stack, void* data);
void removeNode(struct listNode** stack);
char squareCollision(int Ax, int Ay, int Awidth, int Bx, int By, int Bwidth);
struct Tank* tankCollision(struct gameState* state, struct Tank* tenkic);
void Move(struct gameState* state, struct Tank* tenkic, char direction);
void updateBullets(struct gameState* state);
void fireBullet(struct gameState* state, struct Tank* tenkic);
void respawn(struct gameState* state, struct Tank* tenkic);
void hitDetection(struct gameState* state);
void updateBots(struct gameState* state);
struct Tank* spawnTank(struct gameState* state, char tankType, char spawnPoint, char team);
#endif
