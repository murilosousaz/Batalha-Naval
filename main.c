#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define TAM 10

typedef struct {
    char nome[50];
    char tabuleiro[TAM][TAM];
    int naviosRestantes;
} Jogador;

void menu();
void inicializarTabuleiro(Jogador *j);
void tabuleiroCompleto(Jogador *j);
void tabuleiroOculto(Jogador *j);
void processarTurno(Jogador *atacante, Jogador *defensor);
void posicionarNavio(Jogador *j, int tamanho, const char *nomeNavio);
void posicionarTodosNavios(Jogador *j);
void iniciarJogo();
void selecionar();

void menu() {
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n\n");
}

void inicializarTabuleiro(Jogador *j) {
    for (int i = 0; i < TAM; i++) {
        for (int j2 = 0; j2 < TAM; j2++) {
            j->tabuleiro[i][j2] = '~';
        }
    }
}

void tabuleiroCompleto(Jogador *j) {
    printf("\n  A B C D E F G H I J\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", i + 1);
        for (int j2 = 0; j2 < TAM; j2++) {
            printf("%c ", j->tabuleiro[i][j2]);
        }
        printf("\n");
    }
}

void tabuleiroOculto(Jogador *j) {
    printf("\n  A B C D E F G H I J (OPONENTE)\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d ", i + 1);
        for (int j2 = 0; j2 < TAM; j2++) {
            char celula = j->tabuleiro[i][j2];

            if (celula == 'O') {
                printf("~ ");
            } else {
                printf("%c ", celula);
            }
        }
        printf("\n");
    }
}

void processarTurno(Jogador *atacante, Jogador *defensor) {
    char letra;
    int linha;
    int i, j;

    printf("\nVEZ DE: %s\n", atacante->nome);

    printf("\nSeu tabuleiro:\n");
    tabuleiroCompleto(atacante);

    printf("\nTabuleiro do oponente:\n");
    tabuleiroOculto(defensor);

    while (1) {
        printf("\nDigite a coordenada do tiro (ex: A 5): ");
        scanf(" %c %d", &letra, &linha);

        letra = toupper(letra);
        j = letra - 'A';
        i = linha - 1;

        if (i < 0 || i >= TAM || j < 0 || j >= TAM) {
            printf(">> Coordenada fora do mapa! Tente novamente.\n");
            continue;
        }

        if (defensor->tabuleiro[i][j] == 'X' || defensor->tabuleiro[i][j] == '.') {
            printf(">> Voce ja atirou aqui!\n");
            continue;
        }

        break;
    }

    if (defensor->tabuleiro[i][j] == 'O') {
        printf("\n*** ACERTOU! ***\n");
        defensor->tabuleiro[i][j] = 'X';
        defensor->naviosRestantes--;
    } else {
        printf("\nTiro na agua...\n");
        defensor->tabuleiro[i][j] = '.';
    }
}

void iniciarJogo() {
    Jogador p1, p2;

    printf("Nome do Jogador 1: ");
    scanf(" %49s", p1.nome);

    printf("Nome do Jogador 2: ");
    scanf(" %49s", p2.nome);

    inicializarTabuleiro(&p1);
    inicializarTabuleiro(&p2);

    posicionarTodosNavios(&p1);
    posicionarTodosNavios(&p2);

    p1.naviosRestantes = 14;
    p2.naviosRestantes = 14;

    printf("\nTabuleiro final do jogador 1 (completo):\n");
    tabuleiroCompleto(&p1);

    processarTurno(&p1, &p2);
}

void posicionarTodosNavios(Jogador *j) {
    printf("\n--- POSICIONAMENTO DE NAVIOS (%s) ---\n", j->nome);

    posicionarNavio(j, 5, "Porta-Avioes");
    posicionarNavio(j, 4, "Navio-Tanque");
    posicionarNavio(j, 3, "Submarino");
    posicionarNavio(j, 2, "Bote");
}

void posicionarNavio(Jogador *player, int tamanho, const char *nomeNavio) {
    char letra, direcao;
    int i, j;

    bool ok = false;

    while (!ok) {
        printf("\nPosicionar %s (%d células)\n", nomeNavio, tamanho);
        printf("Digite a coordenada inicial (Ex: A 5): ");
        scanf(" %c %d", &letra, &i);

        letra = toupper(letra);

        j = letra - 'A';
        i = i - 1;

        printf("Direção (H = horizontal, V = vertical): ");
        scanf(" %c", &direcao);
        direcao = toupper(direcao);

        if (i < 0 || i >= TAM || j < 0 || j >= TAM) {
            printf("Coordenada inválida!\n");
            continue;
        }

        if (direcao == 'H' && j + tamanho > TAM) {
            printf("Não cabe horizontalmente!\n");
            continue;
        }

        if (direcao == 'V' && i + tamanho > TAM) {
            printf("Não cabe verticalmente!\n");
            continue;
        }

        bool proibido = false;

        for (int k = 0; k < tamanho; k++) {
            int ii = i + (direcao == 'V' ? k : 0);
            int jj = j + (direcao == 'H' ? k : 0);

            for (int a = ii - 1; a <= ii + 1; a++) {
                for (int b = jj - 1; b <= jj + 1; b++) {

                    if (a < 0 || a >= TAM || b < 0 || b >= TAM)
                        continue;

                    if (player->tabuleiro[a][b] == 'O') {
                        proibido = true;
                    }
                }
            }
        }

        if (proibido) {
            printf("Navio muito proximo de outro! Nao eh permitido encostar.\n");
            continue;
        }

        for (int k = 0; k < tamanho; k++) {
            int ii = i + (direcao == 'V' ? k : 0);
            int jj = j + (direcao == 'H' ? k : 0);
            player->tabuleiro[ii][jj] = 'O';
        }

        printf("%s posicionado com sucesso!\n", nomeNavio);
        ok = true;
    }
}

void selecionar() {
    int op;
    scanf("%d", &op);

    switch (op) {
        case 1: iniciarJogo(); break;
        case 2: printf("Continuar jogo ainda não implementado.\n"); break;
        case 3: printf("Instruções ainda não implementadas.\n"); break;
        case 4: exit(0);
        default: printf("Opção inválida!\n");
    }
}

int main() {
    menu();
    selecionar();
    return 0;
}
