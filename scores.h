#ifndef score_h
#define scores_h

struct score
{
	int score;
	char* name;
	struct score* next;
};
void insert_score(struct score** list, char* line);
int get_score(char* line);


void update_score(int score);
#endif