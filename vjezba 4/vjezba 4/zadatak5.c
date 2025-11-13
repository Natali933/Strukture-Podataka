#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _postfix* Position;
typedef struct _postfix {
	double number;
	Position Next;
}postfix;

int FromFile(char*, Position);
int push(Position, double);
double pop(Position);
double operation(double, double, char);
void Check(Position);

int main() {
	postfix Head = { 0, NULL };
	
	FromFile("zad.txt", &Head);
	Check(&Head);

	//oslobodit memoriju 

	return 0;
}


int push(Position head, double number) {

	Position q = (Position)malloc(sizeof(postfix));
	if (q == NULL) {
		printf("\ngreska pri alokaciji memorije");
		exit(EXIT_FAILURE);
	}

	q->number = number;

	q->Next = head->Next;
	head->Next = q;

	return 0; // ne mogu exit jer onda odma napustam kod, a to ne zelim nakon odmah prvog pusha-a
}

double pop(Position head) {
	Position to_delete;
	double return_number;

	if (head->Next == NULL) {
		printf("greska: stog je prazan, postfix kriv");
		exit(EXIT_FAILURE);
	}

	to_delete = head->Next;
	return_number = to_delete->number;
	head->Next = to_delete->Next;


	free(to_delete);
	to_delete = NULL; //da ne osane viseci pokazivac

	return return_number;

}

double operation(double a, double b, char op) {
	if (op == '+')
		return a + b;
	else if (op == '-')
		return a - b;
	else if (op == '*')
		return a * b;
	else if (op == '/') {
		if (b == 0) {
			printf("\ndijelenje sa nulom");
			exit(EXIT_FAILURE);
		}
		return a / b;
	}
	else {
		printf("\nnepoznata opracija");
		exit(EXIT_FAILURE);
	}
}

int FromFile(char* filename, Position head) {

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("\ngreska pri otvaranju datoteke");
		exit(EXIT_FAILURE);
	}

	char buffer[50];
	while (fscanf(fp, "%s", buffer) == 1) {

		//ako je broj
		if ((buffer[0] >= '0' && buffer[0] <= '9') ||
			(buffer[0] == '-' && buffer[1] >= '0' && buffer[1] <= '9')){ //pozitivan ili negativan
				
				double number = atof(buffer); // ASCII to float koristi za pretvaranje stringa u double 
				push(head, number);
			}

		//ako je operator
		else if (strlen(buffer) == 1 &&
			(buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '*' || buffer[0] == '/')) {

			double b = pop(head);
			double a = pop(head);
			double rezultat = operation(a, b, buffer[0]);
			push(head, rezultat);
		}
		
		else {
			printf("\nesipravan element");
			exit(EXIT_FAILURE);
		}


	}

	fclose(fp);
	return 0;
}

void Check(Position head) {
	if (head->Next != NULL && head->Next->Next == NULL)
		printf("rezultat: %.2lf\n", head->Next->number);
	else {
		printf("greska\n");
	}
}