#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_LINE 1024

typedef struct city* TreePos;
typedef struct city {
	char name[MAX_NAME];
	TreePos left;
	TreePos right;
	int population;
}City;

typedef struct countries* Position;
typedef struct countries {
	char name[MAX_NAME];
	Position next;
	TreePos root;
}Countries;

TreePos insertCityIntoTree(TreePos, char*, int);
int loadCountries(Position, char*);
void printTree(TreePos);
void printCountriesAndCities(Position);
void searchTree(TreePos, int);
void searchCitiesInCountry(Position);
TreePos freeCityTree(TreePos);
int freeCountryList(Position);

int main() {
	Position head = (Position)malloc(sizeof(Countries));
	if (head == NULL) {
		printf("greska pri alokaciji\n");
		return 1;
	}

	strcpy(head->name, "");
	head->root = NULL;
	head->next = NULL;

	if (!loadCountries(head, "drzava.txt")) {
		free(head);
		return 1;
	}
	printCountriesAndCities(head);

	printf("\nPRETRAGA GRADOVA PO DRZAVI:\n");
	searchCitiesInCountry(head);

	printf("\nBRISANJE MEMORIJE I IZLAZ\n");
	freeCountryList(head);

	return 0;
}

TreePos insertCityIntoTree(TreePos root, char* cityName, int population) {
	if (root == NULL) {
		TreePos newNode = (TreePos)malloc(sizeof(City));
		if (newNode == NULL) {
			printf("Greška pri alokaciji memorije!\n");
			return NULL;
		}

		strcpy(newNode->name, cityName);
		newNode->population = population;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	if (root->population > population) {
		root->left = insertCityIntoTree(root->left, cityName, population);
	}

	else if(root->population < population) {
		root->right = insertCityIntoTree(root->right, cityName, population);
	}

	else {
		if (strcmp(cityName, root->name) < 0) {
			root->left = insertCityIntoTree(root->left, cityName, population);
		}
		else if (strcmp(cityName, root->name) > 0) {
			root->right = insertCityIntoTree(root->right, cityName, population);
		}
	}
	return root;
}

int loadCountries(Position head, char* filename) {
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

			Position p = head;
			Position newEl = NULL;

			while (p->next != NULL && strcmp(p->next->name, countryName) < 0) {
				p = p->next; 
			}

			if (p->next != NULL && strcmp(p->next->name, countryName) == 0) {
				printf("Drzava %s vec postoji\n", countryName);
				continue;
			}

			newEl = (Position)malloc(sizeof(Countries));
			if (newEl == NULL) {
				printf("Greska pri alokaciji\n");
				continue;
			}

			strcpy(newEl->name, countryName);
			newEl->root = NULL;
			newEl->next = p->next;
			p->next = newEl;

			FILE* cityFile = fopen(fileName, "r");
			if (cityFile == NULL) {
				printf("ne mogu otvoriti %s\n", fileName);
				continue;
			}

			char cityBuffer[MAX_LINE];
			char cityName[MAX_NAME];
			int population;

			while (fgets(cityBuffer, MAX_LINE, cityFile) != NULL) {
				memset(cityName, 0, MAX_NAME);

				if (sscanf(cityBuffer, "%99s %d", cityName, &population) == 2) {
					cityName[MAX_NAME - 1] = '\0';
					newEl->root = insertCityIntoTree(newEl->root, cityName, population);
				}
			}
			fclose(cityFile);
		}
	}
	fclose(fp);
	return 1;
}

void printTree(TreePos root) {
	if (root != NULL) {
		printTree(root->left);
		printf("  %s-%d stanovnika\n", root->name, root->population);
		printTree(root->right);
	}
}

void printCountriesAndCities(Position head) {
	Position p = head->next;

	printf("\nDRZAVE I GRADOVI:\n");

	while (p != NULL) {
		printf("\n%s:\n", p->name);

		if (p->root == NULL) {
			printf("nema gradova\n");
		}
		else {
			printTree(p->root);
		}
		p = p->next;
	}
}

void searchTree(TreePos root, int population) {
	if (root == NULL) {
		return;
	}

	searchTree(root->right, population);

	if (root->population > population) {
		printf("%s-%d stanovnika\n", root->name, root->population);
	}

	searchTree(root->left, population);
}

void searchCitiesInCountry(Position head) {
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

	Position p = head->next;
	int found = 0;

	while (p != NULL) {
		if (strcmp(p->name, countryName) == 0) {
			found = 1;
			printf("\nGradovi u drzavi %s sa vise od %d stanovnika:\n", countryName, minPopulation);

			if (p->root == NULL) {
				printf("nema gradova\n");
				return;
			}
			else {
				 searchTree(p->root, minPopulation);
			}
			break;
		}
		p = p->next;
	}
	if (!found) {
		printf("drzava %s ne postoji!\n", countryName);
	}
}

TreePos freeCityTree(TreePos root) {
	if (root != NULL) {
		freeCityTree(root->left);
		freeCityTree(root->right);
		free(root);
	}
	return NULL;
}

int freeCountryList(Position head) {
	Position temp;
	Position p = head->next;

	while (p != NULL) {
		temp = p;
		p = p->next;

		freeCityTree(temp->root);
		free(temp);
	}
	free(head);
	return 0;
}
