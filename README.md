# Unirun

`Unirun` is a library that provides the programmer with functions that allow him to use the output of other programs, so that he does not have to write from scratch functions that are already implemented in binaries installed on the system. In this sense it facilitates reusability and modularity.

# Run
```c
char* run(char* program, char* args[], int* status);
char* which(char* path);
```

The `run` function executes a program with its arguments, and returns the output. The `which` function is basically like:
```c
char* args[2];
args[0] = "which";
args[1] = path;
run("/usr/bin/which", args, NULL);
```

## Return value
The return value of run is a pointer to a buffer which holds the output of the program and the status of the program (in the variable `int* stauts`)