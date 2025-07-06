#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "raylib.h"

const int larguraTela = 800;
const int alturaTela = 600;

typedef struct{
	float velocidade;
} Game;

typedef struct{
	int xTela , yTela;
	char titulo[10];
	Color corTela;	
} Tela;

typedef struct{
	float xJogador , yJogador;
	int tamanhoJogador;
	bool podeMovimentar;
	Color corJogador;
	Vector2 centroJogador;
} Jogador;

typedef struct{
	float xRecompensa, yRecompensa;
	int tamanhoRecompensa;
	Color corRecompensa;
	Vector2 centroRecompensa;
} Recompensa;

typedef struct{
	float xObstaculo , yObstaculo;
	int tamanhoObstaculo;
	bool colidiuObstaculo;
	bool gerarNovoObstaculo;
	Color corObstaculo;
	Vector2 centroObstaculo;
	float velocidadeObstaculo;
} Obstaculo;

void gerarJogador(Jogador *jogador);
void moverJogador(Jogador *jogador , Game *game);
void prenderJogador(Jogador *jogador , Tela *tela);
void gerarObstaculo(Obstaculo *obstaculo);
void moverObstaculo(Obstaculo *obstaculo);
void inspecionarObstaculo(Obstaculo *obstaculo , Tela *tela);
void colisaoObstaculo(Obstaculo *obstaculo , Jogador *jogador);
void colisaoSinal(Obstaculo *obstaculo , Tela *tela);

int main(){
	
	srand(time(NULL));	

	Game game;

	game.velocidade = 3.5f;

	Tela tela;

	tela.xTela = alturaTela;
	tela.yTela = larguraTela;
	strcpy(tela.titulo , "Game");
	tela.corTela = BLACK;	

	Jogador jogador;
	
	jogador.xJogador = tela.xTela / 2;
	jogador.tamanhoJogador = 30;
	jogador.yJogador = tela.yTela - jogador.tamanhoJogador;
	jogador.podeMovimentar = true;
	jogador.corJogador = BLUE;
	jogador.centroJogador = (Vector2){jogador.xJogador ,jogador.yJogador};
	
	Obstaculo obstaculo;

	obstaculo.xObstaculo = rand() % tela.xTela;
	obstaculo.tamanhoObstaculo = 15;
	obstaculo.yObstaculo = 0 + obstaculo.tamanhoObstaculo;
	obstaculo.colidiuObstaculo = false;
	obstaculo.gerarNovoObstaculo = true;
	obstaculo.corObstaculo = RED;
	obstaculo.velocidadeObstaculo = 5.0f;
	obstaculo.centroObstaculo = (Vector2){obstaculo.xObstaculo , obstaculo.yObstaculo};

	InitWindow(tela.xTela , tela.yTela , tela.titulo);
	SetTargetFPS(144);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(tela.corTela);
		
		gerarJogador(&jogador);
		moverJogador(&jogador , &game);
		prenderJogador(&jogador , &tela);
		gerarObstaculo(&obstaculo);
		moverObstaculo(&obstaculo);
		inspecionarObstaculo(&obstaculo , &tela);
		colisaoObstaculo(&obstaculo , &jogador);		
		colisaoSinal(&obstaculo , &tela);		

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void gerarJogador(Jogador *jogador){
	DrawCircle(jogador->xJogador , jogador->yJogador , jogador->tamanhoJogador , jogador->corJogador);
}

void moverJogador(Jogador *jogador , Game *game){
	if(jogador->podeMovimentar){
		if(IsKeyDown(KEY_A)) jogador->xJogador -= game->velocidade;
		if(IsKeyDown(KEY_D)) jogador->xJogador += game->velocidade;
	}
	
	jogador->centroJogador = (Vector2){jogador->xJogador , jogador->yJogador};	
}

void prenderJogador(Jogador *jogador , Tela *tela){
	if(jogador->xJogador + jogador->tamanhoJogador > tela->xTela) jogador->xJogador = tela->xTela - jogador->tamanhoJogador;
	if(jogador->xJogador - jogador->tamanhoJogador < 0) jogador->xJogador = jogador->tamanhoJogador;
}

void gerarObstaculo(Obstaculo *obstaculo){
	if(obstaculo->gerarNovoObstaculo){
		DrawCircle(obstaculo->xObstaculo , obstaculo->yObstaculo , obstaculo->tamanhoObstaculo , obstaculo->corObstaculo);
	}
}

void moverObstaculo(Obstaculo *obstaculo){
	obstaculo->yObstaculo += obstaculo->velocidadeObstaculo;
	
	obstaculo->centroObstaculo = (Vector2){obstaculo->xObstaculo , obstaculo->yObstaculo};
}

void inspecionarObstaculo(Obstaculo *obstaculo , Tela *tela){
	if(obstaculo->yObstaculo - obstaculo->tamanhoObstaculo > tela->yTela){
		obstaculo->xObstaculo = rand() % tela->xTela;
		obstaculo->yObstaculo = 0;
	}
}

void colisaoObstaculo(Obstaculo *obstaculo , Jogador *jogador){
	if(CheckCollisionCircles(obstaculo->centroObstaculo , obstaculo->tamanhoObstaculo , jogador->centroJogador , jogador->tamanhoJogador)){
		obstaculo->colidiuObstaculo = true;
	}
}

void colisaoSinal(Obstaculo *obstaculo , Tela *tela){
	if(obstaculo->colidiuObstaculo){
		DrawText("COLIDIU" , tela->xTela / 2 , tela->yTela / 2 , 30 , GREEN);
	}
}
