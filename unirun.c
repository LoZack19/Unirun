#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "unirun.h"

char *run(char *program, char *args[], int *status)
{
	pid_t pid;
	int tmp = 0;
	int pipe_fd[2];
	char *buffer;

	if (!~pipe(pipe_fd)) {
		perror("Failed to open the pipe");
		return NULL;
	}

	pid = fork();
	if (!~pid) {
		perror("Failed to fork");
		return NULL;
	} else if (!pid) {
		close(pipe_fd[0]);
		if (!~dup2(pipe_fd[1], STDOUT_FILENO)) {
			perror("Failed to redirect stdout to the pipe");
			exit(-1);
		}
		close(pipe_fd[1]);

		if (!~execv(program, args)) {
			fprintf(stderr, "Failed to execute %s: %s",
				program, strerror(errno));
			exit(-1);
		}
	}

	close(pipe_fd[1]);
	buffer = (char *) malloc(BUFFER_SIZE);
	memset(buffer, 0, BUFFER_SIZE);

	if (!~read(pipe_fd[0], buffer, BUFFER_SIZE))
		perror("Failed to read from the pipe");

	buffer = realloc(buffer, strlen(buffer) + 1);
	buffer[strlen(buffer)] = 0;
	close(pipe_fd[0]);
	wait(&tmp);

	if (status && WIFEXITED(tmp))
		*status = WEXITSTATUS(tmp);
	return buffer;
}

uint8_t launch(char *program, char *args[], int fd)
{
	int wstatus = 0;
	uint8_t exit_status = 0xFF;

	switch (fork()) {
	case -1:
		perror("Failed to fork");
		break;

	case 0:
		if (fd && ~fd && fd != STDOUT_FILENO) {
			if (!~dup2(fd, STDOUT_FILENO))
				perror("Failed to redirect stdout");
			if (!~close(fd))
				perror("Failed to close file descriptor");
		}
		if (!~execvp(program, args))
			fprintf(stderr, "Failed to execute %s: %s",
				program, strerror(errno));
		exit(-1);

	default:
		wait(&wstatus);
		if (WIFEXITED(wstatus))
			exit_status = WEXITSTATUS(wstatus);
		else
			exit_status = 0xFF;
		break;
	}

	return exit_status;
}

char **genargs(size_t size, ...)
{
	va_list _arg_list;
	char **args = calloc(size, sizeof(char *));

	va_start(_arg_list, size);
	for (size_t i = 0; i < size; ++i)
		args[i] = va_arg(_arg_list, char *);

	va_end(_arg_list);
	return args;
}
