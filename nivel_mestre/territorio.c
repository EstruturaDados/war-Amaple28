#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "war_mestre.h" // Inclui o cabeçalho

// Constante para o número de territórios
const int NUM_TERRITORIOS = 5;

// --- Implementação das Funções Auxiliares ---

// Função para simular a rolagem de um dado de 6 faces
int rolar_dado() {
    return (rand() % 6) + 1;
}

// Inicialização automática
Territorio* inicializar_territorios() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Dados automáticos
    const char* nomes[] = {"Brasil", "China", "EUA", "Canada", "Egito"};
    const char* cores[] = {"Azul", "Vermelho", "Azul", "Verde", "Vermelho"};
    int tropas[] = {3, 4, 3, 2, 4}; // Tropas iniciais

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        mapa[i].indice = i + 1;
        strncpy(mapa[i].nome, nomes[i], sizeof(mapa[i].nome) - 1);
        strncpy(mapa[i].corExercito, cores[i], sizeof(mapa[i].corExercito) - 1);
        mapa[i].numTropas = tropas[i];
    }
    printf("Territórios inicializados automaticamente.\n");
    return mapa;
}

// Função para exibir o estado atual do mapa
void exibir_mapa(const Territorio* mapa) {
    printf("\n--- Estado Atual do Mapa ---\n");
    printf("| # | Nome                  | Exército          | Tropas |\n");
    printf("|---|-----------------------|-------------------|--------|\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %d | %-21s | %-17s | %-6d |\n",
               mapa[i].indice,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].numTropas);
    }
    printf("----------------------------------------------------------\n");
}

// Função principal de simulação de ataque 
void simular_ataque(Territorio* atacante, Territorio* defensor) {
    if (atacante->numTropas <= 1) {
        printf("\n--- ATAQUE CANCELADO ---\n");
        printf("%s precisa de pelo menos 2 tropas (1 p/ ataque, 1 p/ permanência).\n", atacante->nome);
        return;
    }

    if (defensor->numTropas == 0) {
        printf("\n--- ATAQUE CANCELADO ---\n");
        printf("%s já está conquistado (0 tropas).\n", defensor->nome);
        return;
    }

    printf("\n--- BATALHA: %s (%s) vs %s (%s) ---\n",
           atacante->nome, atacante->corExercito, defensor->nome, defensor->corExercito);

    int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();

    printf("  Dados Atacante: %d | Dados Defensor: %d\n", dado_ataque, dado_defesa);

    if (dado_ataque >= dado_defesa) { // Empates favorecem o atacante
        printf("  >>> O atacante %s venceu e eliminou 1 tropa do defensor.\n", atacante->nome);
        defensor->numTropas--;

        if (defensor->numTropas == 0) {
            printf("\n!!! CONQUISTA !!!\n");
            printf("O território %s foi CONQUISTADO por %s.\n", defensor->nome, atacante->nome);
            // Troca a posse e move uma tropa
            strcpy(defensor->corExercito, atacante->corExercito);
            atacante->numTropas--;
            defensor->numTropas = 1;
        }
    } else {
        printf("  >>> O defensor %s venceu e eliminou 1 tropa do atacante.\n", defensor->nome);
        atacante->numTropas--;
    }
    printf("  %s tem %d tropas | %s tem %d tropas\n", atacante->nome, atacante->numTropas, defensor->nome, defensor->numTropas);
    printf("\n--- BATALHA CONCLUÍDA ---\n");
}

// --- Funções de Missão ---

// Atribui uma missão aleatória
TipoMissao atribuir_missao_aleatoria(const Territorio* mapa) {
    // Garantir que a cor inicial do jogador (Azul) seja usada para verificar a destruição
    // A cor é hardcoded aqui para simplificar o exemplo, mas em um jogo real seria dinâmica.
    // O objetivo é apenas ter uma missão aleatória
    return (rand() % 2 == 0) ? MISSAO_DESTRUIR_VERDE : MISSAO_CONQUISTAR_3_TERRITORIOS;
}

// Exibe a missão atual
void exibir_missao(TipoMissao missao) {
    printf("\n--- Sua Missão ---\n");
    switch (missao) {
        case MISSAO_DESTRUIR_VERDE:
            printf("Objetivo: Eliminar completamente o exército de cor VERDE do mapa.\n");
            break;
        case MISSAO_CONQUISTAR_3_TERRITORIOS:
            printf("Objetivo: Conquistar um total de 3 territórios.\n");
            break;
        default:
            printf("Nenhuma missão atribuída.\n");
            break;
    }
    printf("------------------\n");
}

// Verifica se a missão foi cumprida
bool verificar_vitoria_missao(TipoMissao missao, const Territorio* mapa, const char* cor_original_jogador) {
    int territorios_jogador = 0;
    bool verde_destruido = true;

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].corExercito, cor_original_jogador) == 0) {
            territorios_jogador++;
        }
        if (strcmp(mapa[i].corExercito, "Verde") == 0) {
            verde_destruido = false; // Há pelo menos um território Verde restante
        }
    }

    switch (missao) {
        case MISSAO_DESTRUIR_VERDE:
            return verde_destruido;
        case MISSAO_CONQUISTAR_3_TERRITORIOS:
            return territorios_jogador >= 3;
        default:
            return false;
    }
}