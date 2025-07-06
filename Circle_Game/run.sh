#/bin/bash
gcc main.c gameover.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./main
