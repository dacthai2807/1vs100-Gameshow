#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string"
#include "iostream"
#include "winsock2.h"
#include "ws2tcpip.h"
#pragma comment(lib,"Ws2_32.lib")

#define BUFF_SIZE 2048
using namespace std;

/* This is a variable that represents the state of the client
+status = 0, client is in menu state
+status = 1, client is in log in state
+status = 2, client is in the sending state of the message
+status = 3, client exits the system
+status = 4, client is in log out state*/
char status = '0';// the first state when initializing the client is zero

				  /* This is a variable that represents the login state of the client
				  +login = 0, the client is not in a successful login status
				  +login = 1 the client is in a successful login status */
int login = 0;

/*functional application interface
@return: no return value
*/
void uni() {
	if (status == '0') {
		cout << "____________________________________________________\n";
		cout << "Menu\n";
		cout << "+Please enter value '1', '2', '3', '4' to use the features\n";
		cout << "--choose 1 to login\n";
		cout << "--choose 2 to send message\n";
		cout << "--choose 3 to exit the application\n";
		cout << "--choose 4 to log out\n";
		cout << "____________________________________________________\n";
	}
	if (status == '1') {
		cout << "______________________________\n";
		cout << "+you are log in\n";
		cout << "+Please enter your username or enter '!q' to exit back \n";
		cout << "______________________________\n";
		cout << "enter username: ";
	}
	if (status == '2' && login == 1) {
		cout << "______________________________\n";
		cout << "+You can send a message or enter '!q'to exit back to the menu\n";
		cout << "______________________________\n";
		cout << "enter message: ";
	}
	if (status == '2' && login == 0) {
		status = '0';
		cout << "______________________________\n";
		cout << "+You need to login before sending any message\n";
		cout << "______________________________\n";
		cout << "Please enter values '1', '2', '3', '4' to use the features\n";
	}
	if (status == '3') {
		cout << "______________________________\n";
		cout << "+do you want exit to the system\n";
		cout << "+Enter any character to exit or enter '!q' to back\n";
		cout << "______________________________\n";
		cout << "enter: ";
	}
	if (status == '4' && login == 0) {
		cout << "______________________________\n";
		status = '0';
		cout << "+You are not logged in\n";
		cout << "______________________________\n";
		cout << "Please enter values '1', '2', '3', '4' to use the features\n";
	}
	if (status == '4' && login == 1) {
		cout << "______________________________\n";
		cout << "+do you want logout\n";
		cout << "+Enter any character to logout or enter '!q' to back\n";
		cout << "______________________________\n";
		cout << "enter: ";
	}
}

/*function for users to choose
@return : no return value
*/
void chooseOption() {
	int sizeDataEnter;
	char getEnter[100] = { '\0' };
	char data;
	do {
		cout << "enter: ";
		gets_s(getEnter);
		sizeDataEnter = strlen(getEnter);
		data = getEnter[0];
		if (data <= 48 || data >= 53 || sizeDataEnter != 1) {
			cout << "entered incorrectly\n";
			cout << "Please enter values '1', '2', '3', '4' to use the features\n";
		}
	} while (data <= 48 || data >= 53 || sizeDataEnter != 1);
	status = getEnter[0];// return satatus
}


/*function processing data received from the server
@pram buff: Buffer stores data received from the server
@return: no return value
*/
void processData(char* buff) {
	char mess[10] = { '\0' };// storage frefix
	char data[BUFF_SIZE] = { '\0' };
	for (int i = 0; i < strlen(buff); i++) {
		if (buff[i] == ' ') {
			strcpy_s(data, buff + i);
			break;
		}
		else {
			mess[i] = buff[i];
		}
	}
	// each frefix will return the displayed result
	if (strcmp(mess, "010") == 0) {
		if (status == '1') {
			login = 1;
			status = '0';
			cout << "+++Successfully logged in \n";
		}
		if (status == '2') {
			printf("%s\n", data);
		}
		if (status == '3') {
			cout << "bye\n";
			exit(0);
		}
		if (status == '4') {
			login = 0;
			status = '0';
			cout << "+++Successfully logged out\n";
		}
	}
	else if (strcmp(mess, "-ERROR") == 0) {
		if (status == '1') {
			cout << "---oh no! Login failed\n";
		}
		if (status == '2') {
			cout << "---error\n";
		}
		if (status == '3') {
			cout << "---cannot exit the system\n";
		}
		if (status == '4') {
			cout << "---can't log out\n";
		}
	}
}

/*function send and receive data from the server
@param client: socket in use
@param buff: Buffer stores data sent to the server
@return: no return value
*/
void sendMessage(SOCKET client, char* buff) {
	int ret, messagelen;
	messagelen = strlen(buff);
	//Send message
	ret = send(client, buff, messagelen, 0);
	if (ret == SOCKET_ERROR)
		printf("ERROR: %d", WSAGetLastError());
	//Receive data from server
	ret = recv(client, buff, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT)
			printf("time out!!!");
		else printf("ERROR: %d\n", WSAGetLastError());
	}
	else if (strlen(buff) > 0) {
		buff[ret] = 0;
		cout << buff << endl;
		processData(buff);
	}
}


/*function insert terminating character
@param mess: message prefix
@param data: data transmitted to the server
@return: Messages prefixed with
*/
char* setMessage(char* mess, char* data) {
	char datasend[BUFF_SIZE] = { '\0' };
	strcat_s(datasend, mess);
	strcat_s(datasend, data);
	strcat_s(datasend, "\r\n");
	return datasend;
}

/*function insert prefix
@param data: data transmitted to the server
@param return: complete message
*/
char* setData(char* data) {
	if (status == '1')
		return setMessage("SIGNIN ", data);
	if (status == '2')
		return setMessage("POST ", data);
	if (status == '3')
		return setMessage("QUIT ", data);
	if (status == '4')
		return setMessage("LOGOUT ", data);
}



int main(int argv, char **argc)
{
	if (argv <= 2) {
		cout << "input is not correct";
		return 0;
	}
	int port = atoi(argc[2]);
	char* addr = argc[1];

	//Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);

	//Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, addr, &serverAddr.sin_addr);

	//Request to connect server
	if (connect(client, (SOCKADDR *)&serverAddr, sizeof(serverAddr))) {
		printf("error! can not connect server.\n");
		return 0;
	}
	cout << "Welcome to my application \n";

	//Communicate with server
	int cnt = 0;
	while (1) {
		uni();
		char data[BUFF_SIZE] = { '\0' };
		if (status != '0') {
			//get data from the keyboard
			gets_s(data);
			if (strcmp(data, "!q") == 0) {
				status = '0';
				continue;
			}
			else {
				char send[BUFF_SIZE] = { '\0' };
				strcpy_s(send, setData(data));// add prefix before sending data
				sendMessage(client, send);// send data
			}

		}
		else {
			chooseOption();
		}
	}
	//close socket
	closesocket(client);
	WSACleanup();
	return 0;
}
