#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "war_mestre.h" // Inclui o cabeçalho com as funções e structs

// --- Função Principal ---

int main() {
    // Inicializa o gerador de números aleatórios e o mapa
    srand(time(NULL));
    Territorio* mapa = inicializar_territorios();
    
    // A cor do exército do jogador é "Azul" (baseado na inicialização)
    const char* cor_do_jogador = "Azul"; 
    TipoMissao missao_atual = atribuir_missao_aleatoria(mapa);
    bool jogo_ativo = true;
    int opcao;

    printf("\n*** BEM-VINDO ao WAR Estruturado - NÍVEL MESTRE! ***\n");
    exibir_mapa(mapa);
    exibir_missao(missao_atual);

    while (jogo_ativo) {
        printf("\n--- Menu Principal ---\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missão\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Por favor, digite um número.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        int c; while ((c = getchar()) != '\n' && c != EOF) {} // Limpar buffer

        switch (opcao) {
            case 1: {
                int atacante_idx, defensor_idx;

                printf("\n>>> FASE DE ATAQUE <<<\n");
                printf("Seu exército: %s\n", cor_do_jogador);
                exibir_mapa(mapa);

                // Lógica de entrada de ataque
                printf("Escolha o território ATACANTE (1 a %d): ", NUM_TERRITORIOS);
                if (scanf("%d", &atacante_idx) != 1 || atacante_idx < 1 || atacante_idx > NUM_TERRITORIOS) {
                    printf("Seleção de atacante inválida.\n");
                    break;
                }
                
                printf("Escolha o território DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
                if (scanf("%d", &defensor_idx) != 1 || defensor_idx < 1 || defensor_idx > NUM_TERRITORIOS) {
                    printf("Seleção de defensor inválida.\n");
                    break;
                }
                
                // Validação de regras de ataque
                if (atacante_idx == defensor_idx) {
                    printf("Erro: Um território não pode atacar a si mesmo!\n");
                } else if (strcmp(mapa[atacante_idx - 1].corExercito, cor_do_jogador) != 0) {
                    printf("Erro: O território atacante não pertence ao seu exército (%s)!\n", cor_do_jogador);
                } else if (strcmp(mapa[defensor_idx - 1].corExercito, cor_do_jogador) == 0) {
                    printf("Erro: Não é possível atacar um território do seu próprio exército!\n");
                } else {
                    simular_ataque(&mapa[atacante_idx - 1], &mapa[defensor_idx - 1]);
                    // Após o ataque, verifica a missão imediatamente
                    if (verificar_vitoria_missao(missao_atual, mapa, cor_do_jogador)) {
                        printf("\n*** VITÓRIA! ***\n");
                        printf("Parabéns! Você cumpriu sua missão e CONQUISTOU o WAR!\n");
                        jogo_ativo = false;
                    }
                }
                break;
            }
            case 2: {
                // Verificar Missão
                exibir_mapa(mapa);
                exibir_missao(missao_atual);
                
                if (verificar_vitoria_missao(missao_atual, mapa, cor_do_jogador)) {
                    printf("\n*** VITÓRIA! ***\n");
                    printf("Parabéns! Você cumpriu sua missão e CONQUISTOU o WAR!\n");
                    jogo_ativo = false;
                } else {
                    printf("\nMissão ainda não cumprida. Continue atacando!\n");
                }
                break;
            }
            case 0:
                // Sair
                jogo_ativo = false;
                printf("Jogo encerrado. Até a próxima!\n");
                break;
            default:
                printf("Opção inválida. Escolha 1, 2 ou 0.\n");
                break;
        }
    }

    // Libera a memória alocada
    free(mapa);
    return 0;
}