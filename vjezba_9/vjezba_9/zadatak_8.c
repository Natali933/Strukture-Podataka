#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



typedef struct tree {
    int value;
    struct tree* left;
    struct tree* right;
} Tree;
typedef Tree* TreePos;

TreePos insertTree(TreePos root, int value);
TreePos deleteTree(TreePos root, int value);
TreePos findMin(TreePos root);
TreePos findElement(TreePos root, int value);
void inorderTraversal(TreePos root);
void preorderTraversal(TreePos root);
void postorderTraversal(TreePos root);
void levelOrderTraversal(TreePos root);
void printLevel(TreePos root, int level);
int getHeight(TreePos root);
TreePos freeTree(TreePos root);
void printMenu();


int main() {
    TreePos root = NULL;
    int choice, value;
    TreePos result;

    while (1) {
        printMenu();
        printf("Unesite izbor: ");
        if (scanf("%d", &choice) != 1) {
            printf("Greška pri uèitavanju! Pokušajte ponovno.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
        case 1:
            printf("Unesite vrijednost za unos: ");
            if (scanf("%d", &value) != 1) {
                printf("Greška pri uèitavanju vrijednosti!\n");
                while (getchar() != '\n');
                break;
            }
            root = insertTree(root, value);
            printf("Element %d dodan u stablo.\n", value);
            break;

        case 2:
            printf("Unesite vrijednost za brisanje: ");
            if (scanf("%d", &value) != 1) {
                printf("Greška pri uèitavanju vrijednosti!\n");
                while (getchar() != '\n'); 
                break;
            }
            root = deleteTree(root, value);
            printf("Element %d obrisan iz stabla.\n", value);
            break;

        case 3:
            printf("Unesite vrijednost za pretragu: ");
            if (scanf("%d", &value) != 1) {
                printf("Greška pri uèitavanju vrijednosti!\n");
                while (getchar() != '\n'); 
                break;
            }
            result = findElement(root, value);
            if (result != NULL)
                printf("Element %d pronaðen u stablu.\n", value);
            else
                printf("Element %d nije pronaðen u stablu.\n", value);
            break;

        case 4:
            printf("Inorder ispis: ");
            inorderTraversal(root);
            printf("\n");
            break;

        case 5:
            printf("Preorder ispis: ");
            preorderTraversal(root);
            printf("\n");
            break;

        case 6:
            printf("Postorder ispis: ");
            postorderTraversal(root);
            printf("\n");
            break;

        case 7:
            printf("Level order ispis:\n");
            levelOrderTraversal(root);
            printf("\n");
            break;

        case 8:
            root = freeTree(root);
            printf("Stablo obrisano. Izlaz iz programa.\n");
            return 0;

        default:
            printf("Neispravan izbor! Pokušajte ponovno.\n");
        }
        printf("\n");
    }

    return 0;
}


void printMenu() {
    printf("\n=== BINARNO STABLO PRETRAZIVANJA ===\n");
    printf("1. Unos elementa\n");
    printf("2. Brisanje elementa\n");
    printf("3. Pronalazenje elementa\n");
    printf("4. Inorder ispis\n");
    printf("5. Preorder ispis\n");
    printf("6. Postorder ispis\n");
    printf("7. Level order ispis\n");
    printf("8. Izlaz\n");
    printf("====================================\n");
}


TreePos insertTree(TreePos root, int value) {

    if (root == NULL) {
        root = (TreePos)malloc(sizeof(Tree));
        if (root == NULL) {
            printf("Greška pri alokaciji memorije!\n");
            return NULL;
        }
        root->value = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

 
    if (value < root->value) {
        root->left = insertTree(root->left, value);
    }
 
    else if (value > root->value) {
        root->right = insertTree(root->right, value);
    }
   

    return root;
}

TreePos findMin(TreePos root) {
    if (root == NULL)
        return NULL;

    if (root->left == NULL)
        return root;

    return findMin(root->left);
}


TreePos deleteTree(TreePos root, int value) {
    TreePos temp;

    
    if (root == NULL) {
        printf("Element nije pronaðen.\n");
        return NULL;
    }

 
    if (value < root->value) {
        root->left = deleteTree(root->left, value);
    }
    
    else if (value > root->value) {
        root->right = deleteTree(root->right, value);
    }
    
    else {
       
        if (root->left != NULL && root->right != NULL) {
            temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteTree(root->right, temp->value);
        }
   
        else {
            temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            free(temp);
        }
    }

    return root;
}


TreePos findElement(TreePos root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->value)
        return findElement(root->left, value);
    else if (value > root->value)
        return findElement(root->right, value);
    else
        return root; 
}


void inorderTraversal(TreePos root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->value);
        inorderTraversal(root->right);
    }
}

void preorderTraversal(TreePos root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(TreePos root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->value);
    }
}


int getHeight(TreePos root) {
    if (root == NULL)
        return 0;

    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}


void printLevel(TreePos root, int level) {
    if (root == NULL)
        return;

    if (level == 1)
        printf("%d ", root->value);
    else if (level > 1) {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
}


void levelOrderTraversal(TreePos root) {
    int height = getHeight(root);

    for (int i = 1; i <= height; i++) {
        printf("Nivo %d: ", i);
        printLevel(root, i);
        printf("\n");
    }
}


TreePos freeTree(TreePos root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return NULL;
}

