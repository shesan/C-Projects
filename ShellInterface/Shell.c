/**
 * Simple shell interface starter kit program.
 * Operating System Concepts
 * Project 1
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
    char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;

    char tmp[MAX_LINE];
    char history[MAX_LINE];
 
    pid_t cpid;

    int i;
    int top;
    int status;
    int length;
    int start;
    int args_index;
    int in;
    int out;

    char *path;

    history[0] = '\0';



    while (should_run){
        top = 0;
        start = -1;
        args_index = 0;
        in = 0;
        out = 0;

        do {
            printf("mysh:~$ ");
            fflush(stdout);
            // fgets(tmp, sizeof tmp, stdin)
            length = read(STDIN_FILENO, tmp, MAX_LINE);
        }
        while (tmp[0] == '\n');

        if (length == 0){
            exit(0);
        }

        if (strncmp(tmp, "exit", 4) == 0){
            should_run = 0;
            return 0;
        }
        else if (strncmp(tmp, "!!", 2) == 0){
            if (history[0] == '\0'){
                printf("No commands in history.\n");
            }
            else{
                strcpy(tmp, history);
                length = strlen(tmp) + 1;
                printf(tmp);
            }
        }
        else{
            strcpy(history, tmp);
        }

        for (i = 0; i < length; i++){
            switch(tmp[i]){
                case ' ': //empty or space

                case '\t': //tab
                    if (start != -1){
                        args[args_index] = &tmp[start];
                        args_index++;
                    }
                    tmp[i] = '\0';
                    start = -1;
                    break;

                case '\n': //newline (end of line)
                    if (start != -1){
                        args[args_index] = &tmp[start];
                        args_index++;
                    }
                    tmp[i] = '\0';
                    args[args_index] = NULL;
                    break;

                default:
                    if (start == -1){
                        start = i;
                    }
                    if (tmp[i] == '&'){
                        top = 1;
                        tmp[i-1] = '\0';
                    }
                    else if (tmp[i] == '<'){
                        in = i;
                    }
                    else if (tmp[i] == '>'){
                        out = i;
                    }

            }
        }

        if (top){
            args[--args_index] = NULL;
        }

        if (strncmp(tmp, "cd", 2) == 0){
            if (args[1] == '\0'){
                path = getenv("HOME");
            }
            else {
                path = args[1];
            }
            chdir(path);
            // printf("cd ");
        }
        else{

            if (in){
                // freopen(args[2], 'r', stdin);
                // printf("dqwed13de1331");

            }
            else if (out){
                // out = open(args[2], O_WRONLY | O_CREAT, 0777);
                // printf(out);
            }



            cpid = fork();
            if (cpid == 0){
                if (out){
                    int file = open(args[2], O_WRONLY | O_CREAT, 0777);
                    for (i = out; i > 2; i++){
                        args[i] = '\0';
                    }
                    if (file == -1){
                        return 0;
                    }
                    int file2 = dup2(file, 1);
  
                }

            execvp(args[0], args);
            exit(0);
                

            }
            if (!top) {
                wait(NULL);
            }
        }



        /**
          * After reading user input, the steps are:
          * (1) fork a child process
          * (2) the child process will invoke execvp()
          * (3) if command includes &, parent and child will run concurrently
          */
        //   should_run = 0;
    }


    return 0;
}

