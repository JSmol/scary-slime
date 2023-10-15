#!/bin/bash
set -ex

# gcc -Wall -Wextra main.c -o out -lraylib

g++ -Wall -Wextra -c ffmpeg.cc -o ffmpeg.o

g++ -Wall -Wextra main.cc ffmpeg.o -o out \
    -lraylib
