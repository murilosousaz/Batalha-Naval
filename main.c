#include "batalha_naval.h"

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
    limparBuffer(); // Limpa o buffer após a leitura do número

    switch (op)
    {
    case 1:
      iniciarJogo();
      break;

    case 2: {
        Jogador p1, p2;
        int turno;

        if (carregarJogo(&p1, &p2, &turno)) {
            printf("Jogo carregado com sucesso! Vez de: %s\n", (turno % 2) ? p1.nome : p2.nome);
            pausar();

            // Loop principal do jogo carregado
            while (p1.naviosRestantes > 0 && p2.naviosRestantes > 0) {
                if (turno % 2)
                    processarTurno(&p1, &p2, &p1, &p2, &turno);
                else
                    processarTurno(&p2, &p1, &p1, &p2, &turno);

                turno++;
            }

            limparTela();
            printf("\n===== FIM DE JOGO =====\n");
            printf("VENCEDOR: %s\n", (p1.naviosRestantes > 0) ? p1.nome : p2.nome);
            pausar();

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