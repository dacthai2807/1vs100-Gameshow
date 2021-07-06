#ifndef __ACCOUNT__
#define __ACCOUNT__

#include "stdafx.h"

#define BLOCKED 0
#define ACTIVE 1

#define ONLINE 1
#define OFFLINE 0

struct Account {
	char username[50] = { '\0' };
	char password[50] = { '\0' };
	int status;
	int accountStatus;
	int countSignIn;

	Account(char *username, char *password, int status, int accountStatus, int countSignIn) {
		strcpy_s(this->username, username);
		strcpy_s(this->password, password);
		this->status = status;
		this->accountStatus = accountStatus;
		this->countSignIn = countSignIn;
	}
};
                                                               
Account *findUserNameAccount(vector<Account*> accountList, char *username);      // find Account By Username                            // find account by position
//load acc t? file
vector<Account*>loadAcc(char *addrFIle);

void addAccount(vector<Account*> accountList, char *username, char *password, int accountStatus); //add node to link list
int countMemberOnline(vector<Account*> accountList);                                    //count member online and save position into array
void printListAccount(vector<Account*> accountList);                                                            // print link list

void blockAccount(vector<Account*> accountList, char *username);  //change status of account to block
void signinAccount(vector<Account*> accountList, char *username); //change status to login

#endif