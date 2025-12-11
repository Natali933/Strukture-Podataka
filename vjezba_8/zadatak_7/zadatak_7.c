#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

struct _directory;
typedef struct _directory* dirPosition;
typedef struct _directory {
    char name[MAX];
    dirPosition next;
    dirPosition subDirPosition;
} directory;

struct _stack;
typedef struct _stack* stackPosition;
typedef struct _stack {
    stackPosition next;
    dirPosition dirLevel;
} stack;

// Deklaracije funkcija
int push(stackPosition headStack, dirPosition dir);
dirPosition pop(stackPosition headStack);
int makeDirectory(stackPosition headStack, char* dirName);
int changeDirectory(stackPosition headStack, char* dirName);
int changeToParent(stackPosition headStack, dirPosition rootDir);
int printCurrentDirectory(stackPosition headStack);
int printAllDirectories(dirPosition current, int level);
int freeMemory(dirPosition current);
void printCurrentPath(stackPosition headStack);

int main() {
    directory headDir = { .name = "", .next = NULL, .subDirPosition = NULL };
    stack headStack = { .next = NULL, .dirLevel = NULL };

    // Inicijalizacija root direktorija C:/
    dirPosition rootDir = (dirPosition)malloc(sizeof(directory));
    if (!rootDir) {
        printf("Greska pri alociranju memorije!\n");
        return -1;
    }

    strcpy(rootDir->name, "C:");
    rootDir->next = NULL;
    rootDir->subDirPosition = NULL;

    headDir.next = rootDir;

    // Stavljanje root direktorija na stack
    push(&headStack, rootDir);

    char command[MAX];
    char argument[MAX];
    char buffer[200];

    printf("DOS Simulator direktorija\n");
    printf("Dostupne naredbe: md <ime>, cd <ime>, cd.., dir, exit, tree\n\n");

    while (1) {
        printCurrentPath(&headStack);
        printf("> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Greska pri unosu!\n");
            continue;
        }

        // Ukloni \n sa kraja
        buffer[strcspn(buffer, "\n")] = 0;

        // Parsiraj naredbu
        int numArgs = sscanf(buffer, "%s %s", command, argument);

        if (numArgs < 1) {
            continue; // Prazan unos
        }

        // Pretvori naredbu u mala slova za usporedbu
        for (int i = 0; command[i]; i++) {
            if (command[i] >= 'A' && command[i] <= 'Z') {
                command[i] = command[i] + 32;
            }
        }

        if (strcmp(command, "md") == 0 || strcmp(command, "mkdir") == 0) {
            if (numArgs < 2) {
                printf("Sintaksa: md <ime_direktorija>\n");
            }
            else {
                if (makeDirectory(&headStack, argument) == 0) {
                    printf("Direktorij '%s' je uspjesno kreiran.\n", argument);
                }
                else {
                    printf("Greska pri kreiranju direktorija!\n");
                }
            }
        }
        else if (strcmp(command, "cd") == 0) {
            if (numArgs < 2) {
                printf("Sintaksa: cd <ime_direktorija>\n");
            }
            else {
                if (changeDirectory(&headStack, argument) == 0) {
                    // Uspjesno
                }
                else {
                    printf("Direktorij '%s' ne postoji!\n", argument);
                }
            }
        }
        else if (strcmp(command, "cd..") == 0) {
            if (changeToParent(&headStack, rootDir) != 0) {
                printf("Vec ste u root direktoriju!\n");
            }
        }
        else if (strcmp(command, "dir") == 0) {
            printCurrentDirectory(&headStack);
        }
        else if (strcmp(command, "tree") == 0) {
            printf("\nStabla struktura svih direktorija:\n");
            printf("==================================\n");
            printAllDirectories(rootDir, 0);
            printf("==================================\n");
        }
        else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
            printf("Oslobadjanje memorije i izlaz...\n");
            freeMemory(rootDir);
            while (headStack.next != NULL) {
                pop(&headStack);
            }
            return 0;
        }
        else {
            printf("Nepoznata naredba: %s\n", command);
            printf("Dostupne naredbe: md, cd, cd.., dir, tree, exit\n");
        }

        printf("\n");
    }

    return 0;
}

void printCurrentPath(stackPosition headStack) {
    if (!headStack || !headStack->next) {
        printf("(prazno)");
        return;
    }

    // Privremeni stack za ispis putanje
    stackPosition tempStack = (stackPosition)malloc(sizeof(stack));
    if (!tempStack) {
        printf("Greska pri alociranju memorije!\n");
        return;
    }
    tempStack->next = NULL;

    stackPosition current = headStack->next;
    while (current != NULL) {
        stackPosition temp = (stackPosition)malloc(sizeof(stack));
        if (!temp) {
            // Oslobodi vec alocirane elemente
            while (tempStack->next != NULL) {
                stackPosition toFree = tempStack->next;
                tempStack->next = toFree->next;
                free(toFree);
            }
            free(tempStack);
            printf("Greska pri alociranju memorije!\n");
            return;
        }
        temp->dirLevel = current->dirLevel;
        temp->next = tempStack->next;
        tempStack->next = temp;
        current = current->next;
    }

    current = tempStack->next;
    while (current != NULL) {
        printf("%s", current->dirLevel->name);
        if (current->next != NULL) {
            printf("/");
        }
        current = current->next;
    }

    // Oslobodi privremeni stack
    while (tempStack->next != NULL) {
        stackPosition toFree = tempStack->next;
        tempStack->next = toFree->next;
        free(toFree);
    }
    free(tempStack);
}

int push(stackPosition headStack, dirPosition dir) {
    stackPosition newElement = (stackPosition)malloc(sizeof(stack));
    if (!newElement) {
        printf("Greska pri alociranju memorije za stack!\n");
        return -1;
    }

    newElement->dirLevel = dir;
    newElement->next = headStack->next;
    headStack->next = newElement;

    return 0;
}

dirPosition pop(stackPosition headStack) {
    if (headStack->next == NULL) {
        return NULL;
    }

    stackPosition temp = headStack->next;
    dirPosition dir = temp->dirLevel;
    headStack->next = temp->next;
    free(temp);

    return dir;
}

int makeDirectory(stackPosition headStack, char* dirName) {
    if (!headStack->next) {
        printf("Greska: Nema trenutnog direktorija!\n");
        return -1;
    }

    // Alokacija novog direktorija
    dirPosition newDir = (dirPosition)malloc(sizeof(directory));
    if (!newDir) {
        printf("Greska pri alociranju memorije!\n");
        return -1;
    }

    strcpy(newDir->name, dirName);
    newDir->next = NULL;
    newDir->subDirPosition = NULL;

    // Uzimamo trenutni direktorij (parent)
    dirPosition parentDir = headStack->next->dirLevel;

    // Dodavanje novog direktorija kao djeteta parent-a
    newDir->next = parentDir->subDirPosition;
    parentDir->subDirPosition = newDir;

    return 0;
}

int changeDirectory(stackPosition headStack, char* dirName) {
    if (!headStack->next) {
        return -1;
    }

    dirPosition temp = headStack->next->dirLevel->subDirPosition;

    // Trazenje direktorija sa tim imenom
    while (temp != NULL) {
        if (strcmp(temp->name, dirName) == 0) {
            push(headStack, temp);
            return 0;
        }
        temp = temp->next;
    }

    return -1; // Direktorij nije pronaden
}

int changeToParent(stackPosition headStack, dirPosition rootDir) {
    if (!headStack->next) {
        return -1;
    }

    // Provjera jesmo li u root direktoriju
    if (headStack->next->dirLevel == rootDir) {
        return -1; // Vec smo u root-u
    }

    pop(headStack);
    return 0;
}

int printCurrentDirectory(stackPosition headStack) {
    if (!headStack || !headStack->next) {
        printf("Greska: Nema trenutnog direktorija!\n");
        return -1;
    }

    dirPosition current = headStack->next->dirLevel;
    dirPosition child = current->subDirPosition;

    printf("\nSadrzaj direktorija %s:\n", current->name);
    printf("------------------------\n");

    if (child == NULL) {
        printf("  (prazan direktorij)\n");
    }
    else {
        while (child != NULL) {
            printf("  [DIR]  %s\n", child->name);
            child = child->next;
        }
    }

    printf("------------------------\n");

    return 0;
}

int printAllDirectories(dirPosition current, int level) {
    if (current == NULL) {
        return 0;
    }

    // Ispis trenutnog direktorija sa uvlacenjem
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    if (level == 0) {
        printf("%s (ROOT)\n", current->name);
    }
    else {
        printf("|-- %s\n", current->name);
    }

    // Rekurzivni ispis poddirektorija (djece)
    dirPosition child = current->subDirPosition;
    while (child != NULL) {
        printAllDirectories(child, level + 1);
        child = child->next;
    }

    return 0;
}

int freeMemory(dirPosition current) {
    if (current == NULL) {
        return 0;
    }

    // Rekurzivno brisanje poddirektorija
    dirPosition temp = current->subDirPosition;
    while (temp != NULL) {
        dirPosition toDelete = temp;
        temp = temp->next;
        freeMemory(toDelete);
    }

    free(current);
    return 0;
}

