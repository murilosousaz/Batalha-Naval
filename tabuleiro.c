#include "batalha_naval.h"

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

        // --- Passo 1: Leitura da Coordenada (A 5) ---
        printf("\nDigite a coordenada inicial (Ex: A 5): ");
        if (scanf(" %c %d", &letra, &linha) != 2) { 
            printf("Entrada inválida! Tente novamente.\n");
            limparBuffer(); 
            pausar();
            continue;
        }
        limparBuffer(); // CRUCIAL: Limpa o buffer após a leitura da coordenada

        letra = toupper(letra);
        int col = letra - 'A';
        int row = linha - 1;

        if (row < 0 || row >= TAM || col < 0 || col >= TAM) {
            printf("Coordenada inicial invalida!\n");
            pausar();
            continue;
        }

        // --- Passo 2: Leitura da Direção (H/V) - O "BOTÃO" ---
        printf("Selecione a direção do navio: Horizontal ou Vertical (H/V): ");
        if (scanf(" %c", &dir) != 1) { 
            printf("Entrada de direção inválida! Tente novamente.\n");
            limparBuffer(); 
            pausar();
            continue;
        }
        limparBuffer(); // CRUCIAL: Limpa o buffer após a leitura da direção

        dir = toupper(dir);
        
        if (dir != 'H' && dir != 'V') {
             printf("Direção invalida! Use 'H' para Horizontal ou 'V' para Vertical.\n");
            pausar();
            continue;
        }

        int reposicionado = 0; // Flag para indicar se o reposicionamento ocorreu
        
        // Reposicionamento Horizontal (Estouro à Direita)
        if (dir == 'H' && col + tamanho > TAM) {
            // Se estoura, ajusta a coluna inicial para que o final do navio
            // termine na última coluna (TAM - 1). Navio se estende para a esquerda.
            int col_nova = TAM - tamanho;
            if (col_nova >= 0) { // Garante que o novo início não seja negativo
                col = col_nova;
                reposicionado = 1;
                printf("\n Reposicionando Horizontalmente: Navio movido para a Esquerda para caber!\n");
            }
        }
        
        // Reposicionamento Vertical (Estouro Abaixo)
        if (dir == 'V' && row + tamanho > TAM) {
            // Se estoura, ajusta a linha inicial para que o final do navio
            // termine na última linha (TAM - 1). Navio se estende para cima.
            int row_nova = TAM - tamanho;
            if (row_nova >= 0) { // Garante que o novo início não seja negativo
                row = row_nova;
                reposicionado = 1;
                printf("\n Reposicionando Verticalmente: Navio movido para Cima para caber!\n");
            }
        }

        // Se o navio for reposicionado e a nova posição for inválida (TAM - tamanho < 0)
        // ou se o tamanho for maior que o tabuleiro (tamanho > TAM, embora isso
        // deva ser tratado antes), um erro mais robusto pode ser necessário.
        // Assumindo que TAM >= tamanho e que o novo início é >= 0.

        // Caso o reposicionamento ainda estoure (o que só aconteceria se TAM < tamanho,
        // o que não deveria ocorrer se a validação inicial do navio estiver correta).
        if ((dir == 'H' && col + tamanho > TAM) || (dir == 'V' && row + tamanho > TAM)) {
            printf("Erro na lógica de reposicionamento ou navio muito grande para o tabuleiro!\n");
            pausar();
            continue;
        }        
        // --- Passo 3: Verificação de Ocupação ---
        // A verificação de ocupação é feita na *nova* posição (row/col)
        if (areaOcupada(j, row, col, tamanho, dir)) {
            printf("Navio encostaria em outro!\n");
            if (reposicionado) {
                 printf("Tente uma coordenada inicial diferente.\n");
            }
            pausar();
            continue;
        }

        // --- Passo 4: Posicionamento Final ---
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