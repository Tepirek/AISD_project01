#include <stdio.h>
#include <stdlib.h>

#define MIN_AMOUNT 1
#define MAX_AMOUNT 100000

struct Node {
	int value;
	Node* prev;
	Node* next;
};

class Number {
	int sign;
	long length;
	Node* head;
	Node* tail;

public:
	Number() {
		sign = '+';
		length = 0;
		head = new Node;
		tail = new Node;
		head->value = 'h';
		head->prev = nullptr;
		head->next = tail;
		tail->value = 't';
		tail->prev = head;
		tail->next = nullptr;
	}
	~Number() {
		free(head);
		free(tail);
	}
	int getSign() {
		return sign;
	}
	long getLenght() {
		return length;
	}
	Node* getHead() {
		return head;
	}
	Node* getTail() {
		return tail;
	}
	void setSign(char s) {
		sign = s;
	}
	void addFirst(int v) {
		Node* tmpHeadNext = head->next;
		Node* tmpHeadNextPrev = tmpHeadNext->prev;
		Node* newNode = new Node;
		newNode->value = v;
		newNode->prev = head;
		newNode->next = tmpHeadNext;
		head->next = newNode;
		tmpHeadNext->prev = newNode;
	}
	void printNumber() {
		Node* tmpTail = tail->prev;
		while (tmpTail->prev != nullptr) {
			printf_s("%d", tmpTail->value);
			tmpTail = tmpTail->prev;
		}
		printf_s("\n");
	}
	void printNumberReverse() {
		Node* tmpHead = head->next;
		while (tmpHead->next != nullptr) {
			printf_s("%d", tmpHead->value);
			tmpHead = tmpHead->next;
		}
		printf_s("\n");
	}
	void countElements() {
		Node* tmpHead = head->next;
		long counter = 0;
		while (tmpHead->next != nullptr) {
			counter++;
			tmpHead = tmpHead->next;
		}
		length = counter;
	}
	void printNumberInfo() {
		printf_s("Sign   => %c\n", sign);
		printf_s("Number => ");
		printNumber();
		printf_s("Length => %d\n", length);
	}
};

void findMin(Number** numbers, int amountOfNumbers);
void findMax(Number** numbers, int amountOfNumbers);
void printAllNumbers(Number** numbers, int amountOfNumbers);

Number* cmpSignMin(Number* a, Number* b);
Number* cmpSignMax(Number* a, Number* b);
Number* cmpLengthMin(Number* a, Number* b);
Number* cmpLengthMax(Number* a, Number* b);
Number* cmpValues(Number* a, Number* b);

int main(void) {
	int amountOfNumbers;
	scanf_s("%d ", &amountOfNumbers);
	if (amountOfNumbers < MIN_AMOUNT || amountOfNumbers > MAX_AMOUNT) exit(1);

	Number** numbers = (Number**)malloc(sizeof(Number) * amountOfNumbers);
	for (int i = 0; i < amountOfNumbers; i++) {
		char c;
		Number* n = new Number();
		while ((c = getchar()) != '\n') {
			if (c == '-') {
				n->setSign(c);
				continue;
			}
			n->addFirst(atoi(&c));
		}
		n->countElements();
		n->printNumberInfo();
		numbers[i] = n;
	}
	char input[10];
	while (true) {
		fgets(input, 10, stdin);
		if (input[6] == '+');
		else if (input[6] == '-');
		else if (input[1] == 'i') findMin(numbers, amountOfNumbers);
		else if (input[1] == 'a') findMax(numbers, amountOfNumbers);
		else if (input[0] == '?') printAllNumbers(numbers, amountOfNumbers);
		else if (input[0] == 'q') exit(0);
	}
	return 0;
}

void findMin(Number** numbers, int amountOfNumbers) {
	Number* min = numbers[0];
	for (int i = 1; i < amountOfNumbers; i++) {
		if (cmpSignMin(min, numbers[i]) == nullptr) {
			if (min->getSign() == '+') {
				if (cmpLengthMin(min, numbers[i]) == nullptr) {
					min = cmpValues(min, numbers[i]);
				}
				else min = cmpLengthMin(min, numbers[i]);
			}
			else {
				if (cmpLengthMax(min, numbers[i]) == nullptr) {
					min = cmpValues(min, numbers[i]);
				}
				else min = cmpLengthMax(min, numbers[i]);
			}
		}
		else min = cmpSignMin(min, numbers[i]);
	}
	min->printNumberInfo();
}

void findMax(Number** numbers, int amountOfNumbers) {
	Number* max = numbers[0];
	for (int i = 1; i < amountOfNumbers; i++) {
		if (cmpSignMax(max, numbers[i]) == nullptr) {
			if (max->getSign() == '+') {
				if (cmpLengthMax(max, numbers[i]) == nullptr) {
					max = cmpValues(max, numbers[i]);
				}
				else max = cmpLengthMax(max, numbers[i]);
			}
			else {
				if (cmpLengthMin(max, numbers[i]) == nullptr) {
					max = cmpValues(max, numbers[i]);
				}
				else max = cmpLengthMin(max, numbers[i]);
			}
		}
		else max = cmpSignMax(max, numbers[i]);
	}
	max->printNumberInfo();
}

void printAllNumbers(Number** numbers, int amountOfNumbers) {
	for (int i = 0; i < amountOfNumbers; i++) {
		printf_s("%c", numbers[i]->getSign());
		numbers[i]->printNumber();
	}
}

Number* cmpSignMin(Number* a, Number* b) {
	if (a->getSign() > b->getSign()) return a;
	else if (a->getSign() < b->getSign()) return b;
	else return nullptr;
}

Number* cmpSignMax(Number* a, Number* b) {
	if (a->getSign() < b->getSign()) return a;
	else if (a->getSign() > b->getSign()) return b;
	else return nullptr;
}

Number* cmpLengthMin(Number* a, Number* b) {
	if (a->getLenght() < b->getLenght()) return a;
	else if (a->getLenght() > b->getLenght()) return b;
	else return nullptr;
}

Number* cmpLengthMax(Number* a, Number* b) {
	if (a->getLenght() > b->getLenght()) return a;
	else if (a->getLenght() < b->getLenght()) return b;
	else return nullptr;
}

Number* cmpValues(Number* a, Number* b) {
	Node* tmpA = a->getTail()->prev;
	Node* tmpB = b->getTail()->prev;

	while (true) {
		if (tmpA == a->getHead() && tmpB == b->getHead()) return a;
		else if (a->getSign() == '+') {
			if (tmpA->value < tmpB->value) return a;
			else if (tmpA->value > tmpB->value) return b;
		}
		else {
			if (tmpA->value > tmpB->value) return a;
			else if (tmpA->value < tmpB->value) return b;
		}
		tmpA = tmpA->prev;
		tmpB = tmpB->prev;
	}
}