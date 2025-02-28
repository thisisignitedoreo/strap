#!/bin/sh

set -e

cd $(dirname "$0")

CC="${1}gcc"
AR="${1}ar"
FLAGS="-Wall -Wextra -Werror -std=gnu99 -pedantic"
TARGET=strap

if test -d objs; then rm -r objs; fi;

mkdir objs
for file in src/*.c; do
    ofile=${file##*/}
    ${CC} ${FLAGS} -c -o objs/${ofile%.*}.o ${file}
done
${AR} -rcs lib${TARGET}.a objs/*.o

rm -r objs

