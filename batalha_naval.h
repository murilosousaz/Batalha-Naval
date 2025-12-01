#ifndef BATALHA_NAVAL_H
#define BATALHA_NAVAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define TAM 10

typedef struct {
    char nome[50];
    char tabuleiro[TAM][TAM];
    int naviosRestantes; // Total de partes de navios não atingidas
} Jogador;

typedef struct {
    Jogador p1, p2;
    int turno;
} SaveGame;

// Protótipos de Funções de Utilitários (utils.c)
void menu();
void limparTela();
void limparBuffer();
void pausar();

// Protótipos de Funções de Tabuleiro (tabuleiro.c)
void inicializarTabuleiro(Jogador *j);
void imprimirCabecalho();
void tabuleiroCompleto(Jogador *j);
void tabuleiroOculto(Jogador *j);
bool areaOcupada(Jogador *player, int l, int c, int tam, char dir);
void posicionarNavio(Jogador *j, int tamanho, const char *nomeNavio);
void posicionarTodosNavios(Jogador *j);

// Protótipos de Funções de Jogo (jogo.c)
void iniciarJogo();
void instrucoes();
void processarTurno(Jogador *atacante, Jogador *defensor, Jogador *p1, Jogador *p2, int *turno);
void salvarJogo(Jogador *p1, Jogador *p2, int turno);
bool carregarJogo(Jogador *p1, Jogador *p2, int *turno);

#endif