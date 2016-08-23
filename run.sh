#!/bin/bash
set -x
gcc -g -o viewer viewer.c -L/usr/brlcad/rel-7.26.1/lib -lrt -lbu -lged -I/usr/brlcad/rel-7.26.1/include -I/usr/brlcad/rel-7.26.1/include/brlcad
cat viewer db/ktank.g > exec
./exec
