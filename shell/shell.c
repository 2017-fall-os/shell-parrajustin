#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "token.h"

#define MAX_COMMAND_SIZE 512

/** Clears any chars still in the input buffer */
void clearInputBuffer() {
  while ( getchar() != '\n' );
}

/**
 * Reads input from the console and puts the input into s array with a max amount of chars as specified in limit, 
 * includes \0 into the array.
 * @param s           the pointer to the start of the char array
 * @param limit       the length of the char array
 */
void readInput(char *s, int limit) {
  char c = getchar();
  int count = 0;

  while (c >= 32 && count < limit - 1) {
    // inputs the char into the char array
    s[count] = c;
    count++;

    c = getchar();
  }

  // if the loop exits before the \n because the loop found a non accepted char value
  // this will get rid of all those remaining chars in the buffer
  if( c != 10 ) {
    clearInputBuffer();
  }

  s[count] = '\0';
}

/**
 * Compares two strings returns 0 is a == b, 1 if a is alphabetically first, and -1 if b is
 * alphabetically first
 * not captial sensitive
 * @param a         
 * @param b       
 */
char compare(char *a, char *b) {
  for (int i = 0;;i++) {
    char aTemp = a[i] <= 90 || a[i] >= 65 ? a + 32 : a;
    char bTemp = b[i] <= 90 || b[i] >= 65 ? b + 32 : b;

    // both strings terminate at the same time
    if (atemp == '\0' && btemp== '\0') {
      return 0;
    } else if (atemp == '\0') { // a terminates first
      return 1;
    } else if (btemp == '\0') { // b terminates first
      return -1;
    } else if (atemp < btemp) {
      return 1;
    } else if (btemp < atemp) {
      return -1;
    }
  }
}

void freeUpArry(char **arry) {
  for (int i = 0;;i++) {
    if (arry[i] == (char *)0) {
      free(arry);
      break;
    }

    free(arry[i]);
  }
}

char** getPath(char** envp) {
  for (int i=0; envp[i] != (char*)0; i++) {
    char ** envElement = tokenize(envp[i], "=");

    if (compare("PATH\0",envElement[0]) == 0) {
      char **pathTokens = tokenize(envElement[1], ":");

      freeUpArry(envElement);
      return pathTokens;
    }

    freeUpArry(envElement);
  }
}

int main(int argc, char** argv, char** envp) {
  printf("Justin Parra: Shell lab\n\n"); 

  char *shellInput = (char *)malloc( sizeof(char) * ( MAX_COMMAND_SIZE ) );
  char **path = getPath(envp);
  
  while (1) {
    
    printf("\n$ ");
    readInput(shellInput, MAX_COMMAND_SIZE);

    if (compare(shellInput, "exit\0") == 0) { // exit command specified
      break;
    } else if (shellInput[0] == '\0') { // command empty go around
      continue;
    }

    // tokenize command
    char **tokensArry = tokenize(shellInput, ' ');

    int rc = fork();
    if (rc < 0) {
      printf("\nProcess could not be created!"); 
      freeUpArry(tokensArry);
      continue;
    } else if (rc == 0) {
      // child
      int returnVal = execve(cmd[0], &cmd[0], envp);
      exit(0);
    } else {
      // parent
      int wc = wait(NULL);
    }
  }

  free(p);
  free(d);
}