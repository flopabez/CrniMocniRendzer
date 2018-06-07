#include <stdlib.h>
#include <stdio.h>
#include "strukture.h"


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

void removeNode(struct listNode* stack) {
	struct listNode* temp = stack->next;
	stack->next = stack->next->next;
	stack->data = temp->data;
	free(temp);
}


char squareCollision(int Ax, int Ay, int Awidth, int Bx, int By, int Bwidth) {
	return !(Ax + Awidth - 1<Bx || Ax>Bx + Bwidth - 1 || Ay + Awidth - 1<By || Ay>By + Bwidth - 1);
}

struct Tank* tankCollision(struct gameState* state, struct Tank* tenkic) {
	struct listNode* temp = (*state).enemyTanks;
	while (temp->data) {
		struct Tank* tenkic1 = (struct Tank*)(*temp).data;
		if ((*temp).data != tenkic &&
			squareCollision((*tenkic).yPos, (*tenkic).xPos, (*tenkic).width, (*tenkic1).yPos, (*tenkic1).xPos, (*tenkic1).width))
			return (*temp).data;
		temp = (*temp).next;
	}
	temp = (*state).playerTanks;
	while (temp->data) {
		struct Tank* tenkic1 = (struct Tank*)(*temp).data;
		if ((*temp).data != tenkic &&
			squareCollision((*tenkic).yPos, (*tenkic).xPos, (*tenkic).width, (*tenkic1).yPos, (*tenkic1).xPos, (*tenkic1).width))
			return (*temp).data;
		temp = (*temp).next;
	}
	return 0;
}

void Move(struct gameState* state, struct Tank* tenkic, char direction) {

	(*tenkic).direction = direction;
	char flag = 1;

	switch (direction) {
	case 0: {
		if ((*tenkic).yPos - (*tenkic).speed < 0) (*tenkic).yPos = 0;
		else {
			struct Tank* temp = tankCollision(state, tenkic);
			if (temp) (*tenkic).yPos = (*temp).yPos + (*temp).width;
			else {
				int limit = ((*tenkic).xPos + (*tenkic).width - 1) / MAP_SCALE + ((*tenkic).xPos%MAP_SCALE != 0);
				int newY = (*tenkic).yPos - (*tenkic).speed;
				for (int i = (*tenkic).xPos / MAP_SCALE; i < limit; i++)
					if ((*state).terrain[newY / MAP_SCALE][i] && (*state).terrain[newY / MAP_SCALE][i] < 4) {
						flag = 0;
						break;
					}
				if (flag) (*tenkic).yPos = newY;
				else (*tenkic).yPos = (*tenkic).yPos / MAP_SCALE * MAP_SCALE;
			}
		}
		break;
	}

	case 1: {
		if ((*tenkic).xPos - (*tenkic).speed < 0) (*tenkic).xPos = 0;
		else {
			struct Tank* temp = tankCollision(state, tenkic);
			if (temp) (*tenkic).xPos = (*temp).xPos + (*temp).width;
			else {
				int limit = ((*tenkic).yPos + (*tenkic).width - 1) / MAP_SCALE + ((*tenkic).yPos%MAP_SCALE != 0);
				int newX = (*tenkic).xPos - (*tenkic).speed;
				for (int i = (*tenkic).yPos / MAP_SCALE; i < limit; i++)
					if ((*state).terrain[i][newX / MAP_SCALE] && (*state).terrain[i][newX / MAP_SCALE] < 4) {
						flag = 0;
						break;
					}
				if (flag) (*tenkic).xPos = newX;
				else (*tenkic).xPos = (*tenkic).xPos / MAP_SCALE * MAP_SCALE;
			}
		}
		break;
	}

	case 2: {
		if ((*tenkic).yPos + (*tenkic).speed > (*state).height*MAP_SCALE - (*tenkic).width) (*tenkic).yPos = (*state).height*MAP_SCALE - (*tenkic).width;
		else {
			struct Tank* temp = tankCollision(state, tenkic);
			if (temp) (*tenkic).yPos = (*temp).yPos - (*temp).width;
			else {
				int limit = ((*tenkic).xPos + (*tenkic).width - 1) / MAP_SCALE + ((*tenkic).xPos%MAP_SCALE != 0);//isto kao case 0
				int newY = (*tenkic).yPos + (*tenkic).speed;//isto kao case 0, samo drugi smer
				for (int i = (*tenkic).xPos / MAP_SCALE; i < limit; i++)//isto kao case 0
					if ((*state).terrain[(newY + (*tenkic).width - 1) / MAP_SCALE][i] && (*state).terrain[(newY + (*tenkic).width - 1) / MAP_SCALE][i] < 4) {//za razliku od case 0, gledamo donju a ne gornju ivicu
						flag = 0;
						break;
					}
				if (flag) (*tenkic).yPos = newY;
				//else (*tenkic).yPos = (*tenkic).yPos / MAP_SCALE * MAP_SCALE +1; nije isto
			}
		}
		break;
	}

	case 3: {
		if ((*tenkic).xPos + (*tenkic).speed > (*state).width*MAP_SCALE - (*tenkic).width) (*tenkic).xPos = (*state).width*MAP_SCALE - (*tenkic).width;
		else {
			struct Tank* temp = tankCollision(state, tenkic);
			if (temp) (*tenkic).xPos = (*temp).xPos - (*temp).width;
			else {
				int limit = ((*tenkic).yPos + (*tenkic).width - 1) / MAP_SCALE + ((*tenkic).yPos%MAP_SCALE != 0);
				int newX = (*tenkic).xPos + (*tenkic).speed;
				for (int i = (*tenkic).yPos / MAP_SCALE; i < limit; i++)
					if ((*state).terrain[i][(newX + (*tenkic).width - 1) / MAP_SCALE] && (*state).terrain[i][(newX + (*tenkic).width - 1) / MAP_SCALE] < 4) {
						flag = 0;
						break;
					}
				if (flag) (*tenkic).xPos = newX;
				//else (*tenkic).xPos = (*tenkic).xPos / MAP_SCALE * MAP_SCALE +1; nije ovako
			}
		}
		break;
	}
	}
}

void updateBullets(struct gameState* state) {

	struct listNode* bulletlist = state->playerBullets;

	int i = 2;
	while (i--) {
		while (bulletlist->data) {
			struct Bullet* bullet = (*bulletlist).data;

			if (bullet->xPos < 0 || bullet->yPos < 0 || bullet->xPos > state->width*MAP_SCALE - bullet->width || bullet->yPos > state->height*MAP_SCALE - bullet->width) {
				(*bullet).source->inAir--;
				free((*bulletlist).data);
				removeNode(bulletlist);
				continue;
			}
			char clip = (
				(*state).terrain[(*bullet).yPos / MAP_SCALE][(*bullet).xPos / MAP_SCALE] == 1 ||
				(*state).terrain[(*bullet).yPos / MAP_SCALE][(*bullet).xPos / MAP_SCALE] == 2 ||
				(*state).terrain[(*bullet).yPos / MAP_SCALE][((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE] == 1 ||
				(*state).terrain[(*bullet).yPos / MAP_SCALE][((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE] == 2 ||
				(*state).terrain[((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE][(*bullet).xPos / MAP_SCALE] == 1 ||
				(*state).terrain[((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE][(*bullet).xPos / MAP_SCALE] == 2 ||
				(*state).terrain[((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE][((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE] == 1 ||
				(*state).terrain[((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE][((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE] == 2);

			if (clip) {

				switch ((*bullet).direction) {
				case 0: {
					int limit = ((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE + ((*bullet).xPos%MAP_SCALE != 0);
					for (int y = (*bullet).yPos / MAP_SCALE - (*bullet).power + 1; y < (*bullet).yPos / MAP_SCALE + 1; y++)
						for (int x = (*bullet).xPos / MAP_SCALE - 1; x < limit + 1; x++) {
							if ((*state).terrain[y][x] == 2 && (*bullet).power == 2) (*state).terrain[y][x] = 0;
							else if ((*state).terrain[y][x] == 1) (*state).terrain[y][x] = 0;
						}
					break;
				}
				case 1: {
					int limit = ((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE + ((*bullet).yPos%MAP_SCALE != 0);
					for (int x = (*bullet).xPos / MAP_SCALE - (*bullet).power + 1; x < (*bullet).xPos / MAP_SCALE + 1; x++)
						for (int y = (*bullet).yPos / MAP_SCALE - 1; y < limit + 1; y++) {
							if ((*state).terrain[y][x] == 2 && (*bullet).power == 2) (*state).terrain[y][x] = 0;
							else if ((*state).terrain[y][x] == 1) (*state).terrain[y][x] = 0;
						}
					break;
				}
				case 2: {
					int limit = ((*bullet).xPos + (*bullet).width - 1) / MAP_SCALE + ((*bullet).xPos%MAP_SCALE != 0);
					for (int y = (*bullet).yPos / MAP_SCALE + 1; y < (*bullet).yPos / MAP_SCALE + (*bullet).power + 1; y++)
						for (int x = (*bullet).xPos / MAP_SCALE - 1; x < limit + 1; x++) {
							if ((*state).terrain[y][x] == 2 && (*bullet).power == 2) (*state).terrain[y][x] = 0;
							else if ((*state).terrain[y][x] == 1) (*state).terrain[y][x] = 0;
						}
					break;
				}
				case 3: {
					int limit = ((*bullet).yPos + (*bullet).width - 1) / MAP_SCALE + ((*bullet).yPos%MAP_SCALE != 0);
					for (int x = (*bullet).xPos / MAP_SCALE + 1; x < (*bullet).xPos / MAP_SCALE + (*bullet).power + 1; x++)
						for (int y = (*bullet).yPos / MAP_SCALE - 1; y < limit + 1; y++) {
							if ((*state).terrain[y][x] == 2 && (*bullet).power == 2) (*state).terrain[y][x] = 0;
							else if ((*state).terrain[y][x] == 1) (*state).terrain[y][x] = 0;
						}
					break;
				}
				}

				(*bullet).source->inAir--;
				free((*bulletlist).data);
				removeNode(bulletlist);

			}
			else {
				switch ((*bullet).direction) {
				case 0: (*bullet).yPos -= (*bullet).speed; break;
				case 1: (*bullet).xPos -= (*bullet).speed; break;
				case 2: (*bullet).yPos += (*bullet).speed; break;
				case 3: (*bullet).xPos += (*bullet).speed; break;
				}
				bulletlist = (*bulletlist).next;
			}
		}
		bulletlist = state->enemyBullets;
	}

}

void fireBullet(struct gameState* state, struct Tank* tenkic) {

	if ((*tenkic).inAir == 0 || (*tenkic).inAir == 1 && (*tenkic).upgrade > 1) {
		(*tenkic).inAir++;
		struct Bullet* new = (struct Bullet*)malloc(sizeof(struct Bullet));
		(*new).source = tenkic;
		(*new).direction = (*tenkic).direction;
		(*new).speed = (*tenkic).bulletSpeed;
		(*new).power = (*tenkic).bulletPower;
		(*new).width = MAP_SCALE;
		switch ((*new).direction) {
		case 0:
			(*new).yPos = (*tenkic).yPos - (*new).width + MAP_SCALE / 2;
			(*new).xPos = (*tenkic).xPos + ((*tenkic).width - (*new).width) / 2;
			break;
		case 1:
			(*new).yPos = (*tenkic).yPos + ((*tenkic).width - (*new).width) / 2;
			(*new).xPos = (*tenkic).xPos - (*new).width + MAP_SCALE / 2;
			break;
		case 2:
			(*new).yPos = (*tenkic).yPos + (*tenkic).width;
			(*new).xPos = (*tenkic).xPos + ((*tenkic).width - (*new).width) / 2;
			break;
		case 3:
			(*new).yPos = (*tenkic).yPos + ((*tenkic).width - (*new).width) / 2;
			(*new).xPos = (*tenkic).xPos + (*tenkic).width;
			break;
		}
		if ((*tenkic).team) insertBefore(&(*state).enemyBullets, new);
		else insertBefore(&(*state).playerBullets, new);
	}
}

void respawn(struct Tank* tenkic) {
	(*tenkic).hitPoints = 1;
	/* treba da se porta na spawn point i da dobije shield */
}

void hitDetection(struct gameState* state) {

	struct listNode* bulletshell = (*state).playerBullets;
	while (bulletshell) {

		struct Bullet* metak = (struct Bullet*)(*bulletshell).data;
		struct listNode* tankshell = (*state).enemyTanks;
		struct Tank* tenkic = 0;

		while (tankshell) {
			tenkic = (struct Tank*)(*tankshell).data;
			if (squareCollision((*metak).yPos, (*metak).xPos, (*metak).width, (*tenkic).yPos, (*tenkic).xPos, (*tenkic).width))
				break;
			tankshell = (*tankshell).next;
		}
		if (tankshell) {
			(*metak).source->inAir--;
			(*tenkic).hitPoints--;
			if ((*tenkic).hitPoints == 0) {
				(*tenkic).lives--;
				if ((*tenkic).lives) respawn(tenkic);
				else {
					if ((*metak).source->bot == 0)
						(*metak).source->score += (*tenkic).score;
					free(tenkic);
					removeNode(&tankshell);
					break;
				}
			}
		}
		bulletshell = (*bulletshell).next;
	}

	bulletshell = (*state).enemyBullets;
	while (bulletshell) {

		struct Bullet* metak = (struct Bullet*)(*bulletshell).data;
		struct listNode* tankshell = (*state).playerTanks;
		struct Tank* tenkic = 0;

		while (tankshell) {
			tenkic = (struct Tank*)(*tankshell).data;
			if (squareCollision((*metak).yPos, (*metak).xPos, (*metak).width, (*tenkic).yPos, (*tenkic).xPos, (*tenkic).width))
				break;
			tankshell = (*tankshell).next;
		}
		if (tankshell) {
			(*metak).source->inAir--;
			(*tenkic).hitPoints--;
			if ((*tenkic).hitPoints == 0) {
				(*tenkic).lives--;
				if ((*tenkic).lives) respawn(tenkic);
				else {
					if ((*metak).source->bot == 0)
						(*metak).source->score += (*tenkic).score;
					free(tenkic);
					removeNode(&tankshell);
					break;
				}
			}
		}
		bulletshell = (*bulletshell).next;
	}
}
