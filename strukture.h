#ifndef STRUKURE_H
#define STRUKURE_H


struct listNode;
struct listNode* newNode(void* data);
void insertBefore(struct listNode** stack, void* data);
void removeNode(struct listNode** stack);
struct Tank;
struct movementWrapper;
struct Bullet;
struct gameState;
struct Level;
char squareCollision(int Ax, int Ay, int Awidth, int Bx, int By, int Bwidth);
struct Tank* tankCollision(struct gameState* state, struct Tank* tenkic);
void Move(struct gameState* state, struct Tank* tenkic, char direction);
void updateBullets(struct gameState* state, struct listNode* bulletlist);
void fireBullet(struct gameState* state, struct Tank* tenkic);
void respawn(struct Tank* tenkic);
void hitDetection(struct gameState* state);


#endif
