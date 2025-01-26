#!/bin/sh

set -xe

CC=gcc
AR=ar
FLAGS="-Wall -Wextra -Werror"
TARGET=strap

if test -d objs; then rm -r objs; fi;

mkdir objs
for file in *.c; do
    ${CC} ${FLAGS} -c -o objs/${file%.*}.o ${file}
done
${AR} -rcs lib${TARGET}.a objs/*.o

rm -r objs

