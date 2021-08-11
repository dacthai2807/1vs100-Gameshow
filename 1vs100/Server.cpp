// SingleIOCPServer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <tchar.h>
#include <winsock2.h>
#include <windows.h>
#include "ws2tcpip.h"
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include "string"
#include "fstream"
#include "ctime"
#include "iostream"


#include "Question.h"
#include "Account.h"
#include "Gameplay.h"

#define PORT 6000
#define DATA_BUFSIZE 2048
#define BUFF_SIZE 2048
#define SEND_BUFF 100
#define MAX_CLIENT 5000
#define MAX_ACC 100
#define SERVER_ADDR "127.0.0.1"
#define MAX_PLAYER 101

#define ADDR_ACC "account.txt"
#define ADDR_QUES "question.txt"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
// Structure definition
typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF recvBuff;
	CHAR bufRecv[DATA_BUFSIZE];
	WSABUF sendBuf;
	CHAR bufSend[SEND_BUFF];
	int id;
	int stt;
} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;


vector<Account*> account;
//vector<Question*> question;
InformationGame gamePlay;
LPPER_IO_OPERATION_DATA operationData[MAX_PLAYER + 1] = { 0 };
InformationPlayer player[MAX_PLAYER + 1] = { 0 };




typedef struct {
	SOCKET socket;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

bool stateId[MAX_CLIENT];
CRITICAL_SECTION critical;
int nSock = 0;// total active socket
ofstream fileLog;// file log


unsigned __stdcall serverWorkerThread(LPVOID CompletionPortID);



/*
function load data base*/
void loadDataBase() {
	//account = loadAcc(ADDR_ACC);
	//question = loadQuestion(ADDR_QUES);
}


/*funcion check the end of the message
@param data: array of processing data
@return: If character '\r\n' is present, return true
*/
int hasDelimeted(char* data) {
	if (strlen(data) == 0)
		return 0;
	for (int i = 0; i < strlen(data); i++) {
		if (data[i] == '\r' && data[i + 1] == '\n') {
			return 1;
		}
	}
	return 0;
}

/*function extract the first substring from data
@param buff: array to separate data
@param data: array returns the delimited string
@return: no return value
*/
void extractFromData(char* buff, char* data) {
	int i;
	for (i = 0; i < strlen(buff); i++) {
		data[i] = buff[i];
		if (buff[i] == '\r') {
			if (buff[i + 1] == '\n') {
				break;
			}
		}
	}
	data[i] = '\0';
	int q = 0;
	//split second message
	for (int j = i + 2; j < strlen(buff); j++) {
		buff[q] = buff[j];
		q++;
	}
	buff[q] = '\0';// return buff
}

/*function to remove prefix from buffer
@param buff:buffer to handle
@param mess: the value of prefix
*/
char* processPrefix(char* buff, char* mess) {
	char data[BUFF_SIZE] = { '\0' };
	for (int i = 0; i < strlen(buff); i++) {
		if (buff[i] == ' ') {
			strcpy_s(data, buff + i + 1);
			break;
		}
		else
			mess[i] = buff[i];//prefix
	}
	return data;// prefix separated data part
}




/*function process the data and return the results
@param recvbuff: Array of data received from client
@param sendbuff: string returns the result after processing
@param id: session index
@return: no return value
*/
void processData(char* recvbuff, char* sendbuff, LPPER_IO_OPERATION_DATA perIOdata) {
	char mess[BUFF_SIZE] = { '\0' };// prefix
	char data[BUFF_SIZE] = { '\0' };// data
	//int clientPort = session[id].clientPort;//session's port
	char clientIp[INET_ADDRSTRLEN];// session's ip address
	//strcpy_s(clientIp, session[id].clientIp);
	// separate data and prefix
	strcpy_s(data, processPrefix(recvbuff, mess));
	if (strcmp(mess, "SIGNIN") == 0) {
		//ki?m tra tài kh?a m?t kh?u và tr? v? code cho sendbuff
		if (checkLogin(data, sendbuff)) {
			for (int i = 1; i <= MAX_PLAYER; i++) {
				if (operationData[i] == 0) {
					operationData[i] = perIOdata;
					perIOdata->stt = i;
					break;
				}
			}
		}
	}
	else if (strcmp(mess, "SIGNUP") == 0) {
		if (checkSignUp(data)) {
			writeFileAccount(data);
			memcpy(sendbuff, "010", SEND_BUFF);
		}
		else {
			memcpy(sendbuff, "011", SEND_BUFF);
		}
	}
	else if (strcmp(mess, "SIGNOUT") == 0) {
		if (checkLogout()) {
			memcpy(sendbuff, "020", SEND_BUFF);
		}
		else {
			memcpy(sendbuff, "021", SEND_BUFF);
		}

	
	}
	else if (strcmp(mess, "QUIT") == 0) {
		//returns results for each received message
		
		//writeFileLog(clientIp, clientPort, mess, "+OK", data);// wite file log
	}
	else {
		memcpy(sendbuff, "021", SEND_BUFF);
		//Client submitted malformed
	
	}
}

/*the function clears the disconnected client session structure
@param client: structure to delete
@param id: structure identifier value
*/


int threadid = 0;
int _tmain(int argc, _TCHAR* argv[])
{
	loadDataBase();
	SOCKADDR_IN serverAddr;
	SOCKET listenSock, acceptSock;
	HANDLE completionPort;
	SYSTEM_INFO systemInfo;
	DWORD transferredBytes;
	DWORD flags;
	WSADATA wsaData;
	sockaddr_in clientAddr;
	char clientIp[INET_ADDRSTRLEN];
	int clientAddrLen = sizeof(clientAddr), clientPort;

	gamePlay.status = WAITING_PLAYER;
	InitializeCriticalSection(&critical);

	if (WSAStartup((2, 2), &wsaData) != 0) {
		printf("WSAStartup() failed with error %d\n", GetLastError());
		return 1;
	}

	// Step 1: Setup an I/O completion port
	if ((completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL) {
		printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
		return 1;
	}

	// Step 2: Determine how many processors are on the system
	GetSystemInfo(&systemInfo);

	// Step 3: Create worker threads based on the number of processors available on the
	// system. Create two worker threads for each processor	
	for (int i = 0; i < (int)systemInfo.dwNumberOfProcessors * 2; i++) {
		// Create a server worker thread and pass the completion port to the thread
		if (_beginthreadex(0, 0, serverWorkerThread, (void*)completionPort, 0, 0) == 0) {
			printf("Create thread failed with error %d\n", GetLastError());
			return 1;
		}
	}

	// Step 4: Create a listening socket
	if ((listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		printf("WSASocket() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
	if (bind(listenSock, (PSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("bind() failed with error %d\n", WSAGetLastError());
		return 1;
	}

	// Prepare socket for listening
	if (listen(listenSock, 20) == SOCKET_ERROR) {
		printf("listen() failed with error %d\n", WSAGetLastError());
		return 1;
	}
	printf("Server start !\n");


	while (1) {
		// Step 5: Accept connections
		if ((acceptSock = WSAAccept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen, NULL, 0)) == SOCKET_ERROR) {
			printf("WSAAccept() failed with error %d\n", WSAGetLastError());
			return 1;
		}
		if (nSock == MAX_CLIENT) {
			printf("\nToo many clients.");
			closesocket(acceptSock);
			continue;
		}
		else {

			LPPER_HANDLE_DATA perHandleData;
			LPPER_IO_OPERATION_DATA perIoData;
			// Step 6: Create a socket information structure to associate with the socket
			if ((perHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL) {
				printf("GlobalAlloc() failed with error %d\n", GetLastError());
				return 1;
			}

			// Step 7: Associate the accepted socket with the original completion port
			printf("Socket number %d got connected...\n", acceptSock);
			perHandleData->socket = acceptSock;
			if (CreateIoCompletionPort((HANDLE)acceptSock, completionPort, (DWORD)perHandleData, 0) == NULL) {
				printf("CreateIoCompletionPort() failed with error %d\n", GetLastError());
				return 1;
			}

			// Step 8: Create per I/O socket information structure to associate with the WSARecv call
			if ((perIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL) {
				printf("GlobalAlloc() failed with error %d\n", GetLastError());
				return 1;
			}
			

			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
			clientPort = ntohs(clientAddr.sin_port);
			EnterCriticalSection(&critical);
			for (int i = 0; i < MAX_CLIENT; i++) {
				if (stateId[i] == false) {
					perIoData->id = i;
					stateId[i] = true;
					
					break;
				}
			}
			LeaveCriticalSection(&critical);
			nSock++;
			ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
			perIoData->recvBuff.len = DATA_BUFSIZE;
			perIoData->recvBuff.buf = perIoData->bufRecv;
			flags = 0;

			if (WSARecv(acceptSock, &(perIoData->recvBuff), 1, &transferredBytes, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != ERROR_IO_PENDING) {
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					return 1;
				}
			}
		}
	}
	DeleteCriticalSection(&critical);
	_getch();

	return 0;
}

unsigned __stdcall serverWorkerThread(LPVOID completionPortID)
{
	HANDLE completionPort = (HANDLE)completionPortID;
	DWORD transferredBytes;
	LPPER_HANDLE_DATA perHandleData;
	LPPER_IO_OPERATION_DATA perIoData;
	DWORD flags;
	int myId;
	EnterCriticalSection(&critical);
	threadid = threadid + 1;
	myId = threadid;
	LeaveCriticalSection(&critical);
	//printf("thread %d started !\n", myId);

	while (TRUE) {

		if (GetQueuedCompletionStatus(completionPort, &transferredBytes, (LPDWORD)&perHandleData, (LPOVERLAPPED *)&perIoData, INFINITE) == 0) {
			printf("GetQueuedCompletionStatus() failed with error %d\n", GetLastError());
			continue;
		}

		//printf("thread %d working !\n", myId);
		// Check to see if an error has occurred on the socket and if so
		// then close the socket and cleanup the SOCKET_INFORMATION structure
		// associated with the socket
		if (transferredBytes == 0) {
			EnterCriticalSection(&critical);
			printf("Closing socket %d\n", perHandleData->socket);
			//closeSesion(perIoData, perHandleData, perIoData->id);
			LeaveCriticalSection(&critical);
			continue;
		}

		EnterCriticalSection(&critical);

		switch (gamePlay.status) 
		{
			case WAITING_PLAYER:
			{
				if (hasDelimeted(perIoData->recvBuff.buf)) {
					ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
					cout << perIoData->recvBuff.buf;
					char data[BUFF_SIZE] = { '\0' };
					extractFromData(perIoData->recvBuff.buf, data);
					LeaveCriticalSection(&critical);
					perIoData->sendBuf.buf = perIoData->bufSend;
					processData(data, perIoData->sendBuf.buf, perIoData);
					perIoData->sendBuf.len = strlen(perIoData->sendBuf.buf);

					//cout << strlen(data) << data << "--->" << perIoData->sendBuf.buf << endl;

					if (WSASend(perHandleData->socket, &(perIoData->sendBuf), 1, &transferredBytes, 0, &(perIoData->overlapped), NULL) == SOCKET_ERROR) {
						if (WSAGetLastError() != ERROR_IO_PENDING) {
							printf("WSASend() failed with error %d\n", WSAGetLastError());
							EnterCriticalSection(&critical);
							//closeSesion(perIoData, perHandleData, perIoData->id);
							LeaveCriticalSection(&critical);
							continue;
						}
					}
				}
				else {
					LeaveCriticalSection(&critical);
					// No more bytes to send post another WSARecv() request
					flags = 0;
					ZeroMemory(&(perIoData->overlapped), sizeof(OVERLAPPED));
					perIoData->recvBuff.len = DATA_BUFSIZE;
					perIoData->recvBuff.buf = perIoData->bufRecv;
					if (WSARecv(perHandleData->socket, &(perIoData->recvBuff), 1, &transferredBytes, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR) {
						if (WSAGetLastError() != ERROR_IO_PENDING) {
							EnterCriticalSection(&critical);
							//closeSesion(perIoData, perHandleData, perIoData->id);
							LeaveCriticalSection(&critical);
							continue;
						}
					}
			}

		default:
			break;
		}
		
		}

	}
}