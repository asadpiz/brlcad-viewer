#!/bin/bash
set -x
gcc -g -o viewer viewer.c -L/usr/brlcad/rel-7.24.4/lib -lrt -lbu -I/usr/brlcad/rel-7.24.4/include -I/usr/brlcad/rel-7.24.4/include/brlcad
cat viewer moss.g > exec
./exec
