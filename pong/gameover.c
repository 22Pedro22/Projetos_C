#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "estrutura.h"
#include "gameover.h"

void gameOver(Tela *tela){
    
    Texto texto1;

    texto1.xTexto = tela->xTela / 5;
    texto1.yTexto = tela->yTela / 2.5;
    texto1.fonteTexto = 80;
    texto1.corTexto = WHITE;
    strcpy(texto1.texto , "GAME OVER");

    DrawText(texto1.texto , texto1.xTexto , texto1.yTexto , texto1.fonteTexto , texto1.corTexto);

}