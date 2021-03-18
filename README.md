# Unirun

`Unirun` is a library that provides the programmer with functions that allow him to use the output of other programs, so that he does not have to write from scratch functions that are already implemented in binaries installed on the system. In this sense it facilitates reusability and modularity.

## RUN
```c
char* run(char* program, char* args[], int* status);
#define WHICH(path) run("/usr/bin/which", genargs(3, "/usr/bin/which", path, NULL), NULL)
```

The `run` function executes a program with its arguments, and returns the output. The `which` function is a wrapper of the `run` function built to expand the path of a program by launching the which command.

It can read up to `BUFFER_SIZE` bytes, which is set by default to `0x4000` or `16384` in `unirun.h`. If you compile the library from source, you can change that value to be higher or lower.  
Note that `run` adjusts the size of the returned buffer to bmatch exactly the string length if it's lower than `BUFFER_SIZE`.
## Return value
The return value of run is a pointer to a buffer which holds the output of the program and the status of the program (in the variable `int* stauts`)

## GENARGS
```c
char** genargs(size_t size, ...);
```

The `genargs` function takes in input a list of `size` arguments and returns an array of strings to hold them. The list must begin with the path of the program and end with `NULL`.