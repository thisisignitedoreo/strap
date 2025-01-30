# libstrap
A bunch of useful stuff for casual C coding

## Overview
Library consists of modules:

- `array.h`: dynamic arrays
- `linked_list.h`: linked lists
- `fileio.h`: files and directories i/o
- `arena.h`: arena allocators
- `string.h`: stringview implementation

Check sources for more info

## Build
```console
$ ./build.sh
$ gcc -L. -o main main.c -lstrap
```