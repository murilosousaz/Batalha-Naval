#include "batalha_naval.h"

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
    if (fread(&save, sizeof(SaveGame), 1, arq) != 1) {
        fclose(arq);
        return false;
    }
    fclose(arq);

    *p1 = save.p1;
    *p2 = save.p2;
    *turno = save.turno;

    return true;
}

void processarTurno(Jogador *atacante, Jogador *defensor, Jogador *p1, Jogador *p2, int *turno){
    char input_line[10];
    char entrada_temp[10];
    char letra = '\0';
    int linha = 0;

    while (1) {
        limparTela();
        printf("\nVEZ DE: %s\n", atacante->nome);
        tabuleiroOculto(defensor);

        printf("\nCoordenada do tiro (ex: A 5) ou 'save' para salvar: ");

        // 1. Usa fgets para ler a linha inteira, garantindo que o buffer seja limpo.
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            continue;
        }
        input_line[strcspn(input_line, "\n")] = 0;

        // 2. Tenta ler se o comando é "save"
        if (sscanf(input_line, "%s", entrada_temp) == 1 && strcmp(entrada_temp, "save") == 0) {
            printf("\nSalvando jogo...\n");
            salvarJogo(p1, p2, *turno);
            printf("Jogo salvo com sucesso!\n");
            pausar();
            exit(0);
        }

        // 3. Tenta ler a coordenada 'Letra Número'
        if (sscanf(input_line, " %c %d", &letra, &linha) != 2) {
            printf("Entrada inválida! Use o formato 'A 5' ou digite 'save'.\n");
            pausar();
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


void iniciarJogo(){
    limparTela();
    Jogador p1, p2;

    // A limpeza do buffer no main.c após a seleção do menu deve ser suficiente.
    // Não é necessário limparBuffer() aqui, pois o fgets abaixo gerencia a entrada.

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

    // Total de partes de navios: 5 + 4 + 3 + 2 = 14
    p1.naviosRestantes = 14;
    p2.naviosRestantes = 14;

    int turno = 1;

    while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) {
        if (turno % 2)
            processarTurno(&p1, &p2, &p1, &p2, &turno);
        else
            processarTurno(&p2, &p1, &p1, &p2, &turno);

        salvarJogo(&p1, &p2, turno);

        turno++;
    }

    limparTela();
    printf("\n===== FIM DE JOGO =====\n");
    printf("VENCEDOR: %s\n", (p1.naviosRestantes > 0) ? p1.nome : p2.nome);
    pausar();
}

void instrucoes(){
  limparTela();
  // ... (conteúdo da instrucoes, sem alteração funcional)
  printf("##################################################\n");
  printf("##  BEM-VINDO(A) À BATALHA NAVAL!  ##\n");
  // ...
  printf("##################################################\n");
  printf("##  Aperte Enter para voltar ao Menu  ##\n");
  printf("##################################################\n");

  pausar();
}