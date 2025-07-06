#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include "raylib.h"
#include <stdio.h>

typedef struct {
    float x_tempo;
    float y_tempo;
    float tempo_total;
    float tempo_inicial;
    float tempo_atual;
    int tempo_restante;
    char tempo[100];
    Color cor;
    int fonte;
} Tempo;

typedef struct {
    int pontos;
    char pontuacao[100];
    float x_pontuacao;
    float y_pontuacao;
    int fonte;
    Color cor;
} Pontuacao;

typedef struct {
    int x_tela;
    int y_tela;
    char nome[10];
} Tela;

typedef struct {
    Vector2 centroJogador;
    float x_jogador;
    float y_jogador;
    int tamanho;
    Color cor;
    bool mover;
} Jogador;

typedef struct {
    Vector2 centroBola;
    float x_bola;
    float y_bola;
    int tamanho;
    Color cor;
    bool colidiu;
} Bola;

typedef struct {
    char texto[50];
    Color cor;
    float x_texto;
    float y_texto;
    int fonte;
} Texto;

typedef struct {
    Sound coletar;
    Sound fim;
    Music fundo;
} Sons;

typedef struct{
    Vector2 centroObstaculo;
    float x_obstaculo;
    float y_obstaculo;
    int tamanho;
    Color cor;
    bool colidiuObstaculo;
    bool aparecer;
} Obstaculo;

typedef enum{
    JOGANDO,
    GAMEOVER
} GameState;

extern GameState estadoAtual;

#endif
