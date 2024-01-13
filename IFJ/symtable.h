//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef SYM_H
#define SYM_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "scanner.h"
#include <stdbool.h>

/**
 * TNode is a structure containing the important information about variable
 */
typedef struct tnode{
    mystring *content; // variable content
    struct tnode *leftPtr; // pointer for its left child
    struct tnode *rightPtr; // pointer for its right child
    int type; // type of variable
    bool declared; // bool if variable was declared
    int height;
}TNode;

/**
 * TRoot is symtable for variables
 */
typedef struct troot{
    TNode *rootPtr; // pointer on specific tree
}TRoot;

/**
 * List for arguments of function
 */
typedef struct linkedList{
    int data_type;
    char *name;
    char *identifier;
    struct linkedList *next;
}argList;

/**
 * TNodef is a structure containing the important information about function
 */
typedef struct tnodef{
    mystring *content; // function name
    struct tnodef *leftPtr; // pointer on its left child in BST
    struct tnodef *rightPtr; // pointer on its right pointer in BST
    int parameters; // number of parameters
    int return_type; // what type function should return
    int height;
    argList *arguments; // list of arguments
}TNodef;

/**
 * TRootf is symtable for functions
 */
typedef struct trootf{
    TNodef *rootPtr;
}TRootf;




/**
 * @brief Find the maximum of two integers
 *
 * @param a The first integer
 * @param b The second integer
 * @return The maximum of the two integers
 */
int max(int a, int b);

/**
 * @brief Get the height of a node in the AVL tree
 *
 * @param node Pointer to the node
 * @return The height of the node, or 0 if the node is NULL
 */
int height(TNode *node);

/**
 * @brief Get the balance factor of a node in the AVL tree
 *
 * @param node Pointer to the node
 * @return The balance factor of the node, or 0 if the node is NULL
 */
int getBalance(TNode *node);

/**
 * @brief Perform a left rotation on the AVL tree
 *
 * @param y The node to be rotated
 * @return The new root of the rotated subtree
 */
TNode *leftRotate(TNode *y);

/**
 * @brief Perform a right rotation on the AVL tree
 *
 * @param x The node to be rotated
 * @return The new root of the rotated subtree
 */
TNode *rightRotate(TNode *x);

/**
 * @brief Perform a left-right rotation on the AVL tree
 *
 * @param z The node to be rotated
 * @return The new root of the rotated subtree
 */
TNode *leftRightRotate(TNode *z);

/**
 * @brief Perform a right-left rotation on the AVL tree
 *
 * @param z The node to be rotated
 * @return The new root of the rotated subtree
 */
TNode *rightLeftRotate(TNode *z);

/**
 * @brief Initialize the Binary Search Tree for Variables
 *
 * @param SymTable Pointer to the root of the Binary Search Tree
 */
void BVSInit(TRoot *SymTable);

/**
 * @brief Insert a new node with a token into the Binary Search Tree for Variables
 *
 * @param root Pointer to the root of the Binary Search Tree
 * @param token The token to be inserted into the tree
 * @return The updated root pointer after insertion
 */
TNode *BVSInsert(TNode *root, token token);

/**
 * @brief Search for a node with a specific token in the Binary Search Tree for Variable
 *
 * @param rootPtr Pointer to the root of the Binary Search Tree
 * @param token The token to be searched for in the tree
 * @return Pointer to the node containing the token, or NULL if not found
 */
TNode *BVSSearch(TNode *rootPtr, token token);

/**
 * @brief Update the variable type of a node in the Binary Search Tree for Variables
 *
 * @param rootPtr Pointer to the node in the Binary Search Tree
 * @param type The new variable type to be assigned
 */
void updateBVS(TNode *rootPtr, int type, bool qw);

/**
 * @brief Recursively dispose of nodes in the Binary Search Tree for Variables
 *
 * @param rootPtr Pointer to the root of the Binary Search Tree
 */
void BVSDisposeNode(TNode *rootPtr);

/**
 * @brief Dispose of the entire Binary Search Tree for Variables
 *
 * @param SymTable Pointer to the root of the Binary Search Tree
 */
void BVSDispose(TRoot *SymTable);

/**
 * @brief Get the height of a node in the AVL tree for functions
 *
 * @param node Pointer to the node
 * @return The height of the node, or 0 if the node is NULL
 */
int height_function(TNodef *node);

/**
 * @brief Get the balance factor of a node in the AVL tree for functions
 *
 * @param node Pointer to the node
 * @return The balance factor of the node, or 0 if the node is NULL
 */
int getBalance_function(TNodef *node);

/**
 * @brief Perform a left rotation on the AVL tree for functions
 *
 * @param y The node to be rotated
 * @return The new root of the rotated subtree
 */
TNodef *leftRotate_function(TNodef *y);

/**
 * @brief Perform a right rotation on the AVL tree for functions
 *
 * @param x The node to be rotated
 * @return The new root of the rotated subtree
 */
TNodef *rightRotate_function(TNodef *x);

/**
 * @brief Perform a left-right rotation on the AVL tree for functions
 *
 * @param z The node to be rotated
 * @return The new root of the rotated subtree
 */
TNodef *leftRightRotate_function(TNodef *z);

/**
 * @brief Perform a right-left rotation on the AVL tree for functions
 *
 * @param z The node to be rotated
 * @return The new root of the rotated subtree
 */
TNodef *rightLeftRotate_function(TNodef *z);

/**
 * @brief Initialize the Binary Search Tree for Functions
 *
 * @param SymTable Pointer to the root of the Binary Search Tree
 */
void BVSInit_function(TRootf *SymTable);

/**
 * @brief Insert a new node with function information into the Binary Search Tree for Functions
 *
 * @param rootPtr Pointer to the root of the Binary Search Tree
 * @param content The name of the function
 * @param parameters The number of parameters of the function
 * @param return_type The return type of the function
 * @param arguments The linked list of function arguments
 * @return The updated root pointer after insertion
 */
TNodef *BVSInsert_function(TNodef *rootPtr, mystring content, int parameters, int return_type, argList *arguments);

/**
 * @brief Search for a node with a specific token in the Binary Search Tree for Functions
 *
 * @param rootPtr Pointer to the root of the Binary Search Tree
 * @param token The token to be searched for in the tree
 * @return Pointer to the node containing the token, or NULL if not found
 */
TNodef *BVSSearch_function(TNodef *rootPtr, token token);


void printBST(TNode *root);
void printFunctionBST(TNodef *root);
// ----------------------------------------------------------------------------------
argList* createNode();
argList* addNode(argList *head, int data, char *name, char *identifier);


#endif