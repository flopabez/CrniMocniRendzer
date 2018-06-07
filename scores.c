#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<SDL.h>
#include "scores.h"

int get_score(char* line);
void insert_score(struct score** list, char* line)
{
	struct score* temp;
	struct score* curr;
	temp = (struct score*)malloc(sizeof(struct score));
	if (temp == NULL)
		return;
	char* name;
	name = malloc(sizeof(char)*strlen(line));
	int i = 0;
	while (line[i] != ':')
	{
		name[i] = line[i];
		i++;
	}
	name[i] = '\0';
	i++;
	temp->name = (char*)malloc(sizeof(char)*30);
	if (temp->name == NULL)
		return;
	strcpy(temp->name, name);
	temp->score =get_score(line);
	temp->next = NULL;
	if (*list == NULL)
		*list =temp;
	else
	{
		curr = *list;
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = temp;
	}
}

int get_score(char* line)
{
	int i = 0;
	char* charscore;
	charscore = (char*)malloc(sizeof(char) * 12);
	if (charscore == NULL)
		return 0;
	while (line[i] != ':')
	{
		i++;
	}
	i++;
	int j = 0;
	while (line[i])
	{
		charscore[j] = line[i];
		j++;
		i++;
	}
	int score = atoi(charscore);
	return score;

}

void update_score(int score)
{

	char* name;
	name = (char*)malloc(sizeof(char) * 20);
	if (name == NULL)
		return;
	char* string;
	struct score* list = NULL;
	string = (char*)malloc(sizeof(char) * 50);
	if (string == NULL)
		return;
	char new_score[50];
	scanf("%s", name);
	char dot[2] = ":";
	snprintf(new_score, sizeof(new_score), "%s%s%d", name, dot, score);
	FILE* fscore = fopen("highscores.txt", "r");
	while (fgets(string, 50, fscore))
	{
		if(score<get_score(string))
			insert_score(&list, string);
		else { insert_score(&list, new_score);insert_score(&list, string); score = -1; }
	}
	if (score != -1)
		insert_score(&list, new_score);
	fclose(fscore);
	FILE * new_fscore = fopen("highscores.txt", "w");
	while (list != NULL)
	{
		fprintf(new_fscore, "%s:%d\n", list->name, list->score);
		list = list->next;
	}
	fclose(new_fscore);
}