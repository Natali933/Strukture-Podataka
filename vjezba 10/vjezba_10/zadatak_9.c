#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 90
#define MIN 10

typedef struct tree* TreePos;
typedef struct tree {
	int value;
	TreePos left;
	TreePos right;
}Tree;

TreePos insert(int, TreePos);
int replace(TreePos);
TreePos deleteTree(TreePos);
TreePos insertNorm(int, TreePos);
void inorderPrint(TreePos, FILE*);
void printTree(const char*, TreePos);

int main() {
	srand(time(NULL));
	TreePos root = NULL;
	
	root = insert(2, root);
	root = insert(5, root);
	root = insert(7, root);
	root = insert(8, root);
	root = insert(11, root);
	root = insert(1, root);
	root = insert(4, root);
	root = insert(2, root);
	root = insert(3, root);
	root = insert(7, root); 

	printTree("firstTree.txt", root);

	replace(root);

	printTree("secondTree.txt", root);

	root = deleteTree(root);

	for (int i = 0; i < 10; i++) {
		int random = rand() % (MAX - MIN + 1) + MIN;
		root = insertNorm(random, root);
		printf("ubaèen novi broj: %d\n", random);
	}

	printTree("thirdTree.txt", root);

}

TreePos insert(int x, TreePos root) {
	if (root == NULL) {
		root = (TreePos)malloc(sizeof(Tree));
		if (root == NULL) {
			printf("alokacija nije uspjela");
			return NULL;
		}

		root->value = x;
		root->left = NULL;
		root->right = NULL;
	}

	else if (x > root->value) {
		root->left = insert(x, root->left);
	}

	else if (x < root->value){
		root->right = insert(x, root->right);
	}
	else {
		root->right = insert(x, root->right); 
	}

	return root;
}

int replace(TreePos root) {
	if (root == NULL) {
		return 0;
	}

	int prevVal = root->value;
	root->value = replace(root->left) + replace(root->right);
	return prevVal + root->value;
}

TreePos deleteTree(TreePos root) {
	if (root == NULL) {
		return NULL;
	}

	else {
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}

	return NULL;
}

TreePos insertNorm(int x, TreePos root) {

	if (root == NULL) {
		root = (TreePos)malloc(sizeof(Tree));
		if (root == NULL) {
			printf("greška pri alokaciji");
			return NULL;
		}

		root->value = x;
		root->left = NULL;
		root->right = NULL;
	}

	else if (x < root->value) {
		root->left = insertNorm(x, root->left);
	}

	else if (x > root->value) {
		root->right = insertNorm(x, root->right);
	}
		
	return root;
}

void inorderPrint(TreePos root, FILE* fp) {
	if (root != NULL) {
		inorderPrint(root->left, fp);
		fprintf(fp, "%d ", root->value);
		inorderPrint(root->right, fp);
	}
}

void printTree(const char* filename, TreePos root) {
	FILE* fp = fopen(filename, "w");
	if (!fp) {
		printf("Nemoguæe otvoriti datoteku %s\n", filename);
		return;
	}

	inorderPrint(root, fp);
	fclose(fp);
}

