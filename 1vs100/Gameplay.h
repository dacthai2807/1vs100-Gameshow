#pragma once
#pragma once
#include "string"
#include "Account.h"
#include "stdio.h"
#include "Question.h"
#include "ws2tcpip.h"




typedef struct {
	char ans;
	string userName;
} InformationPlayer;

typedef struct {
	int numberMainPlayer;
	int status;
	int phayerAnswerWrong;
	int phayerPlaying;
} InformationGame;

typedef enum {
	WAITING_PLAYER,
	WAITING_QUESTION,
	CHOSE_PLAYER,
	SEND_QUESTION,
	SEND_ANSWER,
	PLAYING,
	END_GAME,
} GAMEPLAY_STATUS;

int checkHelp();
int luckyMember();
int calculeteScore(int playerPlaying, int playerLoser);




