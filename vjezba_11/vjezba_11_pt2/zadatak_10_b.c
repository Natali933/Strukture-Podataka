#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_LINE 1024

typedef struct city* Position;
typedef struct city {
	char name[MAX_NAME];
	int population;
	Position next;
}City;

typedef struct countries* TreePos;
typedef struct countries {
	char name[MAX_NAME];
	Position head;
	TreePos left;
	TreePos right;
}Countries;

Position insertCityIntoList(Position, char*, int);
TreePos insertCountryIntoTree(TreePos, char*, char*);
int loadCountries(TreePos*, char*);
void printList(Position);
void printCountriesAndCities(TreePos);
void searchList(Position, int);
void searchCitiesInCountry(TreePos);
int freeCityList(Position);
TreePos freeCountryTree(TreePos);

int main() {
	TreePos root = NULL;

	if (!loadCountries(&root, "drzava.txt")) {
		return 1;
	}

	printf("\nDRZAVE I GRADOVI:\n");
	printCountriesAndCities(root);

	printf("\nPRETRAGA GRADOVA PO DRZAVI:\n");
	searchCitiesInCountry(root);

	printf("\nBRISANJE MEMORIJE I IZLAZ\n");
	freeCountryTree(root);

	return 0;
}


Position insertCityIntoList(Position head, char* cityName, int population) {
	Position p = head;
	Position newEl = NULL;

	while (p->next != NULL && (p->next->population < population || (p->next->population == population && strcmp(p->next->name, cityName) < 0))) {
		p = p->next;
	}

	if (p->next != NULL && p->next->population == population && strcmp(p->next->name, cityName) == 0) {
		printf("grad %s vec postoji\n", cityName);
		return head;
	}

	newEl = (Position)malloc(sizeof(City));
	if (newEl == NULL) {
		printf("Greska pri alokaciji\n");
		return head;
	}

	strcpy(newEl->name, cityName);
	newEl->population = population;
	newEl->next = p->next;
	p->next = newEl;

	return head;
}

TreePos insertCountryIntoTree(TreePos root, char* countryName, char* fileName) {
	if (root == NULL) {
		TreePos newNode = (TreePos)malloc(sizeof(Countries));
		if (newNode == NULL) {
			printf("Greška pri alokaciji memorije!\n");
			return NULL;
		}

		strcpy(newNode->name, countryName);

		
		newNode->head = (Position)malloc(sizeof(City));
		if (newNode->head == NULL) {
			free(newNode);
			return NULL;
		}
		newNode->head->name[0] = '\0';
		newNode->head->population = 0;
		newNode->head->next = NULL;

		newNode->left = NULL;
		newNode->right = NULL;

		
		FILE* cityFile = fopen(fileName, "r");
		if (cityFile == NULL) {
			printf("ne mogu otvoriti %s\n", fileName);
			return newNode;
		}

		char cityBuffer[MAX_LINE];
		char cityName[MAX_NAME];
		int population;

		while (fgets(cityBuffer, MAX_LINE, cityFile) != NULL) {
			memset(cityName, 0, MAX_NAME);

			if (sscanf(cityBuffer, "%99s %d", cityName, &population) == 2) {
				cityName[MAX_NAME - 1] = '\0';
				insertCityIntoList(newNode->head, cityName, population);
			}
		}
		fclose(cityFile);

		return newNode;
	}

	
	if (strcmp(countryName, root->name) < 0) {
		root->left = insertCountryIntoTree(root->left, countryName, fileName);
	}
	else if (strcmp(countryName, root->name) > 0) {
		root->right = insertCountryIntoTree(root->right, countryName, fileName);
	}
	else {
		printf("Drzava %s vec postoji\n", countryName);
	}

	return root;
}

int loadCountries(TreePos* root, char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Greska: Ne mogu otvoriti: %s\n", filename);
		return 0;
	}

	char buffer[MAX_LINE];
	char countryName[MAX_NAME];
	char fileName[MAX_NAME];

	while (fgets(buffer, MAX_LINE, fp) != NULL) {
		memset(countryName, 0, MAX_NAME);
		memset(fileName, 0, MAX_NAME);

		if (sscanf(buffer, "%99s %99s", countryName, fileName) == 2) {
			countryName[MAX_NAME - 1] = '\0';
			fileName[MAX_NAME - 1] = '\0';

			*root = insertCountryIntoTree(*root, countryName, fileName);
		}
	}
	fclose(fp);
	return 1;
}

void printList(Position head) {
	Position p = head->next;

	if (p == NULL) {
		printf("nema gradova\n");
		return;
	}

	while (p != NULL) {
		printf("%s-%d stanovnika\n", p->name, p->population);
		p = p->next;
	}
}

void printCountriesAndCities(TreePos root) {
	if (root != NULL) {
		printCountriesAndCities(root->left);

		printf("\n%s:\n", root->name);
		printList(root->head);

		printCountriesAndCities(root->right);
	}
}

void searchList(Position head, int population) {
	Position p = head->next;

	while (p != NULL) {
		if (p->population > population) {
			printf("  %s-%d stanovnika\n", p->name, p->population);
		}
		p = p->next;
	}
}

void searchCitiesInCountry(TreePos root) {
	char countryName[MAX_NAME];
	int minPopulation;

	memset(countryName, 0, MAX_NAME);

	printf("\nUnesi naziv drzave: ");
	if (scanf("%99s", countryName) != 1) {
		printf("greska pri ucitavanju");
		while (getchar() != '\n');
		return;
	}
	countryName[MAX_NAME - 1] = '\0';

	printf("\nUnesi minimalan broj stanovnika: ");
	if (scanf("%d", &minPopulation) != 1) {
		printf("greska pri ucitavanju!\n");
		while (getchar() != '\n');
		return;
	}


	TreePos current = root;
	int found = 0;

	while (current != NULL) {
		int cmp = strcmp(countryName, current->name);

		if (cmp == 0) {
			found = 1;
			printf("\nGradovi u drzavi %s sa vise od %d stanovnika:\n", countryName, minPopulation);

			if (current->head->next == NULL) {
				printf("nema gradova\n");
			}
			else {
				searchList(current->head, minPopulation);
			}
			break;
		}
		else if (cmp < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	if (!found) {
		printf("drzava %s ne postoji!\n", countryName);
	}
}

int freeCityList(Position head) {
	Position temp;
	Position p = head;

	while (p->next != NULL) {
		temp = p->next;
		p->next = temp->next;
		free(temp);
	}

	return 0;
}

TreePos freeCountryTree(TreePos root) {
	if (root != NULL) {
		freeCountryTree(root->left);
		freeCountryTree(root->right);
		freeCityList(root->head);
		free(root);
	}
	return NULL;
}