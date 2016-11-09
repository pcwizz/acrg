#!/bin/sh

if test "$1" = "debug"
then
	echo 'CC = cc
CCOPTS = -Wall -O2 --std=c99 -fsanitize=address -ggdb -lm
' > Makefile
elif test "$1" = "fuzz"
then
	echo 'CC = afl-gcc
CCOPTS = -Wall -O2 --std=c99 -fsanitize=address -ggdb -lm
' > Makefile
else
	echo 'CC = cc
CCOPTS = -Wall -O2 --std=c99 -lm
' > Makefile
fi

cat Makefile.tail >> Makefile
