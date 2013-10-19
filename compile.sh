#!/bin/bash

CONTEXT="`dirname $0`"
echo $CONTEXT;
rm -r $CONTEXT/bin;
mkdir $CONTEXT/bin;
gcc -std=c99 -pedantic-errors \
  -D_GNU_SOURCE \
  -D__USE_GNU \
  -o $CONTEXT/bin/my.o $CONTEXT/src/my.c
