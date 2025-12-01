# Makefile para o projeto Batalha Naval

# Configurações do Compilador
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Nome do Executável
TARGET = batalha_naval

# Arquivos Fonte
SRC = main.c jogo.c tabuleiro.c utils.c

# Arquivos Objeto
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Regras de Compilação para os arquivos .o
main.o: main.c batalha_naval.h
	$(CC) $(CFLAGS) -c main.c

jogo.o: jogo.c batalha_naval.h
	$(CC) $(CFLAGS) -c jogo.c

tabuleiro.o: tabuleiro.c batalha_naval.h
	$(CC) $(CFLAGS) -c tabuleiro.c

utils.o: utils.c batalha_naval.h
	$(CC) $(CFLAGS) -c utils.c

# Regra para limpeza
clean:
	rm -f $(OBJ) $(TARGET) save.bin