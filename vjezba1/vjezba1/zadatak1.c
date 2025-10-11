#define _CRT_SECURE_NO_WARNINGS
#define MAX 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
	char ime[50];
	char prezime[50];
	int bodovi;
}Student;

int howMany(const char*namefile){ //funkcija za ukupan broj studenata
	int i = 0; //brojac
	char buffer[200]; //za spremit linije koje su procitane u file-u
	FILE * fp;
	fp = fopen(namefile, "r");
	
	if (fp == NULL) {
		printf("greska pri otvaranju datoteke u funkciji howMany!\n");
		return 0;
	}
	else {
		while(fgets(buffer, sizeof(buffer), fp) != NULL){ 
			i++;
		}
	}

	fclose(fp);
	
	return i;
}

Student* scanFile(const char*namefile, int count) { //alociranje i spremanje
	FILE* fp;
	fp = fopen(namefile, "r");
	if (fp == NULL)
		printf("greska pri otvaranju datoteke u funkciji scanFile!\n");

	Student* s= (Student*)malloc(count * sizeof(Student)); //alocira memoriju 
	if (s == NULL) {
		printf("greska pri alokaciji memorije!\n");
		return NULL;
	}
	
	for (int i = 0; i < count; i++) { //sprema
		int provjera = fscanf(fp, "%s %s %d", s[i].ime, s[i].prezime, &s[i].bodovi);
		if (provjera != 3) //provjerava jesu li uhvacena sva tri elementa
			printf("greska u %d retku\n", i+1);
	}

	fclose(fp);
	return s;
}

void printStudents(Student* s, int count) { //printanje rezultata
	printf("ime\tprezime\tbodovi\trelativni\n");
	for (int i = 0; i < count; i++) {
		double relativni = (((double)s[i].bodovi / MAX) * 100);
		printf("%s\t%s\t%d\t%.2f\n", s[i].ime, s[i].prezime, s[i].bodovi, relativni);
	}
}



int main() {
	int numberOfStudents = howMany("primjer.txt");
	if (numberOfStudents == 0) {
		printf("greska u funkciji howMany!");
	}

	Student* x = scanFile("primjer.txt", numberOfStudents);
	
	printStudents(x, numberOfStudents);
	free(x);

	return 0;
}