#include "Protocol.h"

template <class T>
int receiveMessage(int socket, T *buff, int size, int flags) {
	int ret = recv(socket, buff, size, flags);
	if (ret < 0)
		perror("Error: ");
	return ret;
}

template <class T>
int sendMessage(int socket, T *buff, int size, int flags) {
	int ret = send(socket, buff, size, flags);
	if (ret < 0)
		perror("Error: ");
	return ret;
}

void setMessageResponse(Response *msg) {
	if (msg->code != NULL) {
		switch (msg->code) {
		case SYNTAX_ERROR:
			strcpy_s(msg->message, "Syntax error ");
			break;
		case INVALID_OPERATION:
			strcpy_s(msg->message, "Invalid operation ");
			break;
		case USER_NAME_FOUND:
			strcpy_s(msg->message, "Username is correct ");
			break;
		case USER_NAME_NOT_FOUND:
			strcpy_s(msg->message, "Cannot find account ");
			break;
		case USER_NAME_BLOCKED:
			strcpy_s(msg->message, "Account is blocked ");
			break;
		case USER_NAME_IS_SIGNINED:
			strcpy_s(msg->message, "Login only one account ");
			break;
		case PASSWORD_CORRECT:
			strcpy_s(msg->message, "Login successful ");
			break;
		case PASSWORD_INCORRECT:
			strcpy_s(msg->message, "Password incorrect ");
			break;
		case PASSWORD_INCORRECT_THREE_TIMES:
			strcpy_s(msg->message, "Password is incorrect. Account is blocked ");
			break;
		case LOGOUT_SUCCESS:
			strcpy_s(msg->message, "Logout successful ");
			break;
		case REGISTER_SUCCESSFULL:
			strcpy_s(msg->message, "Register successfull ");
			break;
		case REGISTER_USERNAME_EXISTED:
			strcpy_s(msg->message, "Username is existed ");
			break;
		case PASSWORD_CORRECT_BUT_ACCOUNT_IS_SIGNINED:
			strcpy_s(msg->message, "Account is signin in other client ");
			break;
		case ANSWER_CORRECT:
			strcpy_s(msg->message, "The answer is correct ");
			break;
		case ANSWER_INCORRECT:
			strcpy_s(msg->message, "The answer is incorrect. End game ");
			break;
		case ANSWER_INVALID:
			strcpy_s(msg->message, "The answer is invalid ");
			break;
		case USER_USED_HINT_SUCCESS:
			strcpy_s(msg->message, "User used hint success! ");
			break;
		case USER_USED_HINT_FAIL:
			strcpy_s(msg->message, "User used hint fail! You have used up the suggestions ");
			break;
		case TOPIC_USER_CHOOSE_LEVEL:
			strcpy_s(msg->message, "");
			break;
		case TOPIC_TYPE_INVALID:
			strcpy_s(msg->message, "User choose level is invalid ");
			break;
		case INFORMATION_SUCCESS:
			strcpy_s(msg->message, "");
			break;
		case INFORMATION_ORTHER_PLAYER_ANSWERING:
			strcpy_s(msg->message, "");
			break;
		case GAME_READY:
			strcpy_s(msg->message, "Game ready ");
			break;
		case GAME_NOT_READY:
			strcpy_s(msg->message, "Waiting for other players... ");
			break;
		case TOPIC_USER_DONT_CHOOSE_LEVEL:
			strcpy_s(msg->message, "");
			break;
		case GAME_END:
			strcpy_s(msg->message, "End game. You are champion ");
			break;
		case GAME_IS_PLAYING:
			strcpy_s(msg->message, "\nGame is playing!! You can't login \n");
			break;
		default:
			strcpy_s(msg->message, "Exception ");
			break;
		}
	}
}

void readMessageResponse(Response *msg) {
	if (msg->code != NULL) {
		cout << msg->message << "\n";

		switch (msg->code) {
		case PASSWORD_CORRECT:
			cout << "Hello " << msg->data << "! Welcome to 1 vs 100 game show!\n";
			break;
		case LOGOUT_SUCCESS:
			cout << "Goodbye " << msg->data << "! Thank you for playing!\n";
			break;
		case TOPIC_USER_CHOOSE_LEVEL:
			cout << "User choose level " << msg->data << ".\n";
			break;
		default:
			break;
		}
	}
}

void setOpcodeRequest(Request *request, char *input) {
	char code[BUFF_SIZE], data[BUFF_SIZE];
	splitMessage(input, code, data);
	strcpy(request->message, data);
	if (strcmp(code, "SIGNIN") == 0)
		request->code = SIGNIN;
	else if (strcmp(code, "SIGNUP") == 0)
		request->code = SIGNUP;
	else if (strcmp(code, "SIGNOUT") == 0)
		request->code = SIGNOUT;
	else if (strcmp(code, "CHECK") == 0)
		request->code = CHECK;
	else if (strcmp(code, "INFO") == 0)
		request->code = INFO;
	else if (strcmp(code, "ANSWER") == 0)
		request->code = ANSWER;
	else if (strcmp(code, "LEVEL") == 0)
		request->code = LEVEL;
	else if (strcmp(code, "HINT") == 0)
		request->code = HINT;
}

void requestGet(int socket) {
	Request *request = new Request;
	setOpcodeRequest(request, "CHECK check");
	sendMessage(socket, request, sizeof(Request), 0);
}
void requestCheckInformation(int socket) {
	Request *request = new Request;
	setOpcodeRequest(request, "INFO information");
	sendMessage(socket, request, sizeof(Request), 0);
}

void requestLogout(int socket, char *username) {
	Request *request = new Request;
	char buff[BUFF_SIZE];
	strcpy(buff, "SIGNOUT ");
	strcat(buff, username);
	setOpcodeRequest(request, buff);
	sendMessage(socket, request, sizeof(Request), 0);
}
void requestGetHelp(int socket) {
	Request *request = new Request;
	setOpcodeRequest(request, "HELP help");
	sendMessage(socket, request, sizeof(Request), 0);
}
