//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//         author: Adam Nieslanik(xniesl00)                     //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "tokenqueue.h"


void initializeQueue(TokenQueue *queue) {
    queue->front = NULL;
    queue->active = NULL;
    queue->last = NULL;
}

void enqueue(TokenQueue *queue, token *data) {
    // allocate memory for a new node
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error allocating memory for queue node.\n");
        exit(INTERNAL_ERROR);
    }

    // copy token content and type to the new node
    newNode->data.content.str = strdup(data->content.str);
    newNode->data.content.str->str = strdup(data->content.str->str);
    newNode->data.type = data->type;

    newNode->next = NULL;

    // check if the queue is empty
    if (queue->front == NULL) {
        queue->front = newNode;
        newNode->prev = NULL;
        queue->last = newNode;
        queue->last->prev = NULL;
    } else {
        queue->last->next = newNode;
        newNode->prev = queue->last;
        queue->last = newNode;
    }
}

// Return token from the queue's front
token getFront(TokenQueue *queue)
{
    if (queue->front != NULL) {
        return queue->front->data; // Return token
    }
}

// Pop token from queue's front
token dequeue(TokenQueue *queue) {
    if (queue->front != NULL) {   
        QueueNode *temp = queue->active;
        token data = temp->data;
        
        // Just one element in queue
        if (queue->front == queue->last) {
            queue->front = NULL;
            queue->last = NULL;
        } else {
            // Move activity and front to next element
            if(queue->active == queue->front)
            {
                queue->active = queue->active->next;
                queue->front = queue->active;
            }
            else{ // Active element is last element
                if(queue->active == queue->last)
                {
                    // Skip last element
                    queue->last = queue->active->prev;
                    queue->active = queue->front;
                    
                    // Set front to active position
                    if (queue->active != NULL) {
                        queue->active->prev = NULL;
                        queue->front = queue->active;
                    }
                    else {
                        queue->front = NULL;
                    }
                }
                else{
                    // Link elements in 2 ways
                    queue->active->prev->next = queue->active->next;
                    queue->active->next->prev = queue->active->prev;
                    queue->active = queue->active->next;
                }
            }
        }
        free(temp); // Free allocated memory
        return data; // Return token
    }
}

// Checks if queue is empty
int isEmpty(TokenQueue *queue) {
    return queue->front == NULL;
}

// Set activity to function token
void setActiveFunction(TokenQueue *queue)
{
    // Queue is not empty
    if(queue->front != NULL)
    {
        // Start from the front
        queue->active = queue->front;
        // While there is still next element
        while(queue->active->next != NULL)
        {
            // Set activity to function token
            if(queue->active->data.type == KEYWORD_FUNC)
            {
                break;
            }
            else{ // Continue through queue
                queue->active = queue->active->next;
            }

            // If we reached end of queue, move to the front
            if(queue->active == queue->last)
            {
                queue->active = queue->front;
                break;
            }
        }
    }
}
