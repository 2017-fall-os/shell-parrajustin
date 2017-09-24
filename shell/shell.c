#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
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
 * @param a         
 * @param b       
 */
char compare(char *a, char *b) {
  for (int i = 0;;i++) {
    char aTemp = (char) a[i];
    char bTemp = (char) b[i];

    // printf("\n%d %d", a[i], b[i]);

    // both strings terminate at the same time
    if (aTemp == '\0' && bTemp== '\0') {
      return 0;
    } else if (aTemp == '\0') { // a terminates first
      return 1;
    } else if (bTemp == '\0') { // b terminates first
      return -1;
    } else if (aTemp < bTemp) {
      return 1;
    } else if (bTemp < aTemp) {
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
    char ** envElement = tokenize(envp[i], '=');

    if (compare("PATH\0", envElement[0]) == 0) {
      char **pathTokens = tokenize(envElement[1], ':');

      freeUpArry(envElement);
      return pathTokens;
    }

    freeUpArry(envElement);
  }
}

int lengthStr(char *a) {
  for (int i = 0;;i++) {
    if (a[i] == 0 || a[i] == '\0') { 
      return i;
    }
  }
}

char* concat(char *a, char *b) {
  int sizeA = lengthStr(a);
  int sizeB = lengthStr(b);

  char *returnee = (char *)malloc(sizeof(char) * (sizeA + sizeB + 1 + 1)); // first + 1 for the '/' char and other + 1 for null terminator
  int i;
  for (i = 0; i < sizeA; i++) {
    returnee[i] = a[i];
  }

  returnee[i] = '/';
  i++;

  for (int l = 0; l < sizeB; l++, i++) {
    returnee[i] = b[l];
  }

  return returnee;
}

int countArry(char **a) {
  for(int i = 0;;i++) {
    if (a[i] == 0) {
      return i;
    }
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
      int returnVal = execve(countArry(tokensArry), tokensArry, envp);

      for (int i = 0; returnVal != 0 && path[i] != (char *)0; i++) {
        char * str = concat(path[i], tokensArry[0]);
        returnVal = execve(str, tokensArry, envp);
        free(str);
      }

      if (returnVal != 0) {
        printf("\nCommand not found");
      }

      return 0;
    } else {
      // parent
      int waitArg;
      wait(&waitArg);
      if (WEXITSTATUS(waitArg) != 0) {
        printf("\nError Code: %d\n", WEXITSTATUS(waitArg));
      }

    }
  }

  free(shellInput);
  freeUpArry(path);

  return 0;
}