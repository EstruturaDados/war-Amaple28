#ifndef WAR_MESTRE_H
#define WAR_MESTRE_H

#include <stdbool.h>

// Definição da estrutura Territorio
typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
    int indice;
} Territorio;

// Enumeração para os tipos de missão
typedef enum {
    MISSAO_DESTRUIR_VERDE = 1,
    MISSAO_CONQUISTAR_3_TERRITORIOS = 2
} TipoMissao;

// Variáveis globais 
extern const int NUM_TERRITORIOS;

// Funções de Inicialização e Exibição
Territorio* inicializar_territorios();
void exibir_mapa(const Territorio* mapa);

// Funções de Batalha
void simular_ataque(Territorio* atacante, Territorio* defensor);
int rolar_dado();

// Funções de Missão
TipoMissao atribuir_missao_aleatoria(const Territorio* mapa);
void exibir_missao(TipoMissao missao);
bool verificar_vitoria_missao(TipoMissao missao, const Territorio* mapa, const char* cor_original_jogador);

#endif