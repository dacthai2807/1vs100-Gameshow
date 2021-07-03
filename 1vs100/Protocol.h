#ifndef __PROTOCOL__
#define __PROTOCOL__


#include "stdafx.h"

#include "Gameplay.h"
#include "Question.h"

typedef enum {
	SIGNIN,
	SIGNUP,
	SIGNOUT,
	CHECK,
	ANSWER,
	LEVEL,
	HINT,
	INFO,
} OPCODE;

typedef enum {
	NOT_SIGNED_IN,
	UNAUTHENTICATED,
	AUTHENTICATED,
} PHASE;

typedef enum {
	SYNTAX_ERROR = 100,
	INVALID_OPERATION = 101,

	USER_NAME_FOUND = 000,
	USER_NAME_NOT_FOUND = 001,
	USER_NAME_BLOCKED = 002,
	USER_NAME_IS_SIGNINED = 003,

	PASSWORD_CORRECT = 010,
	PASSWORD_INCORRECT = 011,
	PASSWORD_INCORRECT_THREE_TIMES = 012,
	PASSWORD_CORRECT_BUT_ACCOUNT_IS_SIGNINED = 013,

	LOGOUT_SUCCESS = 020,

	REGISTER_SUCCESSFULL = 030,
	REGISTER_USERNAME_EXISTED = 031,

	ANSWER_CORRECT = 040,
	ANSWER_INCORRECT = 041,
	ANSWER_INVALID = 042,

	USER_USED_HINT_SUCCESS = 050,
	USER_USED_HINT_FAIL = 051,

	TOPIC_USER_CHOOSE_LEVEL = 060,
	TOPIC_TYPE_INVALID = 061,
	TOPIC_USER_DONT_CHOOSE_LEVEL = 062,

	INFORMATION_SUCCESS = 070,
	INFORMATION_ORTHER_PLAYER_ANSWERING = 071,

	GAME_READY = 080,
	GAME_NOT_READY = 081,
	GAME_END = 082,
	GAME_IS_PLAYING = 083,

} MESSAGE_STATUS;

typedef struct {
	OPCODE code;
	char message[50] = { '\0' };
} Request;

typedef struct {
	GAMEPLAY_STATUS status;
	MESSAGE_STATUS code;
	char message[50] = { '\0' };
	char data[50] = { '\0' };
} Response;
//core function

template <class T>
int sendMessage(int socket, T *buff, int size, int flags);

template <class T>
int receiveMessage(int socket, T *buff, int size, int flags);

// set message response
void setMessageResponse(Response *msg);

void readMessageResponse(Response *msg);

//set opcode request
void setOpcodeRequest(Request *request, char *input);

//request get
void requestGet(int socket);

void requestLogout(int socket, char *username);

void requestCheckInformation(int socket);

void requestGetHelp(int socket);

#endif