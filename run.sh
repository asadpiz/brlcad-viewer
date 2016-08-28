#!/bin/bash
set -x
gcc -g -o viewer viewer.c glad.c -L/usr/brlcad/rel-7.26.1/lib -lrt -lbu -lged -I/usr/brlcad/rel-7.26.1/include -I/usr/brlcad/rel-7.26.1/include/brlcad -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
cat viewer db/moss.g > exec
./exec
