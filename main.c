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

typedef struct{
    Jogador p1;
    Jogador p2;
    int turno;
}SaveGame;

void menu();
void selecionar();
void iniciarJogo();
void instrucoes();
void limparTela();
void limparBuffer();
void pausar();
void inicializarTabuleiro(Jogador *j);
void tabuleiroCompleto(Jogador *j);
void tabuleiroOculto(Jogador *j);
void posicionarTodosNavios(Jogador *j);
void salvarJogo(Jogador *p1, Jogador *p2, int turno);
bool carregarJogo(Jogador *p1, Jogador *p2, int *turno);
void posicionarNavio(Jogador *j, int tamanho, const char *nomeNavio);
bool areaOcupada(Jogador *player, int linha, int coluna, int tamanho, char dir);
void processarTurno(Jogador *atacante, Jogador *defensor);

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("Pressione Enter para continuar...");
    limparBuffer();
    getchar();
}

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu() {
    printf("\n BEM-VINDO A BATALHA NAVAL \n");
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n\n");
}

void inicializarTabuleiro(Jogador *j) {
    for (int i = 0; i < TAM; i++)
        for (int k = 0; k < TAM; k++)
            j->tabuleiro[i][k] = '~';
}

void imprimirCabecalho() {
    printf("\n   A B C D E F G H I J\n");
}

void tabuleiroCompleto(Jogador *j) {
    imprimirCabecalho();
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i + 1);
        for (int c = 0; c < TAM; c++)
            printf("%c ", j->tabuleiro[i][c]);
        printf("\n");
    }
}

void tabuleiroOculto(Jogador *j) {
    imprimirCabecalho();
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i + 1);
        for (int c = 0; c < TAM; c++) {
            char cel = j->tabuleiro[i][c];
            printf("%c ", cel == 'O' ? '~' : cel);
        }
        printf("\n");
    }
}

bool areaOcupada(Jogador *player, int linha, int coluna, int tamanho, char dir) {
    for (int k = 0; k < tamanho; k++) {
        int r = linha + (dir == 'V' ? k : 0);
        int c = coluna + (dir == 'H' ? k : 0);

        for (int a = r - 1; a <= r + 1; a++) {
            for (int b = c - 1; b <= c + 1; b++) {
                if (a < 0 || a >= TAM || b < 0 || b >= TAM)
                    continue;

                if (player->tabuleiro[a][b] == 'O')
                    return true;
            }
        }
    }
    return false;
}

void posicionarNavio(Jogador *j, int tamanho, const char *nome) {
    while (1) {
        limparTela();
        printf("\nPosicionar %s (%d celulas)\n", nome, tamanho);
        tabuleiroCompleto(j);

        char letra, dir;
        int linha;

        printf("\nDigite a coordenada inicial (Ex: A 5): ");
        if (scanf(" %c %d", &letra, &linha) != 2) {
            limparBuffer();
            continue;
        }

        letra = toupper(letra);
        int col = letra - 'A';
        int row = linha - 1;

        printf("Direcao (H/V): ");
        if (scanf(" %c", &dir) != 1) {
            limparBuffer();
            continue;
        }

        dir = toupper(dir);

        if (row < 0 || row >= TAM || col < 0 || col >= TAM) {
            printf("Coordenada invalida!\n");
            pausar();
            continue;
        }
        if (dir == 'H' && col + tamanho > TAM) {
            printf("Nao cabe horizontalmente!\n");
            pausar();
            continue;
        }
        if (dir == 'V' && row + tamanho > TAM) {
            printf("Nao cabe verticalmente!\n");
            pausar();
            continue;
        }
        if (areaOcupada(j, row, col, tamanho, dir)) {
            printf("Navio encostaria em outro!\n");
            pausar();
            continue;
        }

        for (int k = 0; k < tamanho; k++) {
            int r = row + (dir == 'V' ? k : 0);
            int c = col + (dir == 'H' ? k : 0);
            j->tabuleiro[r][c] = 'O';
        }

        printf("%s posicionado!\n", nome);
        pausar();
        break;
    }
}

void posicionarTodosNavios(Jogador *j) {
    printf("\n--- POSICIONAMENTO DE %s ---\n", j->nome);
    posicionarNavio(j, 5, "Porta-Avioes");
    posicionarNavio(j, 4, "Navio-Tanque");
    posicionarNavio(j, 3, "Submarino");
    posicionarNavio(j, 2, "Bote");
}

void processarTurno(Jogador *atacante, Jogador *defensor) {
    while (1) {
        limparTela();
        printf("\nVEZ DE: %s\n", atacante->nome);

        printf("\nTabuleiro inimigo:\n");
        tabuleiroOculto(defensor);

        char letra;
        int linha;

        printf("\nCoordenada do tiro (ex: A 5): ");
        if (scanf(" %c %d", &letra, &linha) != 2) {
            limparBuffer();
            continue;
        }

        letra = toupper(letra);
        int col = letra - 'A';
        int row = linha - 1;

        if (row < 0 || row >= TAM || col < 0 || col >= TAM) {
            printf("Fora do mapa!\n");
            pausar();
            continue;
        }

        char *cel = &defensor->tabuleiro[row][col];

        if (*cel == 'X' || *cel == '.') {
            printf("Voce ja atirou aqui!\n");
            pausar();
            continue;
        }

        if (*cel == 'O') {
            printf("\n*** ACERTOU! ***\n");
            *cel = 'X';
            defensor->naviosRestantes--;
        } else {
            printf("\nTiro na agua...\n");
            *cel = '.';
        }

        pausar();
        break;
    }
}

void iniciarJogo() {
    limparTela();
    Jogador p1, p2;

    printf("Nome do Jogador 1: ");
    scanf(" %s", p1.nome);

    printf("Nome do Jogador 2: ");
    scanf(" %s", p2.nome);

    inicializarTabuleiro(&p1);
    inicializarTabuleiro(&p2);

    posicionarTodosNavios(&p1);
    posicionarTodosNavios(&p2);

    p1.naviosRestantes = 14;
    p2.naviosRestantes = 14;

    int turno = 1;
    while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) {
        if (turno % 2)
            processarTurno(&p1, &p2);
        else
            processarTurno(&p2, &p1);

        turno++;

        salvarJogo(&p1, &p2, &turno);
    }

    limparTela();
    printf("\n===== FIM DE JOGO =====\n");

    if (p1.naviosRestantes > 0)
        printf("VENCEDOR: %s\n", p1.nome);
    else
        printf("VENCEDOR: %s\n", p2.nome);

    pausar();
}

void salvarJogo(Jogador *p1, Jogador *p2, int turno){
    SaveGame save;
    save.p1 = *p1;
    save.p2 = *p2;
    save.turno = turno;

    FILE *arq = fopen("save.bin", "wb");

    if(arq == NULL){
        printf("Erro ao salvar jogo");
        return;
    }

    fwrite(&save, sizeof(SaveGame), 1, arq);
    fclose(arq);

    printf("Jogo salvo com sucesso");
}

bool carregarJogo(Jogador *p1, Jogador *p2, int *turno){
    FILE *arq = fopen("save.bin", "rb");

    if(arq == NULL){
        printf("Nenhum jogo salvo encontrado!");
        return false;
    }

    SaveGame save;
    fread(&save, sizeof(SaveGame), 1, arq);
    fclose(arq);

    *p1 = save.p1;
    *p2 = save.p2;
    *turno = save.turno;

    printf("Jogo carregado com sucesso");
    return true;
}

void instrucoes() {
    limparTela();
    printf("======= INSTRUCOES =======\n");
    printf("1 - Derrube os navios do adversario.\n");
    printf("2 - Cada jogador posiciona seus navios.\n");
    printf("3 - Ataques usando coordenadas (A5, D7...).\n");
    printf("4 - Vence quem destruir todos os navios.\n");
    printf("==========================\n");
    printf("Pressione ENTER...\n");

    limparBuffer();
    getchar();

    menu();
}

void selecionar() {
    int op;
    printf("Selecione uma opcao: ");
    scanf("%d", &op);

    switch (op) {
        case 1: {
            iniciarJogo(); 
            break;
        }
        case 2: {
            Jogador p1, p2;
            int turno;

            if (carregarJogo(&p1, &p2, turno)){
                while(p1.naviosRestantes > 0 && p2.naviosRestantes > 0){
                    if(turno % 2){
                        processarTurno(&p1, &p2);
                    }else{
                        processarTurno(&p2, &p1);
                    }
                }
                turno++;
                salvarJogo(&p1, &p2, &turno);
            }
            limparTela();
            printf("\n===== FIM DE JOGO =====\n");
            if(p1.naviosRestantes > 0){
                printf("VENCEDOR: %s\n", p1.nome);
            }else{
                printf("VENCEDOR: %s\n", p2.nome);
            }
            pausar();
            break;
        }
        case 3: {
            instrucoes(); 
            break;
        }
        case 4: {
            exit(0);
        }
        default: {
            printf("Opcao invalida!\n");
        }
    }
}

int main() {
    menu();
    selecionar();
    return 0;
}
