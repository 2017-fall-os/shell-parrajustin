#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <fcntl.h>
#include "token.h"

#define MAX_COMMAND_SIZE 512

char *prompt = "$ \0";

//                                                         
//                                                         
//                           tttt            iiii  lllllll 
//                        ttt:::t           i::::i l:::::l 
//                        t:::::t            iiii  l:::::l 
//                        t:::::t                  l:::::l 
//  uuuuuu    uuuuuuttttttt:::::ttttttt    iiiiiii  l::::l 
//  u::::u    u::::ut:::::::::::::::::t    i:::::i  l::::l 
//  u::::u    u::::ut:::::::::::::::::t     i::::i  l::::l 
//  u::::u    u::::utttttt:::::::tttttt     i::::i  l::::l 
//  u::::u    u::::u      t:::::t           i::::i  l::::l 
//  u::::u    u::::u      t:::::t           i::::i  l::::l 
//  u::::u    u::::u      t:::::t           i::::i  l::::l 
//  u:::::uuuu:::::u      t:::::t    tttttt i::::i  l::::l 
//  u:::::::::::::::uu    t::::::tttt:::::ti::::::il::::::l
//   u:::::::::::::::u    tt::::::::::::::ti::::::il::::::l
//    uu::::::::uu:::u      tt:::::::::::tti::::::il::::::l
//      uuuuuuuu  uuuu        ttttttttttt  iiiiiiiillllllll
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//            

/** Clears any chars still in the input buffer */
// void clearInputBuffer() {
//   while ( getchar() != '\n' );
// }

/**
 * Reads input from the console and puts the input into s array with a max amount of chars as specified in limit, 
 * includes \0 into the array.
 * @param s           the pointer to the start of the char array
 * @param limit       the length of the char array
 */
int readInput(char *s, int limit) {
  char c = getchar();
  int count = 0;

  while (c >= 32 && count < limit - 1) {
    // inputs the char into the char array
    s[count] = c;
    count++;

    c = getchar();
  }
  
  s[count] = '\0';

  if (c == -1 && count > 0) {
    return count * -1;
  } else if (c == -1 && count == 0) {
    return 0;
  }

  return 1;
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

int lengthStr(char *a) {
  for (int i = 0;;i++) {
    if (a[i] == 0 || a[i] == '\0' || a[i] == -1 || a[i] < 32) { 
      return i;
    }
  }
}

char** workEnvironment(char** envp) {
  char **path;
  for (int i=0; envp[i] != (char*)0; i++) {
    char ** envElement = tokenize(envp[i], '=');

    if (compare("PATH\0", envElement[0]) == 0) {
      path = tokenize(envElement[1], ':');
    }

    if (compare("PS1\0", envElement[0]) == 0) {
      int actual = countTokens(envElement);
      if (actual == 1) {
        prompt = "\0";
      } else {
        int len = lengthStr(envElement[1]);

        int i = 0;
        prompt = (char *)malloc(sizeof(char) * (len + 1));
        for (i = 0; i < len; i++) {
          prompt[i] = envElement[1][i];
        }
        prompt[i] = '\0';
      }
    }

    freeUpArry(envElement);
  }
  return path;
}

int contains(char *a, char token) {
  for (int i = 0; a[i] != '\0';i++) {
    if (a[i] == token) {
      return 1;
    }
  }
  return 0;
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
  returnee[i] = 0;

  return returnee;
}

//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//      eeeeeeeeeeee  xxxxxxx      xxxxxxx eeeeeeeeeeee    
//    ee::::::::::::ee x:::::x    x:::::xee::::::::::::ee  
//   e::::::eeeee:::::eex:::::x  x:::::xe::::::eeeee:::::ee
//  e::::::e     e:::::e x:::::xx:::::xe::::::e     e:::::e
//  e:::::::eeeee::::::e  x::::::::::x e:::::::eeeee::::::e
//  e:::::::::::::::::e    x::::::::x  e:::::::::::::::::e 
//  e::::::eeeeeeeeeee     x::::::::x  e::::::eeeeeeeeeee  
//  e:::::::e             x::::::::::x e:::::::e           
//  e::::::::e           x:::::xx:::::xe::::::::e          
//   e::::::::eeeeeeee  x:::::x  x:::::xe::::::::eeeeeeee  
//    ee:::::::::::::e x:::::x    x:::::xee:::::::::::::e  
//      eeeeeeeeeeeeeexxxxxxx      xxxxxxx eeeeeeeeeeeeee  
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         
//                                                         

int pathExecute(char **tokenizedCommand, char **envp, char **path) {
  int returnVal = execve(tokenizedCommand[0], tokenizedCommand, envp);
  
  for (int i = 0; returnVal != 0 && path[i] != (char *)0; i++) {
    char * str = concat(path[i], tokenizedCommand[0]);
    returnVal = execve(str, tokenizedCommand, envp);
    free(str);
  }

  return returnVal;
}

void execute(char *command, char **envp, char **path) {
  // check if there are pipes
  char **pipeTokenized = tokenize(command, '|');
  int numofPipes = countTokens(pipeTokenized);
  if (contains(command, '|') == 1 && countTokenWords(command, '|') > 0) {
    // there are pipes

    int fd[2];
    int copy_0 = dup(0);
    int copy_1 = dup(1);
    int i;
    for (i = 0; i < numofPipes - 1; i++) {

      int val = pipe(fd);

      pid_t pid = fork();
      if (pid < 0 || val != 0) {
        printf("pipe failed to start process");
        return;
      } else if (pid == 0) {
        // child process closes up input side of pipe
        // close(fd[1]); // child closing stdin
        close(1);
        int val = dup(fd[1]);
        close(fd[1]);
        close(fd[0]);

        execute(pipeTokenized[i], envp, path);
        return;
      } else {
        // parent, waits for command to end
        int waitArg;
        wait(&waitArg);
        if (WEXITSTATUS(waitArg) != 0) {
          printf("\nError Code: %d\n", WEXITSTATUS(waitArg));
        }

        close(0);
        int val = dup(fd[0]);
        close(fd[0]);
        close(fd[1]);
      }
    }

    execute(pipeTokenized[i], envp, path);

    dup2(copy_0, 0);
    dup2(copy_1, 1);
    freeUpArry(pipeTokenized);
    return;
  } else {
    // there are no pipes

    // now lets check if there are '>' or '<'
    if (contains(command, '<') || contains(command , '>')) {
      if (contains(command, '<')) {
        char **tokenizedInput = tokenize(command, '<');

        int copy_0 = dup(0);
        int copy_1 = dup(1);
        // if there is an input redirect
        pid_t pid = fork();
        if (pid < 0) {
          printf("\ninput redirect failed to start process");
          return;
        } else if (pid == 0) {
          char **removeSpaces = tokenize(tokenizedInput[1], ' ');
          int fd = open(removeSpaces[0], O_RDONLY);
          freeUpArry(removeSpaces);

          if (fd < 0) {
            printf("\nFailed to open file: %s", tokenizedInput[1]);
            freeUpArry(tokenizedInput);
            return;
          }

          int d = dup2(fd, 0); // make file go to input
          close(fd);     // fd no longer needed - the dup'ed handles are sufficient
  
          execute(tokenizedInput[0], envp, path);
          freeUpArry(tokenizedInput);
          return;
        } else {
          // parent, waits for command to end
          int waitArg;
          wait(&waitArg);
          if (WEXITSTATUS(waitArg) != 0) {
            printf("\nError Code: %d\n", WEXITSTATUS(waitArg));
          }
        }
        
        dup2(copy_0, 0);
        dup2(copy_1, 1);
        freeUpArry(tokenizedInput);
        return;
      } else {
        printf("\n for grad students");
        return;
      }

    } else {
      freeUpArry(pipeTokenized); // free pipe token array
      char **tokenizedCommand = tokenize(command, ' ');

      int returnVal = pathExecute(tokenizedCommand, envp, path);

      if (returnVal != 0) {
        printf("\nCommand not found");
      }
    
      freeUpArry(tokenizedCommand);
      return;
    }
  }
}
//                                                                     
//                                                                     
//                                              iiii                   
//                                             i::::i                  
//                                              iiii                   
//                                                                     
//     mmmmmmm    mmmmmmm     aaaaaaaaaaaaa   iiiiiiinnnn  nnnnnnnn    
//   mm:::::::m  m:::::::mm   a::::::::::::a  i:::::in:::nn::::::::nn  
//  m::::::::::mm::::::::::m  aaaaaaaaa:::::a  i::::in::::::::::::::nn 
//  m::::::::::::::::::::::m           a::::a  i::::inn:::::::::::::::n
//  m:::::mmm::::::mmm:::::m    aaaaaaa:::::a  i::::i  n:::::nnnn:::::n
//  m::::m   m::::m   m::::m  aa::::::::::::a  i::::i  n::::n    n::::n
//  m::::m   m::::m   m::::m a::::aaaa::::::a  i::::i  n::::n    n::::n
//  m::::m   m::::m   m::::ma::::a    a:::::a  i::::i  n::::n    n::::n
//  m::::m   m::::m   m::::ma::::a    a:::::a i::::::i n::::n    n::::n
//  m::::m   m::::m   m::::ma:::::aaaa::::::a i::::::i n::::n    n::::n
//  m::::m   m::::m   m::::m a::::::::::aa:::ai::::::i n::::n    n::::n
//  mmmmmm   mmmmmm   mmmmmm  aaaaaaaaaa  aaaaiiiiiiii nnnnnn    nnnnnn
//                                                                     
//                                                                     
//                                                                     
//                                                                     
//                                                                     
//                                                                     
//                                                                     
int main(int argc, char** argv, char** envp) {

  char *shellInput = (char *)malloc( sizeof(char) * ( MAX_COMMAND_SIZE ) );
  char **path = workEnvironment(envp);
  
  while (1) {
    
    // read input command
    printf("%s", prompt);
    int valid = readInput(shellInput, MAX_COMMAND_SIZE);

    // check if exit
    if (compare(shellInput, "exit\0") == 0 || valid == 0) { // exit command specified
      break;
    } else if (shellInput[0] == '\0') { // command empty go around
      continue;
    }

    char **backgroundTokenized = tokenize(shellInput, '&');
    int numberOfBackgroundTasks = countTokens(backgroundTokenized);
    if (contains(shellInput, '&') == 1) {
      // we have background tasks
      for (int i = 0; i < numberOfBackgroundTasks; i--) {
        int rc = fork();
        if (rc < 0) {
          printf("\nProcess could not be created!"); 
          freeUpArry(backgroundTokenized);
          break;
        } else if (rc == 0) {
          execute(backgroundTokenized[i], envp, path);
          return 0;
        }
      }

      freeUpArry(backgroundTokenized);
      continue;
    } else {
      // no background tasks
      freeUpArry(backgroundTokenized);
      
      // begin running it
      int rc = fork();
      if (rc < 0) {
        printf("\nProcess could not be created!"); 
        continue;
      } else if (rc == 0) {
        execute(shellInput, envp, path);
        return 0;
      } else {
        // parent, waits for command to end
        int waitArg;
        wait(&waitArg);
        if (WEXITSTATUS(waitArg) != 0) {
          printf("\nError Code: %d\n", WEXITSTATUS(waitArg));
        }
      }
    }

    if (valid < 0) {
      break;
    }

  }

  free(shellInput);
  freeUpArry(path);

  return 0;
}