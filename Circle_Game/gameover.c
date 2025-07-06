#include "gameover.h"
#include <stdio.h>
#include "estruturas.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

void gerarNovaBola(Bola *bola , Tela *telaPrincipal);

void gameOver(Tempo *tempo , Tela *telaPrincipal , Pontuacao *pontuacao , Jogador *jogador , Bola *bola , Sons *sons , Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2){

    Texto gameOverText;

    strcpy(gameOverText.texto , "GAME OVER");
    gameOverText.cor = RED;
    gameOverText.x_texto = telaPrincipal->x_tela / 2.5 - 20;
    gameOverText.y_texto =  telaPrincipal->y_tela / 2; 
    gameOverText.fonte = 30;

    Texto gameOverText1;

    strcpy(gameOverText1.texto , "APERTE ENTER PARA REINICIAR");
    gameOverText1.cor = RED;
    gameOverText1.x_texto = telaPrincipal->x_tela / 3 - 120;
    gameOverText1.y_texto = telaPrincipal->y_tela / 2 + 50; 
    gameOverText1.fonte = 30;

    DrawText(gameOverText.texto, gameOverText.x_texto, gameOverText.y_texto , gameOverText.fonte , gameOverText.cor);
    DrawText(gameOverText1.texto , gameOverText1.x_texto , gameOverText1.y_texto , gameOverText1.fonte , gameOverText1.cor);
    if(IsKeyPressed(KEY_ENTER)){
        pontuacao->pontos = 0;
        tempo->tempo_total = 5;
        tempo->tempo_inicial = GetTime();
        jogador->x_jogador = telaPrincipal->x_tela / 2;
        jogador->y_jogador = telaPrincipal->y_tela / 2;
        obstaculo->aparecer = false;
        obstaculo1->aparecer = false;
        obstaculo2->aparecer = false;
        obstaculo->colidiuObstaculo = false;
        obstaculo1->colidiuObstaculo = false;
        obstaculo2->colidiuObstaculo = false;
        gerarNovaBola(bola , telaPrincipal);
        PlayMusicStream(sons->fundo);
        estadoAtual = JOGANDO;
    }
}

void gerarNovaBola(Bola *bola , Tela *telaPrincipal){
    bola->x_bola = rand() % telaPrincipal->x_tela;
    bola->y_bola = rand() % telaPrincipal->y_tela;
    while(bola->y_bola < 150){
        bola->y_bola = rand() % telaPrincipal->y_tela;
    }
}
