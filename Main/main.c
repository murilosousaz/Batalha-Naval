#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define TAM 10
typedef struct {
    char nome[50];
    char tabuleiro[TAM][TAM];
    int naviosRestantes;
} Jogador;

typedef struct {
    Jogador p1, p2;
    int turno;
} SaveGame;

void menu();
void selecionar();
void iniciarJogo();
void instrucoes();
void limparTela();
void limparBuffer();
void pausar();

void inicializarTabuleiro(Jogador *j);
void imprimirCabecalho();
void tabuleiroCompleto(Jogador *j);
void tabuleiroOculto(Jogador *j);

bool areaOcupada(Jogador *player, int l, int c, int tam, char dir);
void posicionarNavio(Jogador *j, int tamanho, const char *nomeNavio);
void posicionarTodosNavios(Jogador *j);

void processarTurno(Jogador *atacante, Jogador *defensor, Jogador *p1, Jogador *p2, int *turno);

void salvarJogo(Jogador *p1, Jogador *p2, int turno);
bool carregarJogo(Jogador *p1, Jogador *p2, int *turno);

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
            printf("%c ", (cel == 'O') ? '~' : cel);
        }
        printf("\n");
    }
}

bool areaOcupada(Jogador *player, int linha, int coluna, int tamanho, char dir){
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

void posicionarNavio(Jogador *j, int tamanho, const char *nome){
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
        printf("Selecione a direção do navio: Horizontal ou Vertical (H/V): ");

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

void posicionarTodosNavios(Jogador *j){
    limparTela();

    printf("=====================================\n");
    printf("     POSICIONAMENTO DE NAVIOS\n");
    printf("        Jogador: %s\n", j->nome);
    printf("=====================================\n\n");

    pausar();

    posicionarNavio(j, 5, "Porta-Avioes");
    posicionarNavio(j, 4, "Navio-Tanque");
    posicionarNavio(j, 3, "Submarino");
    posicionarNavio(j, 2, "Bote");
}

void processarTurno(Jogador *atacante, Jogador *defensor, Jogador *p1, Jogador *p2, int *turno){
    while (1) {
        limparTela();
        printf("\nVEZ DE: %s\n", atacante->nome);

        printf("\n(Digite '0 0' para salvar e sair)\n");
        tabuleiroOculto(defensor);

    char entrada[10];

    printf("\nCoordenada do tiro (ex: A 5) ou 'save' para salvar: ");
    scanf("%s", entrada);

    if (strcmp(entrada, "save") == 0) {
        printf("\nSalvando jogo...\n");
        salvarJogo(p1, p2, *turno);
        printf("Jogo salvo com sucesso!\n");
        pausar();
        exit(0);
        }

    char letra = toupper(entrada[0]);
    int linha;

    if (scanf("%d", &linha) != 1) {
        limparBuffer();
        printf("Entrada inválida!\n");
        pausar();
        continue;
        }

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

void salvarJogo(Jogador *p1, Jogador *p2, int turno){
    SaveGame save = {*p1, *p2, turno};

    FILE *arq = fopen("save.bin", "wb");
    if (!arq) {
        printf("Erro ao salvar jogo\n");
        return;
    }

    fwrite(&save, sizeof(SaveGame), 1, arq);
    fclose(arq);
}

bool carregarJogo(Jogador *p1, Jogador *p2, int *turno) {
    FILE *arq = fopen("save.bin", "rb");
    if (!arq)
        return false;

    SaveGame save;
    fread(&save, sizeof(SaveGame), 1, arq);
    fclose(arq);

    *p1 = save.p1;
    *p2 = save.p2;
    *turno = save.turno;

    return true;
}

void iniciarJogo(){
    limparTela();
    Jogador p1, p2;

    limparBuffer();

    printf("Nome do Jogador 1: ");
    fgets(p1.nome, sizeof(p1.nome), stdin);
    p1.nome[strcspn(p1.nome, "\n")] = '\0';

    printf("Nome do Jogador 2: ");
    fgets(p2.nome, sizeof(p2.nome), stdin);
    p2.nome[strcspn(p2.nome, "\n")] = '\0';


    inicializarTabuleiro(&p1);
    inicializarTabuleiro(&p2);

    posicionarTodosNavios(&p1);
    posicionarTodosNavios(&p2);

    p1.naviosRestantes = 14;
    p2.naviosRestantes = 14;

    int turno = 1;

    while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) {
        if (turno % 2)
            processarTurno(&p1, &p2, &p1, &p2, &turno);
        else
            processarTurno(&p2, &p1, &p1, &p2, &turno);

        turno++;

        salvarJogo(&p1, &p2, turno);
    }

    limparTela();
    printf("\n===== FIM DE JOGO =====\n");
    printf("VENCEDOR: %s\n", (p1.naviosRestantes > 0) ? p1.nome : p2.nome);
    pausar();
}

void instrucoes(){
  limparTela();

  printf("##################################################\n");
  printf("##  BEM-VINDO(A) À BATALHA NAVAL!  ##\n");
  printf("##################################################\n\n");

  printf("### Objetivo do Jogo ###\n");
  printf("Seu objetivo\n afundar todos os navios do seu oponente.\n");
  printf("O primeiro jogador a afundar todos os navios do adversario vence a partida.\n\n");

  printf("### O Tabuleiro ###\n");
  printf("* As coordenadas sao referenciadas por uma \n");
  printf("* Voce tera um tabuleiro para posicionar seus navios e um tabuleiro 'de tiro' para registrar seus disparos no oponente.\n\n");

  printf("### Simbolos no Tabuleiro ###\n");
  printf("Durante o jogo, voce vera os seguintes simbolos:\n");
  printf("**~** (Vazio): Agua, local onde voce ainda nao atirou ou nao ha navio.\n");
  printf("* **O** (Navio): Parte de um navio (apenas no seu tabuleiro).\n");
  printf("* **X** (Atingido): Um tiro que acertou uma parte de um navio.\n");
  printf("* **.** (Agua/Erro): Um tiro que caiu na agua (nao acertou um navio).\n\n");

  printf("### Como Jogar ###\n");
  printf("1.  **Posicionamento:** No inicio, voce posicionara seus navios no seu tabuleiro (porta-avioes (5), Navio- tanque(4), Submarino (3) e bote (2)). O outro jogador fara o mesmo.\n");
  printf("2.  **O Ataque:** Em sua vez, voce digitara a coordenada para onde deseja atirar (exemplo: **C7**) o formato Letra e Letra (Ex: AA), Numero e Numero (1 1) ou Numero e Letra (1 A) não serão lidos.\n");
  printf("3.  **Resultado:**\n");
  printf("    * Se acertar um navio:A posicao sera marcada com **X**.\n");
  printf("    * Se errar: A posicao sera marcada com **-**.\n");
  printf("4.  **Afundando:** Quando todas as partes de um navio forem atingidas, ele estara **AFUNDADO**!\n\n");
  printf("5.  **Vezes:** Os jogadores se revezam, atacando um de cada vez.\n\n");
  printf("### Dois navios nao podem ocupar a mesma posicao.\n");
  printf("### Dois navios nao podem ocupar posições adjacentes, ou seja, não será permitido posicionar navios lado a lado.\n");
  printf("### Um navio nao pode ser posicionado fora do tabuleiro.\n\n");
  printf("### Vencedor\n");
  printf("O jogo termina quando um jogador consegue afundar **TODOS** os navios do adversario.\n\n");

  printf("##################################################\n");
  printf("##  BOA SORTE, MARUJO!  ##\n");
  printf("##################################################\n");

  printf("##################################################\n");
  printf("##  Aperte Enter para voltar ao Menu  ##\n");
  printf("##################################################\n");

  limparBuffer();
  getchar();

  menu();
}

void selecionar() {
    int op;
    printf("Selecione uma opcao: ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            iniciarJogo();
            break;

        case 2: {
            Jogador p1, p2;
            int turno;

            if (carregarJogo(&p1, &p2, &turno)) {
                while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) {
                    if (turno % 2)
                        processarTurno(&p1, &p2, &p1, &p2, &turno);
                    else
                        processarTurno(&p2, &p1, &p1, &p2, &turno);

                    turno++;
                }
            } else {
                printf("Nenhum jogo salvo encontrado!\n");
                pausar();
            }
            break;
        }

        case 3:
            instrucoes();
            break;

        case 4:
            exit(0);

        default:
            printf("Opcao invalida!\n");
            break;
    }
}

int main(){
  int op = 0;

  do{

    limparTela();

    menu();

    printf("\nSelecione uma opcao: ");
    if (scanf("%d", &op) != 1)
    {
      limparBuffer();
      op = 0;
    }
    switch (op)
    {
    case 1:
      iniciarJogo();
      break;

    case 2:
      printf("\nContinuar jogo nao implementado.\n");
      pausar();
      break;

    case 3:
      instrucoes();
      break;

    case 4:
      printf("\nSaindo do jogo...\n");
      break;

    default:
      printf("\nOpcao invalida! Tente novamente.\n");
      pausar();
      break;
    }

  } while (op != 4);

  return 0;
}
