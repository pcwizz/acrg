#!/bin/sh

AFL_USE_ASAN=1 AFL_HARDEN=1 ./configure.sh fuzz; make

afl-fuzz -m none -i afl-in -o out build/acrg @@

