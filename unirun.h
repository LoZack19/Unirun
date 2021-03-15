#ifndef UNIRUN_H
#define UNIRUN_H

#include <stddef.h>

#define BUFFER_SIZE     0x4000
#define PATH_SIZE       0x400

char* run(char* program, char* args[], int* status);
char** genargs(size_t size, ...);

#define WHICH(path) run("/usr/bin/which", genargs(3, "which", path, 0), NULL)

#endif
