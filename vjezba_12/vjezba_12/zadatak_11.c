//11. Prepraviti zadatak 10 na naèin da se formira hash tablica država. Tablica ima 11 mjesta, a
//funkcija za preslikavanje kljuè raèuna da se zbraja ASCII vrijednost prvih pet slova države zatim
//raèuna ostatak cjelobrojnog dijeljenja te vrijednosti s velièinom tablice.Države s istim kljuèem se
//pohranjuju u vezanu listu sortiranu po nazivu države.Svaki èvor vezane liste sadrži stablo
//gradova sortirano po broju stanovnika, zatim po nazivu grada.
/*10. Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_LINE 1024
#define HASH_SIZE 11

typedef struct City* CityPosition;
typedef struct Country* CountryPosition;
typedef struct HashTable* hashTab;

//grad-stablo
struct City {
	char name[MAX_NAME];
	int population;
	CityPosition Left;
	CityPosition Right;
};

//drzava-lista
struct Country {
	char name[MAX_NAME];
	int key;
	CityPosition cityTree; //korijen stabla gradova 
	CountryPosition Next;
};

struct HashTable {
	int size;
	CountryPosition* HashList; //niz pokazivaca na liste
};

hashTab InitHashTable(int);
int CalculateKey(const char* countryName, int tableSize);
CountryPosition CreateCountry(const char* name, int key);
CityPosition CreateCity(const char* name, int population);
CityPosition InsertCity(CityPosition root, const char* name, int population);
void InsertCountry(hashTab H, const char* name, const char* filename);
void LoadCities(CountryPosition country, const char* filename);
void PrintHashTable(hashTab H);
void PrintCityTree(CityPosition root);
void PrintCountryList(CountryPosition head);
void FreeHashTable(hashTab H);
void FreeCityTree(CityPosition root);
void FreeCountryList(CountryPosition head);
CountryPosition FindCountry(hashTab H, const char* name);

int main() {
	hashTab H;
	FILE* fp;
	char line[MAX_LINE];
	char countryName[MAX_NAME];
	char fileName[MAX_NAME];

	H = InitHashTable(HASH_SIZE);

	fp = fopen("drzave.txt", "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke drzave.txt\n");
		FreeHashTable(H);
		return -1;
	}

	while (fgets(line, MAX_LINE, fp) != NULL) {
		if (sscanf(line, "%s %s", countryName, fileName) == 2) {
			InsertCountry(H, countryName, fileName);
		}
	}
	fclose(fp);

	printf("TABLICA\n");
	PrintHashTable(H);

	printf("PRETRAGA\n");
	CountryPosition found = FindCountry(H, "Hrvatska");
	if (found != NULL) {
		printf("pronadena drzava: %s (kljuc: %d)\n", found->name, found->key);
		printf("Gradovi:\n");
		PrintCityTree(found->cityTree);
	}
	else {
		printf("nema te drzave\n");
	}

	FreeHashTable(H);

	return 0;
}

hashTab InitHashTable(int size) {
	hashTab H;
	int i;
	H = (hashTab)malloc(sizeof(struct HashTable));
	if (H == NULL) {
		printf("Greska pri alokaciji memorije za hash tablicu!\n");
		return NULL;
	}

	H->size = size;
	H->HashList = (CountryPosition*)malloc(sizeof(CountryPosition) * size);
	if (H->HashList == NULL) {
		printf("greska pri alokaciji memorije za hash listu\n");
		free(H);
		return NULL;
	}

	for (i = 0; i < size; i++) {
		H->HashList[i] = NULL;
	}

	return H;
}

int CalculateKey(const char* countryName, int tableSize) {
	int sum = 0;
	int len = strlen(countryName);
	int limit = (len < 5) ? len : 5;

	for (int i = 0; i < limit; i++) {
		sum += (unsigned char)countryName[i]; //da è æ š.. budu pozitivni u ASCII
	}

	return sum % tableSize;
}

//novi cvor drzave
CountryPosition CreateCountry(const char* name, int key) {
	CountryPosition newCountry;

	newCountry = (CountryPosition)malloc(sizeof(struct Country));
	if (newCountry == NULL) {
		printf("Greska pri alokaciji memorije za drzavu\n");
		return NULL;
	}

	strcpy(newCountry->name, name);
	newCountry->key = key;
	newCountry->cityTree = NULL;
	newCountry->Next = NULL;

	return newCountry;
}

//novi cvor grada
CityPosition CreateCity(const char* name, int population) {
	CityPosition newCity;

	newCity = (CityPosition)malloc(sizeof(struct City));
	if (newCity == NULL) {
		printf("greska pri alokaciji memorije za grad\n");
		return NULL;
	}

	strcpy(newCity->name, name);
	newCity->population = population;
	newCity->Left = NULL;
	newCity->Right = NULL;

	return newCity;
}

//grad u stablo sortirano, grad pa naziv
CityPosition InsertCity(CityPosition root, const char* name, int population) {
	if (root == NULL)
		return CreateCity(name, population);

	//po broju stanovnika
	if (population < root->population)
		root->Left = InsertCity(root->Left, name, population);
	else if (population > root->population)
		root->Right = InsertCity(root->Right, name, population);
	else {
		//po nazivu 
		int cmp = strcmp(name, root->name);
		if (cmp < 0) {
			root->Left = InsertCity(root->Left, name, population);
		}
		else if (cmp > 0) {
			root->Right = InsertCity(root->Right, name, population);
		}
	}
	return root;
}

//ucitavanje gradova u drzavu
void LoadCities(CountryPosition country, const char* filename) {
	char line[MAX_LINE];
	char cityName[MAX_NAME];
	int population;
	
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke %s\n", filename);
		return;
	}

	while (fgets(line, MAX_LINE, fp) != NULL) {
		if (sscanf(line, "%[^,], %d", cityName, &population) == 2) {//citaj sve osim zareza, preskoci zarez i razmak i %d cita broj
			country->cityTree = InsertCity(country->cityTree, cityName, population);
		} 
	}
	fclose(fp);
}

//drzava u hash tablicu
void InsertCountry(hashTab H, const char* name, const char* filename) {
	int key;
	CountryPosition newCountry, current, prev;

	key = CalculateKey(name, H->size);
	newCountry = CreateCountry(name, key);
	if (newCountry == NULL)
		return;

	//gradovi u ovu drzavu
	LoadCities(newCountry, filename);

	//umetanje u sortiranu lisu
	if (H->HashList[key] == NULL || strcmp(name, H->HashList[key]->name) < 0) {
		//na pocetak liste
		newCountry->Next = H->HashList[key];
		H->HashList[key] = newCountry;
	}
	else {
		//trazi poziciju
		prev = H->HashList[key];
		current = prev->Next;

		while (current != NULL && strcmp(name, current->name) > 0) {
			prev = current;
			current = current->Next;
		}

		newCountry->Next = current;
		prev->Next = newCountry;
	}
}

//ispis gradova - inorder stablo
void PrintCityTree(CityPosition root) {
	if (root != NULL) {
		PrintCityTree(root->Left);
		printf("%s-%d stanovnika\n", root->name, root->population);
		PrintCityTree(root->Right);
	}
}

//ispis drzava - lista
void PrintCountryList(CountryPosition head) {
	CountryPosition temp = head;

	while (temp != NULL) {
		printf("Drzava: %s (kljuc: %d)\n", temp->name, temp->key);
		printf("Gradovi:\n");
		PrintCityTree(temp->cityTree);
		printf("\n");
		temp = temp->Next;
	}
}

//ispis cijele hash tablice
void PrintHashTable(hashTab H) {
	int i;

	for (i = 0; i < H->size; i++) {
		printf("Pozicija %d:\n", i);
		if (H->HashList[i] != NULL) {
			PrintCountryList(H->HashList[i]);
		}
		else {
			printf("prazno\n\n");
		}
	}
}

//pretrazivanje drzave po imenu
CountryPosition FindCountry(hashTab H, const char* name) {
	int key = CalculateKey(name, H->size);
	CountryPosition temp = H->HashList[key];

	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) {
			return temp;
		}
		temp = temp->Next;
	}

	return NULL;
}

//oslobadanje stabla gradova
void FreeCityTree(CityPosition root) {
	if (root != NULL) {
		FreeCityTree(root->Left);
		FreeCityTree(root->Right);
		free(root);
	}
}

//oslobadanje liste drzava
void FreeCountryList(CountryPosition head) {
	CountryPosition temp;

	while (head != NULL) {
		temp = head;
		head = head->Next;
		FreeCityTree(temp->cityTree);
		free(temp);
	}
}

//oslobadanje hash tablice 
void FreeHashTable(hashTab H) {
	int i;

	if (H == NULL) {
		return;
	}

	for (i = 0; i < H->size; i++) {
		FreeCountryList(H->HashList[i]);
	}

	free(H->HashList);
	free(H);
}

// hash je kao niz i guess
// pretrazivanje je kratko
// node je povezan na kljuc tipa broj 2 sta je amo rec kljuc u hash tablici, odnosno poazivac na taj node
// Tipa prvo polje pokazuje na node francuska koji pokazuje na node Hrvatska
// kako program zna da je hrvatska na 1, H+r+v+a+t = 251 zbrojen ASSCI  prvih pet slova imena, 251%5 i to je 1 pa stvaljamo na 1 i tako za svaku drzvau pa program tako zna tocno na kojem 1,2,3 polju has tablice gledati i onda ulazi u listu koja je povazana na to
// prosti brojevi se koriste za hash tablice i dijeli se sa prostim brojem 5 jer su idealni za racunanje ostataka jer ostali mogu dat vise ostataka
// velicina tablice je 11
// liste za drzave, stabla za gradove
// 
// struct HashTable{
//	int size;
//	Position HashList //pokazivac na listu
// };
// 
// struct Country{
//	...
//	int key; //na temelju intigera znamo gdje u hashu pohranit državu
// };
// 
// int main(){
// -inicijalizirat hashTable
//		alokacija memorije
//		hashTable->HashList = (Position)malloc(sizeof(Position));
// 
//		for(int i = 0; i<hashTable->size; i++){
// 			hashTable->hashList[i] = NULL;
//		}
// 
// -èitanje iz datoteke "drzava.txt"
//		findKey(string cName){
//			return (cName[0] + cName[1] +...+ cName[4]) % hashTable->size
//		}
// 
// -ubacivanje elemenata
//		if(*(hashTable->hashList == NULL)){
//			dodaj novi element
//		}
//		else if( -||- != NULL && strcmp(<0)){
//			dodaj novi element
//		}
//		else{
//			while(-nije kraj liste i strcmp(>0)){
//				i++;
//				hashTable->hashList += i;
//			}
//			dodaj novi element
//		}
// 
// -dodavanje novog elementa
//		alokacija
//		strcpy
//		newEl->key = findKey(cName)
//		newEl->next = hashTable->HashList[findKey(cName)]
//		hashTable->HashList[findKey(cName)] = newEl
// 
// -pretraživanje 
//		for(int i = 0; i<hashTable->size; i++){
//			-poziv funkcije za pretragu lista
//			findCountry("head", population) // sada je "head" - hashTable->hashList[i];
//		}
// 
// -brisanje
//		for(int i = 0; i<hashTable->size; i++){
//			freeList("head") // sada je "head" - hashTable->hashList[i];
//		}
//}
//