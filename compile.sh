#!/bin/bash

CONTEXT="`dirname $0`"
rm -r $CONTEXT/bin 2>/dev/null && mkdir $CONTEXT/bin
gcc -std=c99 -pedantic-errors -o $CONTEXT/bin/my $CONTEXT/src/my.c
