#define FPS 60
#define MAP_SCALE 10 //pixels per tile length
#define BULLET_WIDTH 10

#include <stdlib.h>
#include <stdio.h>


struct listNode {
	void* data;
	struct listNode* next;
};

struct listNode* newNode(void* data) {
	struct listNode* new = (struct listNode*)malloc(sizeof(struct listNode));
	(*new).next = 0;
	(*new).data = data;
	return new;
}

void insertBefore(struct listNode** stack, void* data) {
	struct listNode* new = newNode(data);
	(*new).next = *stack;
	*stack = new;
}

void removeNode(struct listNode** stack) {
	struct listNode* discard = (*stack);
	*stack = (**stack).next;
	free(discard);
}

struct Tank {
	int xPos, yPos, width;
	char direction, speed;
	char upgrade, lives, hitPoints, bulletSpeed, bulletPower, inAir, score;
	char team;
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
	char timeStop; //clock pickup timer
};

struct Level {
	struct gameState* state;
	struct listNode* enemies;
};

char squareCollision(int Ax, int Ay, int Awidth, int Bx, int By, int Bwidth) {
	return !(Ax+Awidth-1<Bx || Ax>Bx+Bwidth-1 || Ay+Awidth-1<By || Ay>By+Bwidth-1);
}

void Move(struct gameState* state, struct Tank* tenkic, char direction){

	(*tenkic).direction = direction;
	char flag = 1;

	switch(direction){
		case 0:{
			int limit = ((*tenkic).xPos+(*tenkic).width-1) / MAP_SCALE + ((*tenkic).xPos%MAP_SCALE != 0);
			int newY = (*tenkic).yPos - (*tenkic).speed / FPS;
			for (int i=(*tenkic).xPos / MAP_SCALE; i < limit; i++)
			if ((*state).terrain[newY / MAP_SCALE][i] == 1){
				flag = 0;
				break;
			}
			if (flag) (*tenkic).yPos = newY;
			else (*tenkic).yPos = (*tenkic).yPos / MAP_SCALE * MAP_SCALE;
			break;
		}

		case 1:{
			int limit = ((*tenkic).yPos+(*tenkic).width-1) / MAP_SCALE + ((*tenkic).yPos%MAP_SCALE != 0);
			int newX = (*tenkic).xPos - (*tenkic).speed / FPS;
			for (int i=(*tenkic).yPos / MAP_SCALE; i < limit; i++)
			if ((*state).terrain[i][newX / MAP_SCALE] == 1){
				flag = 0;
				break;
			}
			if (flag) (*tenkic).xPos = newX;
			else (*tenkic).xPos = (*tenkic).xPos / MAP_SCALE * MAP_SCALE;
			break;}

		case 2:{
			int limit = ((*tenkic).xPos+(*tenkic).width-1) / MAP_SCALE + ((*tenkic).xPos%MAP_SCALE != 0);//isto kao case 0
			int newY = (*tenkic).yPos + (*tenkic).speed / FPS;//isto kao case 0, samo drugi smer
			for (int i=(*tenkic).xPos / MAP_SCALE; i < limit; i++)//isto kao case 0
			if ((*state).terrain[(newY +(*tenkic).width-1) / MAP_SCALE][i] == 1){//za razliku od case 0, gledamo donju a ne gornju ivicu
				flag = 0;
				break;
			}
			if (flag) (*tenkic).yPos = newY;
			else (*tenkic).yPos = (*tenkic).yPos / MAP_SCALE * MAP_SCALE;//ovo se mozda razlikuje od case 0
			break;
		}

		case 3:{
			int limit = ((*tenkic).yPos+(*tenkic).width-1) / MAP_SCALE + ((*tenkic).yPos%MAP_SCALE != 0);
			int newX = (*tenkic).xPos + (*tenkic).speed / FPS;
			for (int i=(*tenkic).yPos / MAP_SCALE; i < limit; i++)
			if ((*state).terrain[i][(newX +(*tenkic).width-1) / MAP_SCALE] == 1){
				flag = 0;
				break;
			}
			if (flag) (*tenkic).xPos = newX;
			else (*tenkic).xPos = (*tenkic).xPos / MAP_SCALE * MAP_SCALE;
			break;
		}
	}

}

void updateBullets(struct gameState* state, struct listNode* bulletlist){

	while(bulletlist){
		struct Bullet* bullet = (*bulletlist).data;
		char clip = (
			(*state).terrain[(*bullet).yPos / MAP_SCALE][(*bullet).xPos / MAP_SCALE] ||
			(*state).terrain[(*bullet).yPos / MAP_SCALE][((*bullet).xPos +(*bullet).width-1) / MAP_SCALE] ||
			(*state).terrain[((*bullet).yPos +(*bullet).width-1) / MAP_SCALE][(*bullet).xPos / MAP_SCALE] ||
			(*state).terrain[((*bullet).yPos +(*bullet).width-1) / MAP_SCALE][((*bullet).xPos +(*bullet).width-1) / MAP_SCALE]);
		if(clip){
			switch((*bullet).direction){
				case 0:{
					int limit = (*bullet).xPos +(*bullet).width-1 +((*bullet).xPos%MAP_SCALE!=0);
					for(int y=(*bullet).yPos / MAP_SCALE; y < (*bullet).yPos / MAP_SCALE + (*bullet).power; y++)
					for(int x=(*bullet).xPos / MAP_SCALE; x < limit; x++)
					(*state).terrain[y][x] = 0;
					break;
				}
				case 1:{
					int limit = (*bullet).yPos +(*bullet).width-1 +((*bullet).yPos%MAP_SCALE!=0);
					for(int x=(*bullet).xPos / MAP_SCALE; x < (*bullet).xPos / MAP_SCALE + (*bullet).power; x++)
					for(int y=(*bullet).yPos / MAP_SCALE; y < limit; x++)
					(*state).terrain[y][x] = 0;
					break;
				}
				case 2:{
					int limit = (*bullet).xPos +(*bullet).width-1 +((*bullet).xPos%MAP_SCALE!=0);
					for(int y=(*bullet).yPos / MAP_SCALE; y < (*bullet).yPos / MAP_SCALE + (*bullet).power; y++)
					for(int x=(*bullet).xPos / MAP_SCALE; x < limit; x++)
					(*state).terrain[y+((*bullet).width-1)/MAP_SCALE][x] = 0; //ovo mozda ne valja
					break;
				}
				case 3:{
					int limit = (*bullet).yPos +(*bullet).width-1 +((*bullet).yPos%MAP_SCALE!=0);
					for(int x=(*bullet).xPos / MAP_SCALE; x < (*bullet).xPos / MAP_SCALE + (*bullet).power; x++)
					for(int y=(*bullet).yPos / MAP_SCALE; y < limit; x++)
					(*state).terrain[y][x+((*bullet).width-1)/MAP_SCALE] = 0; //ovo isto
					break;
				}
			}
			(*bullet).source->inAir--;
			free(bullet);
			struct listNode* next = (*bulletlist).next;
			removeNode(&bulletlist);
			bulletlist = next;
		}
		else{
			switch((*bullet).direction){
				case 0: (*bullet).yPos -= (*bullet).speed; break;
				case 1: (*bullet).xPos -= (*bullet).speed; break;
				case 2: (*bullet).yPos += (*bullet).speed; break;
				case 3: (*bullet).xPos += (*bullet).speed; break;
			}
			bulletlist = (*bulletlist).next;
		}
	}

}

void fireBullet(struct gameState* state, struct Tank* tenkic){

	if((*tenkic).inAir == 0 || (*tenkic).inAir == 1 && (*tenkic).upgrade > 1){
		(*tenkic).inAir++;
		struct Bullet* new = (struct Bullet*)malloc(sizeof(struct Bullet));
		(*new).source = tenkic;
		(*new).direction = (*tenkic).direction;
		(*new).speed = (*tenkic).bulletSpeed;
		(*new).power = (*tenkic).bulletPower;
		(*new).width = BULLET_WIDTH;
		switch((*new).direction){
			case 0:
				(*new).yPos = (*tenkic).yPos - (*new).width;
				(*new).xPos = (*tenkic).xPos + ((*tenkic).width-(*new).width)/2;
				break;
			case 1:
				(*new).yPos = (*tenkic).yPos + ((*tenkic).width-(*new).width)/2;
				(*new).xPos = (*tenkic).xPos - (*new).width;
				break;
			case 2:
				(*new).yPos = (*tenkic).yPos + (*tenkic).width;
				(*new).xPos = (*tenkic).xPos + ((*tenkic).width-(*new).width)/2;
				break;
			case 3:
				(*new).yPos = (*tenkic).yPos + ((*tenkic).width-(*new).width)/2;
				(*new).xPos = (*tenkic).xPos + (*tenkic).width;
				break;
		}
		if((*tenkic).team) insertBefore(&(*state).enemyBullets, new);
		else insertBefore(&(*state).playerBullets, new);
	}
}
