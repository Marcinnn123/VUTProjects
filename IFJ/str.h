//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef MYSTRING_H
#define MYSTRING_H
#include <stdbool.h>

/**
 * @file mystring.h
 * @brief Header file for dynamic string handling.
 */

/**
 * @struct mystring
 * @brief Structure to represent a dynamic string.
 */
typedef struct mystring{
    char *str;      /**< Content of the string. */
    int length;     /**< Length of the string. */
    int capacity;   /**< Amount of memory allocated. */
    int symbol;    /**< Scope, that variable is visible in.*/
    int variable_type;
    bool qw;
} mystring;

/**
 * @brief Initialize a dynamic string.
 * @param s Pointer to a `mystring` structure to initialize.
 * @return 0 if successful, 1 if memory allocation fails.
 */
int mystrInit(mystring *s);

/**
 * @brief Free memory associated with a dynamic string.
 * @param s Pointer to a `mystring` structure to free.
 */
void mystrFree(mystring *s);

/**
 * @brief Clear the content of a dynamic string.
 * @param s Pointer to a `mystring` structure to clear.
 */
void mystrClear(mystring *s);

/**
 * @brief Append a character to the end of a dynamic string.
 * @param s Pointer to a `mystring` structure.
 * @param c The character to append.
 * @return 0 if successful, 1 if memory reallocation fails.
 */
int mystrAppendChar(mystring *s, char c);

/**
 * @brief Copy the content of one dynamic string to another.
 * @param dest Pointer to the destination `mystring` structure.
 * @param src Pointer to the source `mystring` structure.
 * @return 0 if successful, 1 if memory reallocation fails.
 */
int mystrCopy(mystring *dest, mystring *src);

/**
 * @brief Compare two dynamic strings.
 * @param s1 Pointer to the first `mystring` structure.
 * @param s2 Pointer to the second `mystring` structure.
 * @return An integer less than, equal to, or greater than zero if s1 is found,
 * respectively, to be less than, to match, or be greater than s2.
 */
int mystrCompare(mystring *s1, mystring *s2);

/**
 * @brief Compare a dynamic string with a constant string.
 * @param s1 Pointer to the `mystring` structure.
 * @param s2 Pointer to the constant string to compare.
 * @return An integer less than, equal to, or greater than zero if s1 is found,
 * respectively, to be less than, to match, or be greater than s2.
 */
int mystrCompareConst(mystring *s1, char *s2);

/**
 * @brief Get a pointer to the content of a dynamic string.
 * @param s Pointer to the `mystring` structure.
 * @return A pointer to the content of the dynamic string.
 */
char *mystrGetString(mystring *s);

/**
 * @brief Get the length of a dynamic string.
 * @param s Pointer to the `mystring` structure.
 * @return The length of the dynamic string.
 */
int mystrGetLength(mystring *s);

/**
 * @brief Remove the last character from a dynamic string.
 * @param s Pointer to a `mystring` structure.
 */
void mystrRemoveLastChar(mystring *s);

#endif