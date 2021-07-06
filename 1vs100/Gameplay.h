#pragma once
#pragma once
#include "string"
#include "Account.h"
#include "stdio.h"
#include "Question.h"




typedef struct {
	bool sigIn;
	ANSWER ans;
	int clientPort;
	char clientIp[INET_ADDRSTRLEN];
	string userName;
} InformationPlayer;

typedef struct {
	GAMEPLAY_STATUS status;
	int phayerAnswerWrong;
	int phayerPlaying;
} InformationGame;

typedef enum {
	WAITING_PLAYER,
	WAITING_QUESTION,
	PLAYING,
	END_GAME,
} GAMEPLAY_STATUS;

int luckyMember(vector<Account*> accountList);
int calculeteScore(int playerPlaying, int playerLoser);
int choseHelp();



