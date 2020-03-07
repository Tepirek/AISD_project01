#include <stdio.h>
#include <stdlib.h>

struct Node {
	int value;
	Node* nextNode;
};

class Number {
	char sign;
	Node* string;

public:
	Number() {
		sign = '+';
		Node* newNode = new Node;
		newNode->value = 0;
		newNode->nextNode = nullptr;
		string = newNode;
	}
	char getSign() {
		return sign;
	}
	Node* getString() {
		return string;
	}
	void setSign(char s) {
		sign = s;
	}
	void addFirst(int v) {
		Node* newNode = new Node;
		newNode->value = v;
		newNode->nextNode = nullptr;
		Node* tmp = string->nextNode;
		string->nextNode = newNode;
		newNode->nextNode = tmp;
	}
	void addLast(int v) {
		Node* newNode = new Node;
		newNode->value = v;
		newNode->nextNode = nullptr;
		Node* tmp = string;
		while (tmp->nextNode != nullptr) {
			tmp = tmp->nextNode;
		}
		Node* x = tmp->nextNode;
		tmp->nextNode = newNode;
		newNode->nextNode = x;
	}
	void printNumber() {
		Node* s = string;
		while (s != nullptr) {
			printf_s("%d", s->value);
			s = s->nextNode;
		}
		printf_s("\n");
	}
	void printStats() {
		printf_s("SIGN   -> %c\n", sign);
		printf_s("NUMBER -> ");
		printNumber();
	}
	void replaceFirstValue(int v) {
		string->value = v;
	}
	void printNodes() {
		int cnt = 1;
		Node* tmp = string;
		while (tmp != nullptr) {
			printf_s("%0d. Node, value = %d, next = %p\n", cnt, tmp->value, &tmp->nextNode);
			tmp = tmp->nextNode;
			cnt++;
		}
	}
};

void addNumbers(Number** numbers, int position, int a, int b);
void subtractNumbers(Number** numbers, int position, int a, int b);
void printMinNumber(int amountOfNumbers, Number** numbers);
void printMaxNumber(int amountOfNumbers, Number** numbers);
void printAllNumbers(int amountOfNumbers, Number** numbers);

int main(void) {

	int amountOfNumbers;
	scanf_s("%d ", &amountOfNumbers);
	Number** numbers = (Number**) malloc(amountOfNumbers * sizeof(Number));

	for (int i = 0; i < amountOfNumbers; i++) {
		Number* n = new Number();
		numbers[i] = n;
		int c;
		int counter = 0;
		while ((c = getchar()) != '\n') {
			if (counter == 0 && c == '-') n->setSign(c);
			else if (counter == 0 && n->getSign() == '+') n->replaceFirstValue(c - 48);
			else if (counter == 1 && n->getSign() == '-') n->replaceFirstValue(c - 48);
			else n->addLast(c - 48);
			counter++;
		}
	}

	char input[10];
	while (true) {
		fgets(input, 10, stdin);
		if (input[6] == '+') addNumbers(numbers, input[0], input[4], input[8]);
		else if (input[6] == '-') subtractNumbers(numbers, input[0], input[4], input[8]);
		else if (input[1] == 'i') printMinNumber(amountOfNumbers, numbers);
		else if (input[1] == 'a') printMaxNumber(amountOfNumbers, numbers);
		else if (input[0] == '?') printAllNumbers(amountOfNumbers, numbers);
		else if (input[0] == 'q') exit(0);
	}

	return 0;
}

void addNumbers(Number** numbers, int position, int a, int b) {
	Number* first = numbers[a - 48];
	Node* number1 = first->getString();
	Number* second = numbers[b - 48];
	Node* number2 = first->getString();
	Number* n = new Number();
	int overflow = 0;
	int sum = 0;

	while (number1 != nullptr || number2 != nullptr) {
		sum = overflow + ((number1 == nullptr) ? 0 : number1->value) + ((number2 == nullptr) ? 0 : number2->value);
		if (sum % 9 != sum) overflow = sum % 9;
		else overflow = 0;
		number1 = number1->nextNode;
		number2 = number2->nextNode;
		n->addLast(sum);
		n->printNumber();
	}
}

void subtractNumbers(Number** numbers, int position, int a, int b) {

}

void printMinNumber(int amountOfNumbers, Number** numbers) {
	Number* min = numbers[0];
	for (int i = 0; i < amountOfNumbers; i++) {
	}
}

void printMaxNumber(int amountOfNumbers, Number** numbers) {

}

void printAllNumbers(int amountOfNumbers, Number** numbers) {
	for (int i = 0; i < amountOfNumbers; i++) {
		printf_s("%0d. ", i + 1);
		numbers[i]->printNumber();
	}
}