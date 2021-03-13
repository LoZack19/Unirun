#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "unirun.h"

void empty(char* buffer, size_t size)
{
    for(int i = 0; i < size; ++i)
        buffer[i] = 0;
}

char* run(char* program, char* args[], int* status)
{
    pid_t pid;
    int tmp = 0;
    int pipe_fd[2];
    char* buffer;

    if(!~pipe(pipe_fd)) {
        perror("Failed to open the pipe");
        return NULL;
    }

    pid = fork();
    if(!~pid) {
        perror("Failed to fork");
        return NULL;
    }
    else if(!pid) {
        //child
        close(pipe_fd[0]);
        if(!~dup2(pipe_fd[1], STDOUT_FILENO)) {
            perror("Failed to redirect stdout to th pipe");
            exit(-1);
        }
        close(pipe_fd[1]);

        if(!~execv(program, args)) {
            fprintf(stderr, "Failed to execute %s: %s", program, strerror(errno));
            exit(-1);
        }
    }

    close(pipe_fd[1]);
    buffer = (char*) malloc(BUFFER_SIZE);
    empty(buffer, strlen(buffer));

    if(!~read(pipe_fd[0], buffer, BUFFER_SIZE))
        perror("Failed to read from the pipe");
    
    buffer = realloc(buffer, strlen(buffer)+1);
    buffer[strlen(buffer)] = 0;
    close(pipe_fd[0]);
    wait(&tmp);

    if(status)
        *status = WEXITSTATUS(tmp);
    return buffer;
}

char** genargs(size_t size, ...)
{
    va_list _arg_list;
    char** args = (char**) calloc(size, sizeof(char**));

    va_start(_arg_list, size);
    for(int i = 0; i < size; ++i)
        args[i] = va_arg(_arg_list, char*);
    
    va_end(_arg_list);
    return args;
}