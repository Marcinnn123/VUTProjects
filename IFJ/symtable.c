//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"
#include "str.h"
#include "symtable.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(TNode *node) {
    // If the node is NULL, return 0; otherwise, return the height of the node
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(TNode *node) {
    // If the node is NULL, return 0; otherwise, return the difference in heights between left and right subtrees
    if (node == NULL)
        return 0;
    return height(node->leftPtr) - height(node->rightPtr);
}

TNode *leftRotate(TNode *y) {
    // Set x to be the right child of y, and T2 to be the left child of x
    TNode *x = y->rightPtr;
    TNode *T2 = x->leftPtr;

    // Perform the left rotation
    x->leftPtr = y;
    y->rightPtr = T2;

    // Update heights of y and x after the rotation
    y->height = 1 + max(height(y->leftPtr), height(y->rightPtr));
    x->height = 1 + max(height(x->leftPtr), height(x->rightPtr));

    // Return the new root of the rotated subtree (x)
    return x;
}

TNode *rightRotate(TNode *x) {
    // Set y to be the left child of x, and T2 to be the right child of y
    TNode *y = x->leftPtr;
    TNode *T2 = y->rightPtr;

    // Perform the right rotation
    y->rightPtr = x;
    x->leftPtr = T2;

    // Update heights of x and y after the rotation
    x->height = 1 + max(height(x->leftPtr), height(x->rightPtr));
    y->height = 1 + max(height(y->leftPtr), height(y->rightPtr));

    // Return the new root of the rotated subtree (y)
    return y;
}

TNode *leftRightRotate(TNode *z) {
    // Perform left rotation on the left child of z and then right rotation on z
    z->leftPtr = leftRotate(z->leftPtr);
    return rightRotate(z);
}

TNode *rightLeftRotate(TNode *z) {
    // Perform right rotation on the right child of z and then left rotation on z
    z->rightPtr = rightRotate(z->rightPtr);
    return leftRotate(z);
}

void BVSInit(TRoot *Symtable)
{
    // Initialize the root pointer of the Binary Search Tree for Variables to NULL
    Symtable->rootPtr = NULL;
}

TNode *BVSInsert(TNode *rootPtr, token token){
    // Temporary pointer 
    TNode *tmp = rootPtr;

    // Allocate memory for a new node and its string structure
    TNode *insert = malloc((sizeof (TNode)));
    mystring *insertStr = malloc(sizeof (mystring));

    // Check if memory allocation was successful
    if(insert == NULL || insertStr == NULL)
    {
        exit(INTERNAL_ERROR);
    }

    // Initialize the new node
    insert->leftPtr = NULL;
    insert->rightPtr = NULL;

    // Initialize the content of the new node with information from the token
    insert->content = insertStr;
    insert->content = strdup(token.content.str);
    insert->content->str = strdup(token.content.str->str);
    insert->content->symbol = token.content.str->symbol;
    insert->content->variable_type = token.content.str->variable_type;

    // Set the type of the variable to KEYWORD_NIL as it is not defined yet
    insert->type = KEYWORD_NIL;

    // If the tree is empty, the new node becomes the root
    if (!tmp) {
        rootPtr = insert;
        return rootPtr;
    } else {
        // Insert the node in the correct position in the Binary Search Tree
        if ((mystrCompare(token.content.str, rootPtr->content)) < 0) {
            rootPtr->leftPtr =  BVSInsert(rootPtr->leftPtr, token);
            return rootPtr;
        } else if ((mystrCompare(token.content.str, rootPtr->content)) > 0) {
            rootPtr->rightPtr =  BVSInsert(rootPtr->rightPtr, token);
            return rootPtr;
        }
    }

    // Update the height and check for imbalance in the tree
    rootPtr->height = 1 + max(height(rootPtr->leftPtr), height(rootPtr->rightPtr));
    int balance = getBalance(rootPtr);

    // Left Heavy
    if (balance > 1) {
        if (mystrCompare(token.content.str, rootPtr->leftPtr->content) < 0) {
            // Left-Left case
            return rightRotate(rootPtr);
        } else {
            // Left-Right case
            rootPtr->leftPtr = leftRotate(rootPtr->leftPtr);
            return rightRotate(rootPtr);
        }
    }

    // Return the updated root pointer
    return rootPtr;
}

TNode *BVSSearch(TNode *rootPtr, token token){
    // If the root is NULL, the variable is not found
    if(rootPtr == NULL){
        return NULL;
    }
    else{
        // If the variable is found, return the node
        if((mystrCompare(token.content.str, rootPtr->content)) == 0){
            return rootPtr;
        }
        // If the ASCII value is smaller, go to the left child
        else if((mystrCompare(token.content.str, rootPtr->content)) < 0){
            return BVSSearch(rootPtr->leftPtr, token);
        }
        // If the ASCII value is bigger, go to the right child
        else if((mystrCompare(token.content.str, rootPtr->content)) > 0){
            return BVSSearch(rootPtr->rightPtr, token);
        }
        // If not found, return NULL
        return NULL;
    }
}

void updateBVS(TNode *rootPtr, int type, bool qw)
{
    // Update the variable type in the content of the node
    rootPtr->content->variable_type = type;
    rootPtr->content->qw = qw;
    return;
}

void BVSDisposeNode(TNode *rootPtr){
    // Recursively dispose of nodes in the Binary Search Tree
    if(rootPtr != NULL){
        BVSDisposeNode(rootPtr->leftPtr);
        BVSDisposeNode(rootPtr->rightPtr);
        rootPtr = NULL;
    }
    return;
}

void BVSDispose(TRoot *SymTable){
    // Dispose of the entire Binary Search Tree for Variables
    BVSDisposeNode(SymTable->rootPtr);
    SymTable = NULL;
    return;
}

// works same as in variable, just returns TNodef
int height_function(TNodef *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance_function(TNodef *node) {
    if (node == NULL)
        return 0;
    return height_function(node->leftPtr) - height_function(node->rightPtr);
}

TNodef *leftRotate_function(TNodef *y) {
    TNodef *x = y->rightPtr;
    TNodef *T2 = x->leftPtr;

    x->leftPtr = y;
    y->rightPtr = T2;

    y->height = 1 + max(height_function(y->leftPtr), height_function(y->rightPtr));
    x->height = 1 + max(height_function(x->leftPtr), height_function(x->rightPtr));

    return x;
}

TNodef *rightRotate_function(TNodef *x) {
    TNodef *y = x->leftPtr;
    TNodef *T2 = y->rightPtr;

    y->rightPtr = x;
    x->leftPtr = T2;

    x->height = 1 + max(height_function(x->leftPtr), height_function(x->rightPtr));
    y->height = 1 + max(height_function(y->leftPtr), height_function(y->rightPtr));

    return y;
}

TNodef *leftRightRotate_function(TNodef *z) {
    z->leftPtr = leftRotate_function(z->leftPtr);
    return rightRotate_function(z);
}

TNodef *rightLeftRotate_function(TNodef *z) {
    z->rightPtr = rightRotate_function(z->rightPtr);
    return leftRotate_function(z);
}
void BVSInit_function(TRootf *SymTable){
    SymTable->rootPtr = NULL;
}

TNodef *BVSInsert_function(TNodef *rootPtr, mystring content, int parameters, int return_type, argList *arguments){
    TNodef *tmp = rootPtr;
    TNodef *insert = malloc((sizeof (TNode)));
    mystring *insertStr = malloc(sizeof (mystring));
    insert->leftPtr = NULL;
    insert->rightPtr = NULL;
    insert->height = 1;
    insert->content = insertStr;
    mystrCopy(insert->content, &content);
    insert->return_type = return_type;
    insert->parameters = parameters;
    insert->arguments = arguments;
    if(!tmp){
        rootPtr = insert;
        return rootPtr;
    }
    else {
        if ((mystrCompare(&content, rootPtr->content)) < 0) {
            rootPtr->leftPtr = BVSInsert_function(rootPtr->leftPtr, content, parameters, return_type, arguments);
        } else if ((mystrCompare(&content,rootPtr->content)) > 0) {
            rootPtr->rightPtr =  BVSInsert_function(rootPtr->rightPtr, content, parameters, return_type, arguments);
        }
    }

    rootPtr->height = 1 + max(height_function(rootPtr->leftPtr), height_function(rootPtr->rightPtr));
    int balance = getBalance_function(rootPtr);

    // Left Heavy
    if (balance > 1) {
        if (mystrCompare(&content, rootPtr->leftPtr->content) < 0) {
            // Left-Left case
            return rightRotate_function(rootPtr);
        } else {
            // Left-Right case
            rootPtr->leftPtr = leftRotate_function(rootPtr->leftPtr);
            return rightRotate_function(rootPtr);
        }
    }

    // Right Heavy
    if (balance < -1) {
        if (mystrCompare(&content, rootPtr->rightPtr->content) > 0) {
            // Right-Right case
            return leftRotate_function(rootPtr);
        } else {
            // Right-Left case
            rootPtr->rightPtr = rightRotate_function(rootPtr->rightPtr);
            return leftRotate_function(rootPtr);
        }
    }

    return rootPtr;
}

TNodef *BVSSearch_function(TNodef *rootPtr, token token) {
    if (rootPtr == NULL) {
        return NULL;
    } else {
        if ((mystrCompare(token.content.str, rootPtr->content)) == 0) {
            return rootPtr;
        }
        else if ((mystrCompare(token.content.str, rootPtr->content)) < 0) {
            return BVSSearch_function(rootPtr->leftPtr, token);
        } else if ((mystrCompare(token.content.str, rootPtr->content)) > 0) {
            return BVSSearch_function(rootPtr->rightPtr, token);
        }
    }
    return NULL;
}

void printBST(TNode *root) {
    if (root == NULL) {
        return;
    }

    // In-order traversal (print in ascending order)
    printBST(root->leftPtr);

    // Print the content and other information from the node
    printf("Content: %s, Type: %d, Declared: %s\n", root->content->str, root->type, root->declared ? "Yes" : "No");

    // Recursive call for right subtree
    printBST(root->rightPtr);
}

void printFunctionBST(TNodef *root) {
    
    if (root == NULL) {
        return;
    }
    // In-order traversal (print in ascending order)
    printFunctionBST(root->leftPtr);

    // Recursive call for right subtree
    printFunctionBST(root->rightPtr);

    // Print the function name, parameters, and return type
    printf("Function Name: %s, Parameters: %d, Return Type: %d\n", root->content->str, root->parameters, root->return_type);  
}


argList* createNode(){
    argList* temp = malloc((sizeof(argList)));
    temp->next = NULL;// make next point to NULL
    return temp;//return the new node
}

argList* addNode(argList *head, int data, char *name, char *identifier){
    argList* temp;
    argList* p;
    temp = createNode();//createNode will return a new node with data = value and next pointing to NULL.
    temp->data_type = data; // add element's value to data part of node
    temp->name = name;
    temp->identifier = identifier;
    if(head == NULL){
        head = temp;     //when linked list is empty
    }
    else{
        p  = head;//assign head to p
        while(p->next != NULL){
            p = p->next;//traverse the list until p is the last node.The last node always points to NULL.
        }
        p->next = temp;//Point the previous last node to the new node created.
    }
    return head;
}
