#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _poly* Position;
typedef struct _poly{
    int coeff;
	int pow;
	Position next;
}poly;

int FromFile(char*, Position, Position);
int Insert_Sort(int, int, Position);
int PrintPoly(Position);
int AddPoly(Position, Position, Position);
int MultiplyPoly(Position, Position, Position);
int FreeList(Position);



int main() {
	poly firstP = { .coeff = 0, .pow = 0, .next = NULL };
	poly secondP = { .coeff = 0, .pow = 0, .next = NULL };

	poly sum = { .coeff = 0, .pow = 0, .next = NULL };
	poly product = { .coeff = 0, .pow = 0, .next = NULL };

	FromFile("polinomi.txt", &firstP, &secondP);

	printf("Prvi polinom: ");
	PrintPoly(firstP.next);
	printf("Drugi polinom: ");
	PrintPoly(secondP.next);

	AddPoly(firstP.next, secondP.next, &sum);
	printf("\nZbroj: ");
	PrintPoly(sum.next);

	MultiplyPoly(firstP.next, secondP.next, &product);
	printf("Umnozak: ");
	PrintPoly(product.next);

	//oslobađanje memorije
	FreeList(firstP.next);
	FreeList(secondP.next);
	FreeList(sum.next);
	FreeList(product.next);

	return 0;
}

int FromFile(char* filename, Position first, Position second) {

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("\ngreska pri otvaranju file-a");
		exit(EXIT_FAILURE);
	}

	char buffer[1024];
	int coeff, pow;
	int offset;

	// prvi polinom
	if (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char* ptr = buffer;
		while (sscanf(ptr, "%d %d%n", &coeff, &pow, &offset) == 2) {
			Insert_Sort(coeff, pow, first);
			ptr += offset; // pomakni se iza pročitanog para
		}
	}

	// drugi polinom
	if (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char* ptr = buffer;
		while (sscanf(ptr, "%d %d%n", &coeff, &pow, &offset) == 2) {
			Insert_Sort(coeff, pow, second);
			ptr += offset;
		}
	}

	fclose(fp);
	return 0;
}

int Insert_Sort(int coeff, int pow, Position head) {
	Position q;
	q = (Position)malloc(sizeof(poly));

	if (q == NULL) {
		printf("\nalogacija nije uspjela");
		exit(EXIT_FAILURE);
	}

	q->coeff = coeff;
	q->pow = pow;
	q->next = NULL;
	
	Position prev = head;
	Position curr = head->next;

	while (curr != NULL && curr->pow > pow)
	{
		prev = curr;
		curr = curr->next;
	}

	// ako postoji isti eksponent - zbroji
	if (curr != NULL && curr->pow == pow) {
		curr->coeff += coeff;
		free(q);
		if (curr->coeff == 0) { // izbriši ako 0
			prev->next = curr->next;
			free(curr);
		}
		return 0;
	}

	q->next = curr;
	prev->next = q;

	return 0;
}

int PrintPoly(Position p) {
	if (p == NULL) {
		printf("0\n");
		return 0;
	}

	int first = 1; // pokazuje je li prvi član

	while (p != NULL) {
		if (!first) {
			if (p->coeff > 0)
				printf(" + ");
			else
				printf(" - ");
		}
		else {
			// ako prvi koeficijent negativan, ispiši minus odmah
			if (p->coeff < 0)
				printf("-");
			first = 0;
		}

		int absCoeff = p->coeff < 0 ? -p->coeff : p->coeff;
		printf("%d", absCoeff);

		if (p->pow != 0)
			printf("x^%d", p->pow);

		p = p->next;
	}

	printf("\n");
	return 0;
}

int AddPoly(Position p1, Position p2, Position result) {
	while (p1 != NULL) {
		Insert_Sort(p1->coeff, p1->pow, result);
		p1 = p1->next;
	}
	while (p2 != NULL) {
		Insert_Sort(p2->coeff, p2->pow, result);
		p2 = p2->next;
	}
	return 0;
}

int MultiplyPoly(Position p1, Position p2, Position result) {
	for (Position i = p1; i != NULL; i = i->next) {
		for (Position j = p2; j != NULL; j = j->next) {
			int newCoeff = i->coeff * j->coeff;
			int newPow = i->pow + j->pow;
			Insert_Sort(newCoeff, newPow, result);
		}
	}
	return 0;
}

int FreeList(Position head) {
	Position temp = NULL;

	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}

	return 0;
}





