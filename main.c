#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include "navios.h"

void menu(){
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n");
}

void tabuleiro(){
    char tab[10][10];
    char i, j;
    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j++){
            tab[i][j] = '.';
        }
    }

    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j++){
            printf("%c ", tab[i][j]);
        }
        printf("\n");
    }
}

void iniciarJogo(){
    tabuleiro();
}

void fazerJogada(){

}

void selecionar(){
    int selecao;
    scanf("%d", &selecao);
    switch(selecao){
        case 1:{
            iniciarJogo();
            fazerJogada();
        }
        case 2:{

        }
        case 3:{
            exibirInstrucoes();
        }
        case 4:{
            sair();
        }
    }

}
void selecionarNavio(){

}

void continuarJogo(){

}

void exibirInstrucoes()
{
    printf("##################################################\n");
    printf("##            BEM VINDO A BATALHA NAVAL          ##\n");
    printf("##################################################\n\n");

    printf("=== Objetivo do Jogo ===\n");
    printf("Seu objetivo e afundar todos os navios do seu oponente\n");
    printf("O primeiro jogador a destruir toda a frota adversaria vence a partida\n\n");

    printf("=== O Tabuleiro ===\n");
    printf("As coordenadas podem ser inseridas no formato\n");
    printf("  Letra e numero ex C7\n");
    printf("  Dois numeros ex 3 7\n");
    printf("Voce tera um tabuleiro para posicionar seus navios e outro para registrar seus tiros\n\n");

    printf("=== Simbolos no Tabuleiro ===\n");
    printf(".  Agua vazio\n");
    printf("O  Parte de um navio visivel apenas no seu tabuleiro\n");
    printf("X  Tiro que acertou um navio\n");
    printf("-  Tiro na agua erro\n\n");

    printf("=== Como Jogar ===\n");
    printf("1 Posicionamento Cada jogador posiciona sua frota\n");
}

void sair(){
    printf("Encerrando o jogo...\n");
    printf("Obrigado por jogar Batalha Naval!\n");
    exit(0);
}

int main(){
    menu();
    selecionar();   
}