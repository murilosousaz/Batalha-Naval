#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char PORTA_AVIOES[20];
    char NAVIO_TANQUE[20];
    char SUBMARINO[20];
    char BOTE[20];
} Navios;

void menu(){
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n");
}

void tabuleiro(){
    char tab[10][10];
    int i, j;
    for(i = 0; i < 10; i++){
        for(j = 0; j < 10; j++){
            tab[i][j] = '~';
        }
    }
    for(i = 0; i < 10; i++){
        printf("%d ", i + 1);
        for(j = 0; j < 10; j++){
            printf("%c ", tab[i][j]);
        }
        printf("\n");
    }
}

void selecionarNavio(){
    Navios navios = {"Porta Avioes", "Navio Tanque", "Submarino", "Bote"};
    int select;

    tabuleiro();

    printf("Escolha o seu navio:\n");
    printf("(1) - Porta Avioes\n(2) - Navio Tanque\n(3) - Submarino\n(4) - Bote\n");
    scanf("%d", &select);

    if(select == 1){
        printf("Voce escolheu %s\n", navios.PORTA_AVIOES);
    } else if(select == 2){
        printf("Voce escolheu %s\n", navios.NAVIO_TANQUE);
    } else if(select == 3){
        printf("Voce escolheu %s\n", navios.SUBMARINO);
    } else if(select == 4){
        printf("Voce escolheu %s\n", navios.BOTE);
    } else {
        printf("Opcao invalida.\n");
    }
}

void iniciarJogo(){
    tabuleiro();
    selecionarNavio();
}

void continuarJogo(){

}

void fazerJogada(){

}

void exibirInstrucoes(){
    printf("--------------------------------------------------\n");
    printf("BEM-VINDO(A) A BATALHA NAVAL\n");
    printf("--------------------------------------------------\n\n");

    printf("Objetivo do Jogo\n");
    printf("Seu objetivo e afundar todos os navios do seu oponente.\n\n");

    printf("O Tabuleiro\n");
    printf("Coordenadas podem ser digitadas como XY ou letra e numero.\n");
    printf("Ha um tabuleiro para seus navios e outro para registrar tiros.\n\n");

    printf("Simbolos\n");
    printf("~ : Agua\n");
    printf("O : Navio (somente no seu tabuleiro)\n");
    printf("X : Acerto\n");
    printf("- : Erro\n\n");

    printf("Como Jogar\n");
    printf("1. Posicione seus navios.\n");
    printf("2. Escolha coordenadas para atacar.\n");
    printf("3. X = acerto, - = erro.\n\n");

    printf("Vencedor\n");
    printf("Ganha quem afundar todos os navios inimigos.\n\n");

    printf("--------------------------------------------------\n");
    printf("BOA SORTE, MARUJO\n");
    printf("--------------------------------------------------\n");
}

void sair(){
    printf("Encerrando o jogo...\n");
    printf("Obrigado por jogar Batalha Naval!\n");
    exit(0);
}

void selecionar(){
    int selecao;
    scanf("%d", &selecao);

    if(selecao == 1){
        iniciarJogo();
    } else if(selecao == 2){
        continuarJogo();
    } else if(selecao == 3){
        exibirInstrucoes();
    } else if(selecao == 4){
        sair();
    } else {
        printf("Opcao invalida.\n");
    }
}

int main(){
    menu();
    selecionar();
    return 0;
}