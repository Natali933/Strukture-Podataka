#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person* Position;
typedef struct person {
	char name[50];
	char surname[50];
	int points;

	Position Next;

}Person;

Position initialization();


int UnosP(char[], char[], int, Position);
int Ispis(Position);
int UnosK(char[], char[], int, Position);
int Search(char[], Position); // radila sa normalnim Position ali sam htjela da mi izbaci sve ljude ako imaju ista prezimena
int Delete(char[], char[], Position);


int main() {
	Person Head = { "", "", 0, NULL };
	
	char name[50], surname[50];
	int points;
	char choice[10];

	printf("UNOS NA POCETAK");
	while (1) {
		printf("\nUnesi ime, prezime, bodove(ili 'kraj' za prekid)");
		
		if (scanf("%49s", name) != 1)
			break;
		
		if (strcmp(name, "kraj") == 0) 
			break;

		if (scanf("%49s %d", surname, &points) != 2) {
			printf("\neispravan unos");
			continue;
		}
		

		UnosP(name, surname, points, &Head);

	}
	
	Ispis(Head.Next);

	printf("\nUNOS NA KRAJ");
	while (1) {
		printf("\nUnesi ime, prezime, bodove(ili 'kraj' za prekid)");

		if (scanf("%49s", name) != 1)
			break;

		if (strcmp(name, "kraj") == 0)
			break;

		if (scanf("%49s %d", surname, &points) != 2) {
			printf("\nneispravan unos");
			continue;
		}

		UnosK(name, surname, points, &Head);
	}

	Ispis(Head.Next);

	printf("\nUnesi prezime osobe koju zelis pronaci(ili 'kraj' za preskok): ");
	
	if (scanf("%49s", surname) != 1)
		printf("neispravan unos");

	if (strcmp(surname, "kraj") != 0) 
		 Search(surname, Head.Next);

	
	printf("\nZelis li obrisati nekoga? (da/ne): ");
	if (scanf("%9s", choice) != 1)
		printf("neispravan unos opcije\n");

	if (strcmp(choice, "da") == 0) {
		printf("\nUnesi ime i prezime osobe koju zelis obrisati: ");
		
		if (scanf("%s %s", name, surname) != 2)
			printf("\nneispravan unos");
		
		Delete(name, surname, &Head);
		
	}
	Ispis(Head.Next);
	

}

Position initialization()
{
	Position q;
	q = (Position)malloc(sizeof(Person));

	if (q == NULL) {
		printf("\ngreska u alokaciji memorije\n");
		exit(EXIT_FAILURE);
	}

	return q;
}

int UnosP(char thisname[], char thissurname[], int x, Position p) 
{
	Position q;
	q = initialization();
	
	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->points = x;
	q->Next = p->Next;
	p->Next = q;

	return 0;
}

int Ispis(Position p)
{
	printf("\nispis liste:\n");

	while (p != NULL)
	{
		printf("%s %s - %d\n", p->name, p->surname, p->points);
		p = p->Next;
	}

	return 0;
}

int UnosK(char thisname[], char thissurname[], int x, Position p)
{
	Position q;

	while (p->Next != NULL)
		p = p->Next;

	q = initialization();

	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->points = x;

	q->Next = p->Next;
	p->Next = q;

	return 0;

}

int  Search(char need[], Position p) {
	int found = 0;
	while (p != NULL) {
		if (strcmp(p->surname, need) == 0) {
			printf("%s %s - %d\n", p->name, p->surname, p->points);
			found++;
		}
		p = p->Next;
	}

	if (found == 0)
		printf("\nnema te osobe");

	return found;
}

int Delete(char thisname[], char thissurname[], Position p) {
	Position prev = p; //na head
	Position curr = p->Next; //na prvi element
	//trazi cvor
	while (curr != NULL){
		if (strcmp(curr->name, thisname) == 0 && strcmp(curr->surname, thissurname) == 0) {
			prev->Next = curr->Next;
			free(curr);
			printf("\nosoba je pronadena");
			return 0;
		}

		prev = curr;
		curr = curr->Next;
	}

		printf("\nOsoba %s %s nije pronaðena.\n", thisname, thissurname);
		return -1;
	
}






