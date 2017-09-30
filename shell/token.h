/**
 * This Header file is based on code provided by Dr. Freudenthal
 */
#ifndef MYTOC_H_INCLUDED
#define MYTOC_H_INCLUDED

/**
 * Token generator that "return(s) a
 * freshly allocated null-terminated array of pointers to freshly
 * allocated strings containing each "token" from the input string"
 */
char ** tokenize(char *str, char delim);

/** counts the number of tokens in an array **/
int countTokens(char **tokenArry);

/** instead of just counting the number of tokens in an arry this actually counts seperated words **/
int countTokenWords(char *str, char delim);

/** frees up all memory associated with a token arry **/
void freeUpArry(char **arry);

#endif // BTREE_H_INCLUDED
