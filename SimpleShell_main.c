/**************************************************************
*
* Name:: Cole Chiodo
* GitHub-Name:: colechiodo
* Project:: Simple Shell
*
* File:: Chiodo_Cole_HW3_main.c
*
* Description:: This program is a simple shell that will take
* in commands from the user and execute them. The shell will
* also take in a file as an argument and execute the commands.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const int INPUT_SIZE = 177;

int main(int argc, char *argv[]){
    //if a prefix is given in the argv array, use that prefix.
    //otherwise, use the default
    char* prefix;
    if(argc >= 2){
        prefix = argv[1];
    } else{
        prefix = "> ";
    }

    char* input = malloc(INPUT_SIZE);
    
    //we want to know if we are going to read from a file or from manual input
    //if file name is given as argument, we will read from the file, else we will read from stdin
    FILE *inputMethod = stdin;
    for(int i = 1; i < argc; i++){
        if(strstr(argv[i], ".txt") != NULL){
            inputMethod = fopen(argv[i], "r");
            if(inputMethod == NULL){
                printf("Error opening file\n");
                return 2;
            }
        }
    }

    while(!feof(inputMethod)){
        //if input is null, prompt the user again so we can actually run the program
        printf("%s", prefix);
        fflush(stdout);
        fgets(input, INPUT_SIZE, inputMethod);

        while(input[0] == '\n' && !feof(inputMethod)){
            printf("Error! Empty line.\nPlease try again.\n%s", prefix);
            fflush(stdout);
            fgets(input, INPUT_SIZE, inputMethod);
        }

        input[strcspn(input, "\n")] = 0;

        if(strcmp(input, "exit") == 0)
            break;

        //implements the cd command.
        //if the command is cd, run the chdir function
        if(strncmp(input, "cd ", 3) == 0) {
            char *path = input + 3;

            if(chdir(path) != 0) {
                perror("cd");
            }
            continue;
        }

        //parse the input for the pipe character and place it into an array for execution later
        //this will allow us to execute the command with the arguments after
        char* inputp[INPUT_SIZE];
        int i = 0;
        inputp[i] = strtok(input, "|");
        while(inputp[i]){
            i++;
            inputp[i] = strtok(NULL, "|");
        }

        //pipe here, take the result of the first command and pass it to the second command.
        //continue to do so while there are more commands to execute
        int pipefd[2];
        int prev_pipe = 0;

        for(int j = 0; j < i; j++){
            pipe(pipefd);

            //parse the pipe by whitespace and place it into an array for execution later
            //this will allow us to execute the program with the parameters after
            char* inputv[INPUT_SIZE];
            int k = 0;
            inputv[k] = strtok(inputp[j], " ");
            while(inputv[k]){
                k++;
                inputv[k] = strtok(NULL, " ");
            }

            //fork here. if this is the parent process, wait for the child to finish. if this is the child, execute the command
            //so the parent process will wait for the child to die before it continues
            int childPid = fork();
            switch(childPid){
                case -1:
                    //error
                    printf("Error forking\n");
                    return 1;
                case 0:
                    //child: executes the command and dies
                    if(j < i - 1){
                        dup2(pipefd[1], 1);
                    }
                    if(prev_pipe != 0){
                        dup2(prev_pipe, 0);
                    }
                    close(pipefd[0]);
                    int err = execvp(inputv[0], inputv);
                    if(err == -1){
                        printf("Error executing command\n");
                        return 3;
                    }
                default:
                    //parent: waits for the child to die to continue running
                    int wstatus;
                    wait(&wstatus);
                    printf("Child %d exited with %d\n", childPid, WEXITSTATUS(wstatus));
                    close(pipefd[1]);
                    prev_pipe = pipefd[0];
                    break;
            }
        }
    }


    free(input);
    input = NULL;

    return 0;
}