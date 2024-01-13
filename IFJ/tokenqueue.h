//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//         author: Adam Nieslanik (xniesl00)                    //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef TOKEN_QUEUE_H
#define TOKEN_QUEUE_H

#include "scanner.h"

/**
 * Structure definition for a node in the queue
 */
typedef struct QueueNode {
    token data;                  // Token data in the node
    struct QueueNode *next;      // Pointer to the next node in the queue
    struct QueueNode *prev;      // Pointer to the previous node in the queue
} QueueNode;

/**
 * Structure definition for the token queue
 */
typedef struct {
    QueueNode *front;            // Pointer to the front of the queue
    QueueNode *active;           // Pointer to the currently active node in the queue
    QueueNode *last;             // Pointer to the last node in the queue
} TokenQueue;

/**
 * @brief Initialize the token queue
 *
 * This function initializes the token queue by setting pointers to NULL
 *
 * @param queue Pointer to the TokenQueue structure
 */
void initializeQueue(TokenQueue *queue);

/**
 * @brief Add a token to the end of the queue
 *
 * This function adds a token to the end of the queue
 *
 * @param queue Pointer to the TokenQueue structure
 * @param data Pointer to the token to be added
 */
void enqueue(TokenQueue *queue, token *data);

/**
 * @brief Remove a token from the front of the queue
 *
 * This function removes a token from the front of the queue and returns it
 *
 * @param queue Pointer to the TokenQueue structure
 * @return Token removed from the front of the queue
 */
token dequeue(TokenQueue *queue);

/**
 * @brief Get the token at the front of the queue without removing it
 *
 * This function returns the token at the front of the queue without removing it
 *
 * @param queue Pointer to the TokenQueue structure
 * @return Token at the front of the queue
 */
token getFront(TokenQueue *queue);

/**
 * @brief Check if the queue is empty
 *
 * This function checks if the token queue is empty
 *
 * @param queue Pointer to the TokenQueue structure
 * @return 1 if the queue is empty, 0 otherwise
 */
int isEmpty(TokenQueue *queue);

/**
 * @brief Sets activity to function token
 * @param queue Pointer to the TokenQueue structure
 * @return 1 if the queue is empty, 0 otherwise
 */
void setActiveFunction(TokenQueue *queue);





#endif // TOKEN_QUEUE_H
