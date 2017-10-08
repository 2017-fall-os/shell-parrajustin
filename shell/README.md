# Parra Justin Shell

Objective: make a shell similiar to that of bash, implement several commands such as pipes, redirecting outputs and inputs

- token library I created in token.c
- Shell main code in shell.c
  - I have comments for util, exe, and main
  - under the util comment exist most of the utilities to run the program like concating strings and their lengts
  - under exe comment exists code to execute the commands like pipes and redirecting inputs
  - under main comment is the main function

## my tests now show that it works with the test script

- problem
  - I didn't do redirect output


# Running

If the variable PS1 is empty than the shell will prompt $ for user input after which you may type in your command and push enter to send it. To exit the shell simply type "exit" and the shell with exit

```
make 
./myShell
```

or 

```
make run
```

# Test

To test just run

```
make test
```

piping help was found on http://tldp.org/LDP/lpg/node11.html
redirecting input / output found on https://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file
opening file help found on https://www.tutorialspoint.com/cprogramming/c_file_io.htm