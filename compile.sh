#!/bin/bash

CONTEXT="`dirname $0`"
rm -r $CONTEXT/bin 2>/dev/null && mkdir $CONTEXT/bin
gcc -std=c99 -pedantic-errors \
  -D_GNU_SOURCE \
  -D__USE_GNU \
  -o $CONTEXT/bin/my.o $CONTEXT/src/my.c
