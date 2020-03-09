#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996)

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
	~Number();	
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
		Node* newNode = new Node;
		newNode->value = v;
		newNode->prev = head;
		newNode->next = tmpHeadNext;
		head->next = newNode;
		tmpHeadNext->prev = newNode;
	}
	void addLast(int v) {
		Node* tmpTailPrev = tail->prev;
		Node* newNode = new Node;
		newNode->value = v;
		newNode->next = tail;
		newNode->prev = tmpTailPrev;
		tail->prev = newNode;
		tmpTailPrev->next = newNode;
	}
	void printNumber() {
		Node* tmpTail = tail->prev;
		if (sign == '-') printf_s("-");
		while (tmpTail->prev != nullptr) {
			printf_s("%d", tmpTail->value);
			tmpTail = tmpTail->prev;
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
};

void addNumbers(int a, int b, Number** numbers, int position);
void subtractNumbers(int a, int b, Number** numbers, int position);
void findMin(Number** numbers, int amountOfNumbers);
void findMax(Number** numbers, int amountOfNumbers);
void printAllNumbers(Number** numbers, int amountOfNumbers);

Number* cmpSignMin(Number* a, Number* b);
Number* cmpSignMax(Number* a, Number* b);
Number* cmpLengthMin(Number* a, Number* b);
Number* cmpLengthMax(Number* a, Number* b);
Number* cmpValuesMin(Number* a, Number* b);
Number* cmpValuesMax(Number* a, Number* b);

int main(void) {
	int amountOfNumbers;
	scanf_s("%d ", &amountOfNumbers);
	if (amountOfNumbers < MIN_AMOUNT || amountOfNumbers > MAX_AMOUNT) exit(1);

	Number** numbers = (Number**)malloc(amountOfNumbers * sizeof(Number));
	for (int i = 0; i < amountOfNumbers; i++) {
		char c;
		Number* n = new Number();
		while ((c = getchar()) != '\n') {
			if (c == '-') {
				n->setSign(c);
				continue;
			}
			else if (c == '+') continue;
			n->addFirst(atoi(&c));
		}
		n->countElements();
		numbers[i] = n;
	}
	char input[25];
	while (true) {
		fgets(input, 25, stdin);
		if (input[0] == '?') printAllNumbers(numbers, amountOfNumbers);
		else if (input[0] == 'q') exit(0);
		else if (input[1] == 'i') findMin(numbers, amountOfNumbers);
		else if (input[1] == 'a') findMax(numbers, amountOfNumbers);
		else {
			int a, b, c;
			char operation;
			sscanf(input, "%5d = %5d %c %5d", &a, &b, &operation, &c);
			printf("%5d, %5d, %5d %c\n", a, b, c, operation);
			if (a < amountOfNumbers && b < amountOfNumbers && c < amountOfNumbers) {
				for (int i = 0; i < 25; i++) {
					if (input[i] == '+') {
						if (numbers[a]->getSign() == numbers[b]->getSign()) addNumbers(b, c, numbers, a);
						else subtractNumbers(b, c, numbers, a);
						break;
					}
					else if (input[i] == '-') {
						if (numbers[a]->getSign() == '+' && numbers[b]->getSign() == '-') addNumbers(b, c, numbers, a);
						else subtractNumbers(b, c, numbers, a);
						break;
					}
				}
			}
		}
	}
	return 0;
}

void addNumbers(int a, int b, Number** numbers, int position) {
	Node* tmpA = numbers[a]->getHead()->next;
	Node* tmpB = numbers[b]->getHead()->next;
	Number* newNumber = new Number();
	int valA, valB, overflow = 0, sum = 0;
	if (numbers[a]->getSign() == numbers[b]->getSign()) {
		newNumber->setSign(numbers[a]->getSign());
		while (tmpA->next != nullptr || tmpB->next != nullptr) {
			valA = (tmpA->next == nullptr) ? 0 : tmpA->value;
			valB = (tmpB->next == nullptr) ? 0 : tmpB->value;
			sum = overflow + valA + valB;
			if (sum % 10 == sum) overflow = 0;
			else overflow = 1;
			newNumber->addLast(sum % 10);
			if (tmpA->next != nullptr) tmpA = tmpA->next;
			if (tmpB->next != nullptr) tmpB = tmpB->next;
		}
		if (overflow) newNumber->addLast(overflow);
	}
	numbers[position] = newNumber;
}

void subtractNumbers(int a, int b, Number** numbers, int position) {
	// minuend - subtrahend = difference
	Number* minuend;
	Number* subtrahend;
	Number* difference = new Number();
	if (cmpLengthMax(numbers[a], numbers[b]) == nullptr) {
		minuend = cmpValuesMax(numbers[a], numbers[b]);
	}
	else minuend = cmpLengthMax(numbers[a], numbers[b]);
	difference->setSign(minuend->getSign());
	if (minuend == numbers[a]) subtrahend = numbers[b];
	else subtrahend = numbers[a];
	Node* tmpA = minuend->getHead()->next;
	Node* tmpB = subtrahend->getHead()->next;
	int valA, valB, underflow = 0;
	while (tmpA->next != nullptr || tmpB->next != nullptr) {
		valA = (tmpA->next == nullptr) ? 0 : tmpA->value - underflow;
		valB = (tmpB->next == nullptr) ? 0 : tmpB->value;
		if (valA >= valB) {
			difference->addLast(valA - valB);
			underflow = 0;
		}
		else {
			difference->addLast(valA + 10 - valB);
			underflow = 1;
		}
		if (tmpA->next != nullptr) tmpA = tmpA->next;
		if (tmpB->next != nullptr) tmpB = tmpB->next;
	}
	numbers[position] = difference;
}

void findMin(Number** numbers, int amountOfNumbers) {
	Number* min = numbers[0];
	for (int i = 1; i < amountOfNumbers; i++) {
		if (cmpSignMin(min, numbers[i]) == nullptr) {
			if (min->getSign() == '+') {
				if (cmpLengthMin(min, numbers[i]) == nullptr) {
					min = cmpValuesMin(min, numbers[i]);
				}
				else min = cmpLengthMin(min, numbers[i]);
			}
			else {
				if (cmpLengthMax(min, numbers[i]) == nullptr) {
					min = cmpValuesMax(min, numbers[i]);
				}
				else min = cmpLengthMax(min, numbers[i]);
			}
		}
		else min = cmpSignMin(min, numbers[i]);
	}
	min->printNumber();
}

void findMax(Number** numbers, int amountOfNumbers) {
	Number* max = numbers[0];
	for (int i = 1; i < amountOfNumbers; i++) {
		if (cmpSignMax(max, numbers[i]) == nullptr) {
			if (max->getSign() == '+') {
				if (cmpLengthMax(max, numbers[i]) == nullptr) {
					max = cmpValuesMax(max, numbers[i]);
				}
				else max = cmpLengthMax(max, numbers[i]);
			}
			else {
				if (cmpLengthMin(max, numbers[i]) == nullptr) {
					max = cmpValuesMin(max, numbers[i]);
				}
				else max = cmpLengthMin(max, numbers[i]);
			}
		}
		else max = cmpSignMax(max, numbers[i]);
	}
	max->printNumber();
}

void printAllNumbers(Number** numbers, int amountOfNumbers) {
	for (int i = 0; i < amountOfNumbers; i++) {
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

Number* cmpValuesMin(Number* a, Number* b) {
	Node* tmpA = a->getTail()->prev;
	Node* tmpB = b->getTail()->prev;

	while (true) {
		if (tmpA->prev == nullptr && tmpB->prev == nullptr) return a;
		else if (tmpA->value > tmpB->value) return b;
		else if (tmpA->value < tmpB->value) return a;
		tmpA = tmpA->prev;
		tmpB = tmpB->prev;
	}
}

Number* cmpValuesMax(Number* a, Number* b) {
	Node* tmpA = a->getTail()->prev;
	Node* tmpB = b->getTail()->prev;

	while (true) {
		if (tmpA->prev == nullptr && tmpB->prev == nullptr) return a;
		else if (tmpA->value < tmpB->value) return b;
		else if (tmpA->value > tmpB->value) return a;
		tmpA = tmpA->prev;
		tmpB = tmpB->prev;
	}
}