#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person* Position;
typedef struct person {
	char name[50];
	char surname[50];
	int year;

	Position Next;

}Person;

Position initialization();


int InsertB(char[], char[], int, Position);
int Ispis(Position);
int InsertE(char[], char[], int, Position);
int SearchSurname(char[], Position); // radila sa normalnim Position ali sam htjela da mi izbaci sve ljude ako imaju ista prezimena
int Delete(char[], char[], Position);
Position Search(char[], char[], Position); //htjela sam po imenu i prezimenu ako netko slucajno ima isto prezime

//funkcije za vjezbu 3
Position SearchBef(char[], char[], Position); //isto po imenu i prezimenu
int InsertBef(char[], char[], int, char[], char[], Position);
int InsertAft(char[], char[], int, char[], char[], Position);
int InFile(Position);
int FromFile(Position);
int ClearAll(Position);
int Sort(Position);

int InsertPerson(char[], char[], int*);


int main() {
	Person Head = { "", "", 0, NULL };
	
	char name[50], surname[50];
	int year;
	char choice[10];
	char thisname[50], thissurname[50];

	printf("UNOS NA POCETAK");
	while (1) {
		int status = InsertPerson(name, surname, &year);
		
		if (status == 0) break;
		if (status == -1) continue;
		
		InsertB(name, surname, year, &Head);

	}
	
	Ispis(Head.Next);

	printf("\nUNOS NA KRAJ");
	while (1) {
		int status = InsertPerson(name, surname, &year);

		if (status == 0) break;
		if (status == -1) continue;

		InsertE(name, surname, year, &Head);
	}

	Ispis(Head.Next);

	printf("\nUnesi prezime osobe koju zelis pronaci(ili 'kraj' za preskok): ");
	
	if (scanf("%49s", surname) != 1)
		printf("neispravan unos");

	if (strcmp(surname, "kraj") != 0) 
		 SearchSurname(surname, Head.Next);

	
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

	printf("\nnapisi koga zelis dodati ispred koga(ili 'kraj' za preskok): ");

	if (scanf("%49s", thisname) != 1)
		printf("neispravan unos");

	if (strcmp(thisname, "kraj") != 0) {
		if (scanf("%49s %d", thissurname, &year) != 2)
			printf("\neispravan unos");

			printf("\nispred koga:");

		if (scanf("%49s %49s", name, surname) != 2)
			printf("\neispravan unos");

		InsertBef(thisname, thissurname, year, name, surname, &Head);

	}
	Ispis(Head.Next);

	printf("\nnapisi koga zelis dodati iza koga(ili 'kraj' za preskok): ");

	if (scanf("%49s", thisname) != 1)
		printf("neispravan unos");

	if (strcmp(thisname, "kraj") != 0) {
		if (scanf("%49s %d", thissurname, &year) != 2)
			printf("\neispravan unos");

		printf("\niza koga:");

		if (scanf("%49s %49s", name, surname) != 2)
			printf("\neispravan unos");

		InsertAft(thisname, thissurname, year, name, surname, &Head);

	}
	Ispis(Head.Next);

	InFile(Head.Next);

	ClearAll(&Head); //jer je pri pozivanju FromFile samo dodao opet na vec postojece
	
	printf("\n--iscitano iz file-a--");
	FromFile(&Head);
	Ispis(Head.Next);

	Sort(&Head);
	printf("\n--sortirana lista--");
	Ispis(Head.Next);
	
	return 0; 

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

int InsertB(char thisname[], char thissurname[], int x, Position p) 
{
	Position q;
	q = initialization();
	
	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->year = x;
	q->Next = p->Next;
	p->Next = q;

	return 0;
}

int Ispis(Position p)
{
	printf("\nispis liste:\n");

	while (p != NULL)
	{
		printf("%s %s - %d\n", p->name, p->surname, p->year);
		p = p->Next;
	}

	return 0;
}

int InsertE(char thisname[], char thissurname[], int x, Position p)
{
	Position q;

	while (p->Next != NULL)
		p = p->Next;

	q = initialization();

	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->year = x;

	q->Next = p->Next;
	p->Next = q;

	return 0;

}

int  SearchSurname(char need[], Position p) { 
	int found = 0;
	while (p != NULL) {
		if (strcmp(p->surname, need) == 0) {
			printf("%s %s - %d\n", p->name, p->surname, p->year);
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

			curr->Next = NULL;
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

Position Search(char name[], char surname[], Position p) {
	while (p != NULL && (strcmp(p->name, name) != 0 || strcmp(p->surname, surname) != 0))
		p = p->Next;

	if (p == NULL) return NULL;

	return p;
	
}


Position SearchBef(char name[], char surname[], Position p) {
	while (p->Next != NULL && (strcmp(p->Next->name, name) != 0 || strcmp(p->Next->surname, surname) != 0)) //&& ima veci prioritet od || pa mi trebauju zagrade
		p = p->Next;

	if (p->Next == NULL) return NULL;

	return p;
}

int InsertBef(char thisname[], char thissurname[], int x, char name[], char surname[], Position p) {
	Position q, prev;

	prev = SearchBef(name, surname, p);

	if (prev == NULL) {
		printf("\nnije pronadena osoba");
		return -1;
	}

	q = initialization();

	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->year = x;

	q->Next = prev->Next;
	prev->Next = q;
		
	return 0;

}

int InsertAft(char thisname[], char thissurname[], int x, char name[], char surname[], Position p) {
	Position q, temp;

	temp = Search(name, surname, p);

	if (temp == NULL) {
		printf("\nnije pronadena osoba");
		return -1;
	}

	q = initialization();

	if (q == 0) {
		printf("\nalokacija pogresna");
		return -1;
	}

	strcpy(q->name, thisname);
	strcpy(q->surname, thissurname);
	q->year = x;

	q->Next = temp->Next;
	temp->Next = q;

	return 0;

}

int InFile(Position p) {
	FILE* fp;
	fp = fopen("zad.txt", "w");

	if (fp == NULL) {
		printf("greska pri otvaranju file-a");
		exit(EXIT_FAILURE);
	}

	while (p != NULL)
	{
		fprintf(fp, "%s %s %d\n", p->name, p->surname, p->year);
		p = p->Next;
	}

	fclose(fp);
	return 0;
}

int FromFile(Position p) {
	char name[50], surname[50];
	int year;

	FILE* fp;
	fp = fopen("zad.txt", "r");

	if (fp == NULL) {
		printf("greska pri otvaranju file-a");
		exit(EXIT_FAILURE);
	}

	while (fscanf(fp, "%s %s %d", name, surname, &year) == 3)
	{
		InsertE(name, surname, year, p);
	}

	fclose(fp);
	return 0;
}

int ClearAll(Position p) {
	Position temp;

	while (p->Next != NULL) {
		temp = p->Next;
		p->Next = temp->Next;
		free(temp);
	}
	return 0;
}

int Sort(Position p) {
	Position j, prev_j,temp, end;

	end = NULL;
	while (p->Next != end) {
		prev_j = p;
		j = p->Next;
		
		while (j->Next != end) {
			if (strcmp(j->surname, j->Next->surname) > 0 || (strcmp(j->surname, j->Next->surname) == 0 && strcmp(j->name, j->Next->name) > 0)) {
				temp = j->Next;
				prev_j->Next = temp;
				j->Next = temp->Next;
				temp->Next = j;

				j = temp;
			}

			prev_j = j;
			j = j->Next;
		}
		end = j;
	}
	return 0;

}

int InsertPerson(char name[], char surname[], int* year) {
	printf("\nUnesi ime, prezime i godinu (ili 'kraj' za prekid): ");

	if (scanf("%49s", name) != 1)
		return 0; // greška u unosu

	if (strcmp(name, "kraj") == 0)
		return 0; // korisnik želi prekinuti

	if (scanf("%49s %d", surname, year) != 2) {
		printf("Neispravan unos! Pokušaj ponovo.\n");
		while (getchar() != '\n'); // oèisti ulazni buffer
		return -1;
	}

	return 1; // sve ok
}

// tribalo bi free-at memoriju prije return-a, pogledaj di mozes jos oslobidit pokazivac tempa da ne visi u memoriji kad brises
//ako pises adresu &head u argument, onda u funciji treba bit q = head->Next, a ne head = head->Next jer se gubi head onda, treba nova varijabla za to







