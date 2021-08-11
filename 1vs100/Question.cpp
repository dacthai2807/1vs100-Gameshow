#include "Question.h"

vector<Question*> createQuestionList() {
	vector<Question*> questionList;
	return questionList;
}

Question *newQuestion(char *question, char* answer1, char *answer2, char *answer3, char true_ans) {
	Question *ques = (Question *)malloc(sizeof(Question));
	strcpy_s(ques->question, question);
	strcpy_s(ques->answer1, answer1);
	strcpy_s(ques->answer2, answer2);
	strcpy_s(ques->answer3, answer3);
	ques->true_ans = true_ans;
	return ques;
}

void addQuestion(vector<Question*> questionList, char *question, char* answer1, char *answer2, char *answer3, char true_ans) {
	Question *ques = newQuestion(question, answer1, answer2, answer3, true_ans);
	ques->stt = questionList.size() + 1;
	questionList.push_back(ques);
}

void loadQuesFromfile(char* namefile, vector<Question*> questionList) {
	string line;
	ifstream file(namefile);
	char question[BUFF_SIZE];
	char answer1[BUFF_SIZE];
	char answer2[BUFF_SIZE];
	char answer3[BUFF_SIZE];
	char true_ans;

	if (file.is_open()) {
		while (getline(file, line)) {
			strcpy_s(question, line.c_str());
			getline(file, line);
			strcpy_s(answer1, line.c_str());
			getline(file, line);
			strcpy_s(answer2, line.c_str());
			getline(file, line);
			strcpy_s(answer3, line.c_str());
			getline(file, line);
			true_ans = line[0];
			addQuestion(questionList, question, answer1, answer2, answer3, true_ans);
		}
	}
	else {
		cout << "\n can not open file " << namefile << endl;;
	}
}

void printListQues(vector<Question*> questionList) {
	for (int i = 0; i < questionList.size(); i++) {
		;
		cout << "Question: " << questionList[i]->stt << endl;
		cout << questionList[i]->question << endl;
		cout << "A: " << questionList[i]->answer1 << endl;
		cout << "B: " << questionList[i]->answer2 << endl;
		cout << "C: " << questionList[i]->answer3 << endl;
		cout << "true answer: " << questionList[i]->true_ans << endl;
	}

}
