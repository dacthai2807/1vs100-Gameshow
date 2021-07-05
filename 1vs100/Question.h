#pragma once
#pragma once
#include "stdio.h"
#include "vector"
#include "iostream"
#include "fstream"
#include "string"

#define BUFF_SIZE 1024
#define HISTORY 1
#define SCIENT 2
#define CULTURAL 3
#define SPORT 4

using namespace std;

typedef struct Question {
	int stt;
	char question[BUFF_SIZE];
	char answer1[BUFF_SIZE];
	char answer2[BUFF_SIZE];
	char answer3[BUFF_SIZE];
	char true_ans;
};

vector<Question*> createQuestionList();

Question *newQuestion(char *question, char* answer1, char *answer2, char *answer3, char true_ans);

void addQuestion(vector<Question*> questionList, char *question, char* answer1, char *answer2, char *answer3, char true_ans);
void loadQuesFromfile(char* namefile, vector<Question*> questionList);

void printListQues(vector<Question*> questionList);
