#include <stdio.h>
#include <string.h>

// Definição da estrutura Territorio
typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

// Número de territórios 
#define NUM_TERRITORIOS 5

// Função para cadastrar os 5 territórios
void cadastrar_territorios(Territorio mapa[]) {
    printf("--- Cadastro Inicial de 5 Territórios ---\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nTerritório %d:\n", i + 1);

        // Nome do Território
        printf("Nome: ");
        // Limpar o buffer antes de usar fgets, caso haja um \n pendente
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        if (fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin) != NULL) {
            // Remover o '\n' lido pelo fgets, se estiver presente
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        }

        // Cor do Exército
        printf("Cor do Exército (ex: Vermelho): ");
        if (fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin) != NULL) {
            // Remover o '\n' lido pelo fgets, se estiver presente
            mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = 0;
        }

        // Número de Tropas
        printf("Número de Tropas: ");
        while (scanf("%d", &mapa[i].numTropas) != 1 || mapa[i].numTropas < 1) {
            printf("Entrada inválida. Digite um número de tropas positivo: ");
            // Limpa o buffer de entrada em caso de erro
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }
}

// Função para exibir o estado atual do mapa
void exibir_mapa(const Territorio mapa[]) {
    printf("\n--- Estado Atual do Mapa ---\n");
    printf("| # | Nome                  | Exército          | Tropas |\n");
    printf("|---|-----------------------|-------------------|--------|\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %d | %-21s | %-17s | %-6d |\n",
               i + 1,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].numTropas);
    }
    printf("----------------------------------------------------------\n");
}

int main() {
    Territorio mapa[NUM_TERRITORIOS];

    cadastrar_territorios(mapa);
    exibir_mapa(mapa);

    return 0;
}