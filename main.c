#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define TAM 10

typedef struct
{
    char nome[50];
    char tabuleiro[TAM][TAM];
    int naviosRestantes;
} Jogador;

void menu();
void selecionar();
void iniciarJogo();
void limparTela();
void limparBuffer();
void pausar();
void inicializarTabuleiro(Jogador *j);
void tabuleiroCompleto(Jogador *j);
void tabuleiroOculto(Jogador *j);
void posicionarTodosNavios(Jogador *j);
void posicionarNavio(Jogador *player, int tamanho, const char *nomeNavio);
void processarTurno(Jogador *atacante, Jogador *defensor);

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
    }
}

void pausar()
{
    printf("Pressione Enter para continuar...");
    limparBuffer();
    getchar();
}

void limparTela()
{
#ifdef _WIN32
    system("cls");
#endif
}

void menu()
{
    printf("(1) - Novo Jogo\n");
    printf("(2) - Continuar Jogo\n");
    printf("(3) - Instrucoes\n");
    printf("(4) - Sair\n\n");
}

void inicializarTabuleiro(Jogador *j)
{
    for (int i = 0; i < TAM; i++)
        for (int k = 0; k < TAM; k++)
            j->tabuleiro[i][k] = '~';
}

void tabuleiroCompleto(Jogador *j)
{
    printf("\n   A B C D E F G H I J\n");
    for (int i = 0; i < TAM; i++)
    {
        printf("%2d ", i + 1);
        for (int j2 = 0; j2 < TAM; j2++)
            printf("%c ", j->tabuleiro[i][j2]);
        printf("\n");
    }
}

void tabuleiroOculto(Jogador *j)
{
    printf("\n  A B C D E F G H I J (OPONENTE)\n");
    for (int i = 0; i < TAM; i++)
    {
        printf("%2d ", i + 1);
        for (int j2 = 0; j2 < TAM; j2++)
        {
            char celula = j->tabuleiro[i][j2];
            printf("%c ", celula == 'O' ? '~' : celula);
        }
        printf("\n");
    }
}

bool areaOcupada(Jogador *player, int i, int j, int tamanho, char dir)
{
    for (int k = 0; k < tamanho; k++)
    {
        int ii = i + (dir == 'V' ? k : 0);
        int jj = j + (dir == 'H' ? k : 0);

        for (int a = ii - 1; a <= ii + 1; a++)
        {
            for (int b = jj - 1; b <= jj + 1; b++)
            {
                if (a < 0 || a >= TAM || b < 0 || b >= TAM)
                    continue;

                if (player->tabuleiro[a][b] == 'O')
                    return true;
            }
        }
    }
    return false;
}

void posicionarNavio(Jogador *player, int tamanho, const char *nomeNavio)
{
    while (1)
    {
        limparTela();

        printf("\nPosicionar %s (%d celulas)\n\n", nomeNavio, tamanho);
        tabuleiroCompleto(player);

        char letra, direcao;
        int linha;

        printf("\nDigite a coordenada inicial (Ex: A 5): ");
        if (scanf(" %c %d", &letra, &linha) != 2)
        {
            limparBuffer();
            continue;
        }

        letra = toupper(letra);
        int col = letra - 'A';
        int row = linha - 1;

        printf("Direcao (H = horizontal, V = vertical): ");
        if (scanf(" %c", &direcao) != 1)
        {
            limparBuffer();
            continue;
        }
        direcao = toupper(direcao);

        if (row < 0 || row >= TAM || col < 0 || col >= TAM)
        {
            printf("Coordenada invalida!\n");
            pausar();
            continue;
        }
        if (direcao == 'H' && col + tamanho > TAM)
        {
            printf("Nao cabe horizontalmente!\n");
            pausar();
            continue;
        }
        if (direcao == 'V' && row + tamanho > TAM)
        {
            printf("Nao cabe verticalmente!\n");
            pausar();
            continue;
        }
        if (areaOcupada(player, row, col, tamanho, direcao))
        {
            printf("Navio nao pode encostar em outro!\n");
            pausar();
            continue;
        }

        for (int k = 0; k < tamanho; k++)
        {
            int ii = row + (direcao == 'V' ? k : 0);
            int jj = col + (direcao == 'H' ? k : 0);
            player->tabuleiro[ii][jj] = 'O';
        }

        printf("%s posicionado com sucesso!\n", nomeNavio);
        pausar();
        break;
    }
}

void posicionarTodosNavios(Jogador *j)
{
    printf("\n--- POSICIONAMENTO DE NAVIOS PARA: %s ---\n", j->nome);

    posicionarNavio(j, 5, "Porta-Avioes");
    posicionarNavio(j, 4, "Navio-Tanque");
    posicionarNavio(j, 3, "Submarino");
    posicionarNavio(j, 2, "Bote");
}

void processarTurno(Jogador *atacante, Jogador *defensor)
{
    while (1)
    {
        limparTela();

        printf("\nVEZ DE: %s\n", atacante->nome);

        printf("\nSeu tabuleiro:\n");
        tabuleiroCompleto(atacante);
        limparTela();
        tabuleiroOculto(defensor);

        char letra;
        int linha;

        printf("\nDigite a coordenada do tiro (ex: A 5): ");
        if (scanf(" %c %d", &letra, &linha) != 2)
        {
            limparBuffer();
            continue;
        }

        letra = toupper(letra);
        int j = letra - 'A';
        int i = linha - 1;

        if (i < 0 || i >= TAM || j < 0 || j >= TAM)
        {
            printf(">> Coordenada fora do mapa!\n");
            pausar();
            continue;
        }
        if (defensor->tabuleiro[i][j] == 'X' || defensor->tabuleiro[i][j] == '.')
        {
            printf(">> Voce ja atirou aqui!\n");
            pausar();
            continue;
        }

        if (defensor->tabuleiro[i][j] == 'O')
        {
            printf("\n*** ACERTOU! ***\n");
            defensor->tabuleiro[i][j] = 'X';
            defensor->naviosRestantes--;
        }
        else
        {
            printf("\nTiro na agua...\n");
            defensor->tabuleiro[i][j] = '.';
        }

        pausar();
        break;
    }
}

void iniciarJogo()
{
    limparTela();
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

    printf("\nPosicionamento concluido! Iniciando jogo...\n");
    pausar();

    int turno = 1;

    while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) // COLOQUEI SÓ ESSA CONDIÇÃO AQUI   -LUCAS
    {

        if (turno % 2 != 0)
        {
            processarTurno(&p1, &p2);
        }
        else
        {
            processarTurno(&p2, &p1);
        }

        turno++;
    }

    limparTela();
    printf("\n=================================\n");
    printf("       FIM DE JOGO! \n");
    printf("=================================\n");

    if (p1.naviosRestantes > 0)
    {
        printf("PARABENS! %s VENCEU A BATALHA NAVAL!\n", p1.nome);
    }
    else
    {
        printf("PARABENS! %s VENCEU A BATALHA NAVAL!\n", p2.nome);
    }

    pausar();
}

void selecionar()
{
    int op;
    printf("Selecione uma opcao: ");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
    {
        iniciarJogo();
        break;
    }
    case 2:
    {
        printf("Continuar jogo nao implementado.\n");
        break;
    }
    case 3:
    {
        Instrucoes();
        break;
    }
    case 4:
    {
        exit(0);
    }
    default:
    {
        printf("Opcao invalida!\n");
    }
    }
}

void Instrucoes()
{
}

int main()
{
    menu();
    selecionar();
    return 0;
}
