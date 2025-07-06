#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "raylib.h"
#include "estruturas.h"

void gameOver(Tempo *tempo , Tela *telaPrincipal , Pontuacao *pontuacao , Jogador *jogador , Bola *bola , Sons *sons , Obstaculo *obstaculo , Obstaculo *obstaculo1 ,Obstaculo *obstaculo2);
void gerarNovaBola(Bola *bola , Tela *telaPrincipal);

#endif
