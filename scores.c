#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<SDL.h>
#include "scores.h"


//122

void generate_new_key()
{
	FILE* fkey = fopen("resursi\\key.bin", "rb");
	char key[10000];
	for (int i = 0;i < 9999;i++)
		key[i] = 1+ random(20);
	key[9999] = '\0';
	fprintf(fkey, "%s", key);
	fclose(fkey);
}

char* read_key()
{
	FILE* key = fopen("resursi\\key.bin", "rb");
	char* k=(char*)malloc(sizeof(char)*10000);
	fscanf(key, "%s", k);
	fclose(key);
	return k;
}
int encrypt()
{
	FILE *fp1 = fopen("resursi\\highscores.txt", "r");
	if (fp1 == NULL)
	{
		return 0;
	}
	FILE *fp2 = fopen("resursi\\pp2.bin", "wb");
	if (fp2 == NULL)
	{
		return 0;
	}
	char ch;
	while (1)
	{
		ch = fgetc(fp1);
		if (ch == EOF)
		{
			break;
		}
		else
		{
			ch = ch-37;
			fputc(ch, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 1;
}

int decrypt()
{
	FILE* fp1 = fopen("resursi\\pp2.bin", "rb");
	if (fp1 == NULL)
	{
		return 0;
	}
	FILE* fp2 = fopen("resursi\\highscores.txt", "w");
	if (fp2 == NULL)
	{
		return 0;
	}
	char ch;
	while (1)
	{
		ch = fgetc(fp1);
		if (ch == EOF)
		{
			break;
		}
		else
		{
			ch = ch +37;
			fputc(ch, fp2);
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 1;
}



void insert_score(struct score** list, char* name,int score,int pos)
{
	struct score* temp;
	temp = (struct score*)malloc(sizeof(struct score));
	if (temp == NULL)
		return;
	strcpy(temp->name, name);
	temp->score = score;
	temp->next = NULL;
	list[pos] = temp;
}

void free_list(struct score** list, int size)
{
	int pos = 0;
	for(int pos=0;pos<size;pos++)
	{
		free(list[pos]);
	}
}

void update_score(int score)
{
	//char* key = read_key();
	char new_name[30];
	char number[20];
	char name[30];
	struct score* list[20];
	int val;
	scanf("%s", new_name);
	int not_inserted = 1;
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	int i = 0;
	char c;
	while( (c=fscanf(fscore,"%s %s ",name ,number))!=EOF && i<19)
	{
		val = atoi(number);
			if ( score < val)
			{
				insert_score(list, name,val, i);i++;
			}
			else { insert_score(list, new_name,score, i);i++;insert_score(list, name,val, i);i++; not_inserted = 0;score = -1; }
	}
	if (not_inserted)
	{
		insert_score(list, new_name,score, i);i++;
	}

	fclose(fscore);
	FILE * new_fscore = fopen("resursi\\highscores.txt", "w");
	for(int j=0;j<i;j++)
		fprintf(new_fscore, "%s %d ", list[j]->name, list[j]->score);
	free_list(list,i);
	fclose(new_fscore);
	encrypt();
}


void show_score()
{
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	char c;
	char name[30], number[20];
	int i = 1;
	while ((c = fscanf(fscore, "%s %s ", name, number)) != EOF)
	{
		printf("%d-%s:%s\n", i, name, number);
		i++;
	}
	fclose(fscore);
	//system("pause");
}

struct score** read_score()
{
	decrypt();
	FILE* fscore = fopen("resursi\\highscores.txt", "r");
	struct score* list[20];
	char c;
	char name[30], number[20];
	int i = 0;
	while ((c = fscanf(fscore, "%s %s ", name, number)) != EOF)
	{
		insert_score(&list, name, number,i);
		i++;
	}
	fclose(fscore);
	return list;
}