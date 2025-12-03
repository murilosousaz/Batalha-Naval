#include "batalha_naval.h"

void limparBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar(){
    printf("Pressione ENTER para continuar...");
    limparBuffer(); 
    getchar();
}

void limparTela(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu() {
    printf("\n===== BATALHA NAVAL =====\n");
    printf("1 - Novo Jogo\n");
    printf("2 - Continuar Jogo\n");
    printf("3 - Instrucoes\n");
    printf("4 - Sair\n\n");
}