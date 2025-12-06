#include "batalha_naval.h"

void salvarJogo(Jogador *p1, Jogador *p2, int turno){
    SaveGame save = {*p1, *p2, turno};
    size_t written_items;

    FILE *arq = fopen("save.bin", "wb");
    if (!arq) {
        printf("Erro FATAL ao salvar: Nao foi possivel abrir ou criar 'save.bin'.\n");
        printf("Verifique se o programa tem permissoes de escrita no diretorio atual.\n");
        return;
    }

    written_items = fwrite(&save, sizeof(SaveGame), 1, arq);
    fclose(arq);
    
    if (written_items != 1) {
        printf("Aviso: Houve uma falha parcial ao escrever os dados no 'save.bin'. Dados incompletos.\n");
    }
}

bool carregarJogo(Jogador *p1, Jogador *p2, int *turno) {
    FILE *arq = fopen("save.bin", "rb");
    if (!arq) {
        return false;
    }

    SaveGame save;
    if (fread(&save, sizeof(SaveGame), 1, arq) != 1) {
        printf("Erro ao carregar jogo: O arquivo 'save.bin' esta corrompido ou incompleto.\n");
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
    int col = 0; 
    int row = 0; 

    while (1) {
        limparTela();
        printf("\nVEZ DE: %s\n", atacante->nome);
        tabuleiroOculto(defensor);

        printf("\nCoordenada do tiro (ex: A 5) ou 'save' para salvar: ");

        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            continue;
        }
        input_line[strcspn(input_line, "\n")] = 0;

        if (sscanf(input_line, "%s", entrada_temp) == 1 && strcmp(entrada_temp, "save") == 0) {
            printf("\nSalvando jogo...\n");
            salvarJogo(p1, p2, *turno);
            printf("Jogo salvo com sucesso!\n");
            pausar();
            exit(0);
        }

        if (sscanf(input_line, " %c %d", &letra, &linha) != 2) {
            printf("Entrada invalida! Use o formato 'A 5' ou digite 'save'.\n");
            pausar();
            continue;
        }

        letra = toupper(letra);
        col = letra - 'A';
        row = linha - 1;

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

  printf("##################################################\n");
  printf("##  BEM-VINDO(A) À BATALHA NAVAL!  ##\n");
  printf("##################################################\n\n");

  printf("### Objetivo do Jogo ###\n");
  printf("Seu objetivo é afundar todos os navios do seu oponente.\n");
  printf("O primeiro jogador a reduzir a zero as partes de navios restantes do adversário vence a partida.\n\n");

  printf("### O Tabuleiro ###\n");
  printf("* As coordenadas são referenciadas por uma **Letra** (Coluna A-J) e um **Número** (Linha 1-10).\n");
  printf("* Você terá um tabuleiro para posicionar seus navios e um tabuleiro 'de tiro' para registrar seus disparos no oponente.\n\n");

  printf("### Simbolos no Tabuleiro ###\n");
  printf("Durante o jogo, você verá os seguintes símbolos:\n");
  printf("* **~** (Vazio): Água, local onde você ainda não atirou ou não há navio.\n");
  printf("* **O** (Navio): Parte de um navio (apenas no seu tabuleiro).\n");
  printf("* **X** (Atingido): Um tiro que acertou uma parte de um navio.\n");
  printf("* **.** (Água/Erro): Um tiro que caiu na água (não acertou um navio).\n\n");

  printf("### Como Jogar ###\n");
  printf("1.  **Posicionamento:** No início, você posicionará seus navios (Porta-Aviões (5), Navio-Tanque (4), Submarino (3) e Bote (2)) no seu tabuleiro. Você deve inserir a coordenada (Ex: **A 5**) e depois a direção (**H** ou **V**).\n");
  printf("2.  **O Ataque:** Em sua vez, você digitará a coordenada para onde deseja atirar (exemplo: **C 7**). Para salvar o jogo, digite **save**.\n");
  printf("3.  **Resultado:**\n");
  printf("    * Se acertar um navio: A posição será marcada com **X**.\n");
  printf("    * Se errar: A posição será marcada com **.**.\n");
  printf("4.  **Regras de Posicionamento:**\n");
  printf("    * Dois navios não podem ocupar a mesma posição.\n");
  printf("    * **Dois navios não podem ser adjacentes**, ou seja, não será permitido posicionar navios lado a lado ou diagonalmente.\n");
  printf("    * Um navio não pode ser posicionado fora do tabuleiro.\n\n");
  printf("5.  **Vezes:** Os jogadores se revezam, atacando um de cada vez.\n\n");

  printf("### Vencedor\n");
  printf("O jogo termina quando um jogador consegue reduzir a zero as partes de navios restantes do adversário.\n\n");

  printf("##################################################\n");
  printf("##  BOA SORTE, MARUJO!  ##\n");
  printf("##################################################\n");

  printf("##################################################\n");
  printf("##  Aperte Enter para voltar ao Menu  ##\n");
  printf("##################################################\n");

  pausar();
}