# Comando para automatizar a compilação e a execução do jogo

# Comand to automatize the compilation and run the game

#/bin/bash
gcc main.c gameover.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./main
