#include<stdio.h>
#include<stdlib.h>

void menu(){
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n");
}

void tabuleiro(){

}

void iniciarJogo(){
    tabuleiro();
}

void selecionar(){
    int selecao;
    scanf("%d", &selecao);
    switch(selecao){
        case 1:{
            iniciarJogo();
        }
        case 2:{

        }
        case 3:{

        }
        case 4:{

        }
        default:{
            printf("Opcao invalida\n");
        }
    }

}
int main(){
    menu();
    selecionar();
    
}