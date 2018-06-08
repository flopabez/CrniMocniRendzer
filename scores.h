#ifndef score_h
#define scores_h

struct score
{
	int score;
	char name[30];
	struct score* next;
};
int encrypt();
int decrypt();
char* read_key();
void generate_new_key();
void insert_score(struct score** list, char* name, int score, int pos);
void update_score(int score);
void show_score();
#endif