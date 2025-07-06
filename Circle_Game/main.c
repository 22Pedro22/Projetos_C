#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "raylib.h"
#include <time.h>
#include "gameover.h"
#include "estruturas.h"

GameState estadoAtual = JOGANDO;

float velocidade = 3.5f;

void mostrarPontos(Pontuacao *pontuacao);
void mostrarTempo(Tempo *tempo);
void contarTempo(Tempo *tempo);
void gerarJogador(Jogador *jogador);
void moverJogador(Jogador *jogador);
void prenderJogador(Jogador *jogador, Tela *telaPrincipal);
void gerarBola(Bola *bola);
void moverBola(Bola *bola, Jogador *jogador, Tela *telaPrincipal, Sons *sons);
void contarPontos(Bola *bola, Pontuacao *pontuacao , Tempo *tempo , Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2);
void gerarObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2);
void moverObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2 , Tela *telaPrincipal);
void colidirObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2 , Jogador *jogador);

int main() {
    srand(time(NULL));

    Tempo tempo;
    tempo.x_tempo = 10;
    tempo.y_tempo = 40;
    tempo.tempo_total = 5;
    tempo.tempo_inicial = GetTime();
    tempo.fonte = 30;
    tempo.cor = YELLOW;

    Pontuacao pontuacao;
    pontuacao.pontos = 0;
    pontuacao.x_pontuacao = 10;
    pontuacao.y_pontuacao = 10;
    pontuacao.fonte = 30;
    pontuacao.cor = YELLOW;

    Tela telaPrincipal;
    strcpy(telaPrincipal.nome, "Game Ball");
    telaPrincipal.x_tela = 800;
    telaPrincipal.y_tela = 600;

    Jogador jogador;
    jogador.x_jogador = telaPrincipal.x_tela / 2;
    jogador.y_jogador = telaPrincipal.y_tela / 2;
    jogador.tamanho = 30;
    jogador.cor = BLUE;
    jogador.centroJogador = (Vector2){jogador.x_jogador, jogador.y_jogador};
    jogador.mover = true;

    Bola bola;
    bola.x_bola = rand() % telaPrincipal.x_tela;
    bola.y_bola = rand() % telaPrincipal.y_tela;
    bola.tamanho = 15;
    bola.cor = GREEN;
    bola.colidiu = false;
    bola.centroBola = (Vector2){bola.x_bola, bola.y_bola};

    Obstaculo obstaculo;

    obstaculo.y_obstaculo = telaPrincipal.y_tela / 2;
    obstaculo.x_obstaculo = 0;
    obstaculo.tamanho = 10;
    obstaculo.cor = WHITE;
    obstaculo.centroObstaculo = (Vector2){obstaculo.x_obstaculo , obstaculo.y_obstaculo};
    obstaculo.colidiuObstaculo = false;
    obstaculo.aparecer = false;
    
    Obstaculo obstaculo1;
    
    obstaculo1.x_obstaculo = telaPrincipal.x_tela / 2;
    obstaculo1.y_obstaculo = 0;
    obstaculo1.tamanho = 10;
    obstaculo1.cor = WHITE;
    obstaculo1.centroObstaculo = (Vector2){obstaculo1.x_obstaculo , obstaculo1.y_obstaculo};
    obstaculo1.colidiuObstaculo = false;
    obstaculo1.aparecer = false;
	
    Obstaculo obstaculo2;

    obstaculo2.x_obstaculo = telaPrincipal.x_tela;
    obstaculo2.y_obstaculo = telaPrincipal.y_tela / 2 + 200;
    obstaculo2.tamanho = 10;
    obstaculo2.cor = WHITE;
    obstaculo2.centroObstaculo = (Vector2){obstaculo2.x_obstaculo , obstaculo2.y_obstaculo};
    obstaculo2.colidiuObstaculo = false;
    obstaculo2.aparecer = false;

    InitAudioDevice();

    Sons sons;
    sons.coletar = LoadSound("assets/sons/coletar.mp3");
    sons.fim = LoadSound("assets/sons/fim.mp3");
    sons.fundo = LoadMusicStream("assets/musicas/fundo.mp3");

    InitWindow(telaPrincipal.x_tela, telaPrincipal.y_tela + 50, telaPrincipal.nome);
    PlayMusicStream(sons.fundo);
    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        UpdateMusicStream(sons.fundo);
        BeginDrawing();
        ClearBackground(BLACK);

		switch(estadoAtual){
			case JOGANDO:
				mostrarPontos(&pontuacao);
				mostrarTempo(&tempo);
				gerarJogador(&jogador);
				gerarBola(&bola);
				moverJogador(&jogador);
				prenderJogador(&jogador, &telaPrincipal);
				moverBola(&bola, &jogador, &telaPrincipal, &sons);
				contarPontos(&bola, &pontuacao , &tempo , &obstaculo , &obstaculo1 , &obstaculo2);
				contarTempo(&tempo);
                gerarObstaculo(&obstaculo , &obstaculo1 , &obstaculo2);
                moverObstaculo(&obstaculo , &obstaculo1 , &obstaculo2 , &telaPrincipal);
                colidirObstaculo(&obstaculo , &obstaculo1 , &obstaculo2 , &jogador);
				if(tempo.tempo_restante <= 0 || obstaculo.colidiuObstaculo || obstaculo1.colidiuObstaculo || obstaculo2.colidiuObstaculo){
					estadoAtual = GAMEOVER;
					StopMusicStream(sons.fundo);
					PlaySound(sons.fim);
				}
				break;
			
			case GAMEOVER:
				gameOver(&tempo , &telaPrincipal , &pontuacao , &jogador , &bola , &sons , &obstaculo , &obstaculo1 , &obstaculo2);
				break;
				
		}

        EndDrawing();
    }

    UnloadMusicStream(sons.fundo);
    UnloadSound(sons.coletar);
    UnloadSound(sons.fim);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void colidirObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2 , Jogador *jogador){
    if(obstaculo->aparecer && CheckCollisionCircles(obstaculo->centroObstaculo , obstaculo->tamanho , jogador->centroJogador , jogador->tamanho)){
        obstaculo->colidiuObstaculo = true;
    }

    if(obstaculo1->aparecer && CheckCollisionCircles(obstaculo1->centroObstaculo , obstaculo1->tamanho , jogador->centroJogador , jogador->tamanho)){
        obstaculo1->colidiuObstaculo = true;
    }

    if(obstaculo2->aparecer && CheckCollisionCircles(obstaculo2->centroObstaculo , obstaculo2->tamanho , jogador->centroJogador , jogador->tamanho)){
        obstaculo2->colidiuObstaculo = true;
    }
}

void mostrarPontos(Pontuacao *pontuacao) {
    sprintf(pontuacao->pontuacao, "PONTOS: %d", pontuacao->pontos);
    DrawText(pontuacao->pontuacao, pontuacao->x_pontuacao, pontuacao->y_pontuacao, pontuacao->fonte, pontuacao->cor);
}

void mostrarTempo(Tempo *tempo) {
    sprintf(tempo->tempo, "TEMPO: %d", tempo->tempo_restante);
    DrawText(tempo->tempo, tempo->x_tempo, tempo->y_tempo, tempo->fonte, tempo->cor);
}

void contarTempo(Tempo *tempo) {
    tempo->tempo_atual = GetTime();
    tempo->tempo_restante = tempo->tempo_total - (int)(tempo->tempo_atual - tempo->tempo_inicial);
}

void gerarJogador(Jogador *jogador) {
    DrawCircle(jogador->x_jogador, jogador->y_jogador, jogador->tamanho, jogador->cor);
}

void moverJogador(Jogador *jogador) {
    if (jogador->mover) {
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) jogador->y_jogador -= velocidade;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) jogador->y_jogador += velocidade;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) jogador->x_jogador -= velocidade;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) jogador->x_jogador += velocidade;
        }
    jogador->centroJogador = (Vector2){jogador->x_jogador, jogador->y_jogador};
}

void prenderJogador(Jogador *jogador, Tela *telaPrincipal) {
    if (jogador->x_jogador - jogador->tamanho > telaPrincipal->x_tela) jogador->x_jogador = 0;
    if (jogador->x_jogador + jogador->tamanho < 0) jogador->x_jogador = telaPrincipal->x_tela + jogador->tamanho;
    if (jogador->y_jogador - jogador->tamanho > telaPrincipal->y_tela) jogador->y_jogador = 0;
    if (jogador->y_jogador + jogador->tamanho < 0) jogador->y_jogador = telaPrincipal->y_tela + jogador->tamanho;

    jogador->centroJogador = (Vector2){jogador->x_jogador, jogador->y_jogador};
}

void gerarBola(Bola *bola) {
    DrawCircle(bola->x_bola, bola->y_bola, bola->tamanho, bola->cor);
}

void moverBola(Bola *bola, Jogador *jogador, Tela *telaPrincipal, Sons *sons) {
    if (CheckCollisionCircles(jogador->centroJogador, jogador->tamanho, bola->centroBola, bola->tamanho)) {
        bola->colidiu = true;
        bola->x_bola = rand() % telaPrincipal->x_tela;
        bola->y_bola = rand() % telaPrincipal->y_tela;
        PlaySound(sons->coletar);
        while (bola->y_bola < 150) {
            bola->y_bola = rand() % telaPrincipal->y_tela;
        }
    } else {
        bola->colidiu = false;
    }
    bola->centroBola = (Vector2){bola->x_bola, bola->y_bola};
}

void contarPontos(Bola *bola, Pontuacao *pontuacao, Tempo *tempo , Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2) {
    if (bola->colidiu == true) {
        pontuacao->pontos += 1;
        if(pontuacao->pontos % 5 == 0){
            tempo->tempo_total += 5;
        }
        if(pontuacao->pontos >= 10 && pontuacao->pontos < 20){
            obstaculo->aparecer = true;
        }
        if(pontuacao->pontos >=20 && pontuacao->pontos < 30){
            obstaculo1->aparecer = true;
        }
        if(pontuacao->pontos >= 30){
            obstaculo2->aparecer = true;
        }
        if(pontuacao->pontos == 0){
            obstaculo->aparecer = false;
            obstaculo1->aparecer = false;
            obstaculo2->aparecer = false;
        }
    }
}

void gerarObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2){
    if(obstaculo->aparecer){
        DrawCircle(obstaculo->x_obstaculo , obstaculo->y_obstaculo , obstaculo->tamanho , obstaculo->cor);
    }
    if(obstaculo1->aparecer){
        DrawCircle(obstaculo1->x_obstaculo , obstaculo1->y_obstaculo , obstaculo1->tamanho , obstaculo1->cor);
    }
    if(obstaculo2->aparecer){
        DrawCircle(obstaculo2->x_obstaculo , obstaculo2->y_obstaculo , obstaculo2->tamanho , obstaculo2->cor);
    }
}

void moverObstaculo(Obstaculo *obstaculo , Obstaculo *obstaculo1 , Obstaculo *obstaculo2 , Tela *telaPrincipal){
    if(obstaculo->aparecer){
        obstaculo->x_obstaculo += velocidade;
        if(obstaculo->x_obstaculo > telaPrincipal->x_tela){
            obstaculo->x_obstaculo = 0;
        }
        obstaculo->centroObstaculo = (Vector2){obstaculo->x_obstaculo , obstaculo->y_obstaculo};
    }

    if(obstaculo1->aparecer){
        obstaculo1->y_obstaculo += velocidade;
        if(obstaculo1->y_obstaculo > telaPrincipal->y_tela){
            obstaculo1->y_obstaculo = 0;
        }
        obstaculo1->centroObstaculo = (Vector2){obstaculo1->x_obstaculo , obstaculo1->y_obstaculo};
    }

    if(obstaculo2->aparecer){
        obstaculo2->x_obstaculo -= velocidade;
        if(obstaculo2->x_obstaculo < 0){
            obstaculo2->x_obstaculo = telaPrincipal->x_tela;
        }
        obstaculo2->centroObstaculo = (Vector2){obstaculo2->x_obstaculo , obstaculo2->y_obstaculo};
    }
}
