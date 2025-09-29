#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura Territorio
typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
    int indice; // Para facilitar a identificação
} Territorio;

// Constante para o número inicial de territórios
#define NUM_INICIAL_TERRITORIOS 5

// --- Funções Auxiliares ---

// Função para cadastrar os territórios 
Territorio* cadastrar_territorios_dinamico(int num_territorios) {
    Territorio* mapa = (Territorio*)calloc(num_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        perror("Erro ao alocar memória para o mapa");
        exit(EXIT_FAILURE);
    }

    printf("--- Cadastro Inicial de %d Territórios ---\n", num_territorios);
    for (int i = 0; i < num_territorios; i++) {
        mapa[i].indice = i + 1;

        printf("\nTerritório %d:\n", i + 1);

        // Nome
        printf("Nome: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin) != NULL) {
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        }

        // Cor
        printf("Cor do Exército (ex: Azul): ");
        if (fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin) != NULL) {
            mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;
        }

        // Tropas
        printf("Número de Tropas: ");
        while (scanf("%d", &mapa[i].numTropas) != 1 || mapa[i].numTropas < 1) {
            printf("Entrada inválida. Digite um número de tropas positivo: ");
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }
    return mapa;
}

// Função para exibir o estado atual do mapa
void exibir_mapa(const Territorio* mapa, int num_territorios) {
    printf("\n--- Estado Atual do Mapa (%d Territórios) ---\n", num_territorios);
    printf("| # | Nome                  | Exército          | Tropas |\n");
    printf("|---|-----------------------|-------------------|--------|\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("| %d | %-21s | %-17s | %-6d |\n",
               mapa[i].indice,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].numTropas);
    }
    printf("----------------------------------------------------------\n");
}

// Simula a rolagem de um dado de 6 faces
int rolar_dado() {
    return (rand() % 6) + 1;
}

// Função principal de simulação de ataque
void simular_ataque(Territorio* atacante, Territorio* defensor) {
    if (atacante->numTropas <= 1) {
        printf("\n--- ATAQUE CANCELADO ---\n");
        printf("%s não pode atacar. Precisa de pelo menos 2 tropas (1 para o ataque, 1 para permanecer).\n", atacante->nome);
        return;
    }

    if (defensor->numTropas == 0) {
        printf("\n--- ATAQUE CANCELADO ---\n");
        printf("%s já está conquistado (0 tropas).\n", defensor->nome);
        return;
    }

    printf("\n--- BATALHA: %s (Atacante) vs %s (Defensor) ---\n", atacante->nome, defensor->nome);

    // Regras simplificadas: 1 dado para cada
    int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();

    printf("Dados Sorteados:\n");
    printf("  Atacante (%s): %d\n", atacante->nome, dado_ataque);
    printf("  Defensor (%s): %d\n", defensor->nome, dado_defesa);

    // Lógica da Batalha: Empates favorecem o atacante
    if (dado_ataque >= dado_defesa) {
        printf("\nResultado: O atacante %s venceu o confronto!\n", atacante->nome);
        defensor->numTropas--;
        printf("  %s perdeu 1 tropa. Tropas restantes: %d\n", defensor->nome, defensor->numTropas);

        if (defensor->numTropas == 0) {
            printf("\n!!! CONQUISTA !!!\n");
            printf("O território %s foi CONQUISTADO por %s.\n", defensor->nome, atacante->nome);
            // Troca a posse do território
            strcpy(defensor->corExercito, atacante->corExercito);
            // Move 1 tropa do atacante para o novo território
            atacante->numTropas--;
            defensor->numTropas = 1;
        }
    } else {
        printf("\nResultado: O defensor %s venceu o confronto!\n", defensor->nome);
        atacante->numTropas--;
        printf("  %s perdeu 1 tropa. Tropas restantes: %d\n", atacante->nome, atacante->numTropas);
    }
    printf("\n--- BATALHA CONCLUÍDA ---\n");
}

// --- Função Principal ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    int num_territorios = NUM_INICIAL_TERRITORIOS;
    Territorio* mapa = cadastrar_territorios_dinamico(num_territorios);
    int atacante_idx, defensor_idx;
    char continuar = 's';

    // Laço interativo de batalhas
    while (continuar == 's' || continuar == 'S') {
        exibir_mapa(mapa, num_territorios);

        printf("\n--- Fase de Ataque ---\n");

        // Entrada do atacante
        printf("Escolha o território ATACANTE (1 a %d): ", num_territorios);
        if (scanf("%d", &atacante_idx) != 1 || atacante_idx < 1 || atacante_idx > num_territorios) {
            printf("Seleção inválida. Tente novamente.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        // Entrada do defensor
        printf("Escolha o território DEFENSOR (1 a %d): ", num_territorios);
        if (scanf("%d", &defensor_idx) != 1 || defensor_idx < 1 || defensor_idx > num_territorios) {
            printf("Seleção inválida. Tente novamente.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        // Validação adicional
        if (atacante_idx == defensor_idx) {
            printf("Um território não pode atacar a si mesmo!\n");
        } else if (strcmp(mapa[atacante_idx - 1].corExercito, mapa[defensor_idx - 1].corExercito) == 0) {
            printf("Não é possível atacar um território do seu próprio exército!\n");
        } else {
            // Executa a batalha
            simular_ataque(&mapa[atacante_idx - 1], &mapa[defensor_idx - 1]);
        }

        printf("\nDeseja continuar atacando? (s/n): ");
        // Limpa o buffer antes de ler o char
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        scanf(" %c", &continuar);
    }

    exibir_mapa(mapa, num_territorios);

    // Libera a memória alocada
    free(mapa);

    return 0;
}