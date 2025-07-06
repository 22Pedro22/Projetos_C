#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

extern float velocidade;
extern bool podeMovimentar; 

typedef enum{
    JOGANDO,
    GAMEOVER
} GameState;

extern GameState estadoAtual;

typedef struct{
    int xTexto , yTexto , fonteTexto;
    char texto[20];
    Color corTexto;
} Texto;

typedef struct{
	int pontuacao;
} Pontuacao;

typedef struct{
	int xTela;
	int yTela;
	char nomeTela[10];
} Tela;

typedef struct{
	float xJogador;
	float yJogador;
	int larguraJogador;
	int alturaJogador;
	Color corJogador;
	Rectangle corpoJogador;
    int pontosJogador;
} Jogador;

typedef struct{
	float xBola;
	float yBola;
	int tamanhoBola;
	Color corBola;
	float xVelocidadeBola , yVelocidadeBola;
	Vector2 centroBola;
    bool escapouDoJogador;
    bool escapouDoOponente;
} Bola;

typedef struct{
	float xOponente;
	float yOponente;
	int larguraOponente;
	int alturaOponente;
	Color corOponente;
	int direcaoOponente;
	Rectangle corpoOponente;
	float centroOponente;
    int pontosOponente;
} Oponente;

#endif