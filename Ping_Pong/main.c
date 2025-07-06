#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "estrutura.h"
#include "gameover.h"

float velocidade = 3.0f;
bool podeMovimentar = true; 
GameState estadoAtual = JOGANDO;

void contarPontos(Bola *bola , Oponente *oponente , Jogador *jogador);
void exibirPontos(Jogador *jogador , Oponente *oponente , Texto *texto);
void gerarJogador(Jogador *jogador);
void movimentarJogador(Jogador *jogador);
void prenderJogador(Jogador *jogador , Tela *tela);
void gerarOponente(Oponente *oponente);
void movimentarOponente(Oponente *oponente , Tela *tela , Bola *bola);
void gerarBola(Bola *bola);
void movimentarBola(Bola *bola);
void colidirBola(Bola *bola , Tela *tela , Jogador *jogador , Oponente *oponente);
void escaparBola(Bola *bola , Tela *tela);

int main(){
	
	srand(time(NULL));
	
	Tela tela;

	tela.xTela = 800;
	tela.yTela = 600;
	strcpy(tela.nomeTela , "Ping Pong");

	Texto texto;

	texto.xTexto = tela.xTela / 2 - 85;
	texto.yTexto = 10;
	texto.fonteTexto = 70;
	texto.corTexto = WHITE;

	Jogador jogador;

	jogador.xJogador = tela.xTela - tela.xTela;
	jogador.yJogador = tela.yTela / 3;
	jogador.larguraJogador = 20;
	jogador.alturaJogador = 200;
	jogador.corJogador = WHITE;
	jogador.corpoJogador = (Rectangle){jogador.xJogador , jogador.yJogador , jogador.larguraJogador , jogador.alturaJogador}; 
	jogador.pontosJogador = 0;

	Bola bola;

	bola.xBola = tela.xTela / 2;
	bola.yBola = rand() % tela.yTela;
	bola.tamanhoBola = 10;
	bola.corBola = WHITE;
	bola.xVelocidadeBola = 3.0f;
	bola.yVelocidadeBola = 3.0f;
	bola.centroBola = (Vector2){bola.xBola , bola.yBola};
	bola.escapouDoJogador = false;
	bola.escapouDoOponente = false;

	Oponente oponente;

	oponente.xOponente = tela.xTela - 20;
	oponente.yOponente = tela.yTela / 3;
	oponente.larguraOponente = 20;
	oponente.alturaOponente = 200;
	oponente.corOponente = WHITE;
	oponente.direcaoOponente = 1;
	oponente.corpoOponente = (Rectangle){oponente.xOponente , oponente.yOponente , oponente.larguraOponente , oponente.alturaOponente};
	oponente.centroOponente = oponente.yOponente + oponente.alturaOponente / 2;
	oponente.pontosOponente = 0;

	InitWindow(tela.xTela , tela.yTela , tela.nomeTela);

	SetTargetFPS(144);

	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		switch(estadoAtual){
			case JOGANDO:
			contarPontos(&bola , &oponente , &jogador);
			exibirPontos(&jogador , &oponente , &texto);
			gerarJogador(&jogador);
			movimentarJogador(&jogador);
			prenderJogador(&jogador , &tela);
			gerarOponente(&oponente);
			movimentarOponente(&oponente , &tela , &bola);
			gerarBola(&bola);
			movimentarBola(&bola);
			colidirBola(&bola , &tela , &jogador , &oponente);
			escaparBola(&bola , &tela);
			if(oponente.pontosOponente >= 5){
				estadoAtual = GAMEOVER;
			}
			break;

			case (GAMEOVER):
				gameOver(&tela);
				break;
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}


void contarPontos(Bola *bola , Oponente *oponente , Jogador *jogador){
	if(bola->escapouDoOponente){
		jogador->pontosJogador += 1;
		bola->escapouDoOponente = false;
	}
	if(bola->escapouDoJogador){
		oponente->pontosOponente += 1;
		bola->escapouDoJogador = false;
	}
}


void exibirPontos(Jogador *jogador , Oponente *oponente , Texto *texto){
	sprintf(texto->texto , "%d - %d" , jogador->pontosJogador , oponente->pontosOponente);
	DrawText(texto->texto , texto->xTexto , texto->yTexto , texto->fonteTexto , texto->corTexto);
}

void gerarJogador(Jogador *jogador){
	DrawRectangle(jogador->xJogador , jogador->yJogador , jogador->larguraJogador , jogador->alturaJogador , jogador->corJogador);
}

void movimentarJogador(Jogador *jogador){
	if(podeMovimentar){
		if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) jogador->yJogador -= velocidade;
		if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) jogador->yJogador += velocidade;
	}

	jogador->corpoJogador = (Rectangle){jogador->xJogador , jogador->yJogador , jogador->larguraJogador , jogador->alturaJogador};
}

void prenderJogador(Jogador *jogador , Tela *tela){
	if(jogador->yJogador < tela->yTela - tela->yTela) jogador->yJogador = tela->yTela - tela->yTela;
	if(jogador->alturaJogador + jogador->yJogador > tela->yTela) jogador->yJogador = tela->yTela - jogador->alturaJogador;
}

void gerarOponente(Oponente *oponente){
	DrawRectangle(oponente->xOponente , oponente->yOponente , oponente->larguraOponente , oponente->alturaOponente , oponente->corOponente);
}

void movimentarOponente(Oponente *oponente , Tela *tela , Bola *bola){
	if(podeMovimentar){
		if(bola->yBola > oponente->centroOponente){
			oponente->yOponente += velocidade;
		}
		if(bola->yBola < oponente->centroOponente){
			oponente->yOponente -= velocidade;
		}
		if(oponente->alturaOponente + oponente->yOponente >= tela->yTela){
			oponente->yOponente = tela->yTela - oponente->alturaOponente;
			oponente->direcaoOponente = -1;
		} 
		if(oponente->yOponente <= 0){
			oponente->yOponente = 0;
			oponente->direcaoOponente = 1;
		}

		oponente->centroOponente = oponente->alturaOponente + oponente->yOponente / 2;
	}

	oponente->corpoOponente = (Rectangle){oponente->xOponente , oponente->yOponente , oponente->larguraOponente , oponente->alturaOponente};
}

void gerarBola(Bola *bola){
	DrawCircle(bola->xBola , bola->yBola , bola->tamanhoBola , bola->corBola);
}

void movimentarBola(Bola *bola){
	bola->xBola -= bola->xVelocidadeBola;
	bola->yBola -= bola->yVelocidadeBola;

	bola->centroBola = (Vector2){bola->xBola , bola->yBola};
}

void colidirBola(Bola *bola , Tela *tela , Jogador *jogador , Oponente *oponente){
	if(bola->yBola - bola->tamanhoBola <= 0 || bola->yBola + bola->tamanhoBola >= tela->yTela){
		bola->yVelocidadeBola *= -1;
	}

	if(CheckCollisionCircleRec(bola->centroBola , bola->tamanhoBola , jogador->corpoJogador)){
		bola->xVelocidadeBola *= -1;
		bola->xBola = jogador->xJogador + jogador->larguraJogador + bola->tamanhoBola;
	}

	if(CheckCollisionCircleRec(bola->centroBola , bola->tamanhoBola , oponente->corpoOponente)){
		bola->xVelocidadeBola *= -1;
		bola->xBola = oponente->xOponente - bola->tamanhoBola;
	}
}

void escaparBola(Bola *bola , Tela *tela){
	if(bola->xBola > tela->xTela){
		bola->escapouDoOponente = true;
		bola->xBola = tela->xTela / 2;
		bola->yBola = rand() % tela->yTela; 
	}
	if(bola->xBola < 0){
		bola->escapouDoJogador = true;
		bola->xBola = tela->xTela / 2;
		bola->yBola = rand() % tela->yTela;
	}

}