#pragma once
#pragma once
#include "string"
#include "Account.h"
#include "stdio.h"




typedef struct {
	int score;
	int playerAnswerWrong;
	int playerPlaying;
	int status;
} Information;

typedef struct {
	int stt;

} help;

typedef enum {
	WAITING_PLAYER,
	WAITING_QUESTION,
	PLAYING,
	END_GAME,
} GAMEPLAY_STATUS;

int luckyMember(vector<Account*> accountList);
int calculeteScore(int playerPlaying, int playerLoser);
int choseHelp();



