#include "Account.h"


int checkLogin(char * usernameAndPass) {
	return 1;
};
int checkSignUp(char* usernameAndPass) {
	return 1;
};
void writeFileAccount(char* userNameAndPass) {

};
int checkLogout() {
	return 1;
};


//find account by username
Account *findUserNameAccount(vector<Account*> accountList, char *username) {
	for (int i = 0; i < accountList.size(); ++i) {
		if (strcmp(accountList[i]->username, username) == 0) {
			return accountList[i];
		}
	}
	return NULL;
}

//add account
void addAccount(vector<Account*> accountList, char *username, char *password, int accountStatus) {
	Account *newAccount = new Account(username, password, OFFLINE, accountStatus, 0);
	accountList.push_back(newAccount);
}

//printf all account
void printAccountList(vector<Account*> accountList) {
	for (int i = 0; i < accountList.size(); ++i) {
		cout << "STT: " << i << "\n";
		cout << "Username: " << accountList[i]->username << "\n";
		cout << "Password: " << accountList[i]->password << "\n";
		cout << "Account: " << (accountList[i]->accountStatus == ACTIVE ? "Active" : "Blocked") << "\n";
		cout << "Status : " << (accountList[i]->status == ONLINE ? "Online" : "Offline") << "\n";
		cout << "Count wrong password: " << accountList[i]->countSignIn << "\n";
		cout << "*-------------------------------------*\n";
	}
}

void blockAccount(vector<Account*> accountList, char *username) {
	Account *curAccount = findUserNameAccount(accountList, username);
	curAccount->accountStatus = BLOCKED;
}

void signinAccount(vector<Account*> accountList, char *username) {
	Account *curAccount = findUserNameAccount(accountList, username);
	curAccount->status = ONLINE;
}

int countMemberOnline(vector<Account*> accountList) {
	int members = 0;

	for (int i = 0; i < accountList.size(); ++i) {
		if (accountList[i]->status == ONLINE) {
			++members;
		}
	}
	return members;
}