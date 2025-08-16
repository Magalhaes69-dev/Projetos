#include <stdio.h>
#include <string.h>

// constante global
#define MAX_TERRITORIOS 5

// define a estrutura tipo território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// função para limpar buffer entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    struct Territorio territorios[MAX_TERRITORIOS];
    int totalTerritorios = 0;
    int opcao;

    // laço principal do menu
    do {
        printf("=============================\n");
        printf("       MENU DE OPÇÕES\n");
        printf("=============================\n");
        printf("1. Cadastrar novo território\n");
        printf("2. Listar todos os territórios\n");
        printf("0. Sair\n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: ");

        // lê a opção do usuário
        scanf("%d", &opcao);
        limparBuffer();

        // processa a opção escolhida
        switch (opcao) {
            case 1:
                printf("\n\n---Cadastrando Território %d---\n", totalTerritorios + 1);

            if (totalTerritorios < MAX_TERRITORIOS) {
                printf("Nome do território: ");
                fgets(territorios[totalTerritorios].nome, 30, stdin);

                printf("Cor do exército (Ex. azul, vermelho, verde): ");
                fgets(territorios[totalTerritorios].cor, 10, stdin);

                // remove o caractere de nova linha (\n)
                territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = '\0';
                
                territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = '\0';

                printf("Número de tropas: ");
                scanf("%d", &territorios[totalTerritorios].tropas);
                limparBuffer();

                totalTerritorios++;
                
                if (totalTerritorios == MAX_TERRITORIOS) {
                    printf("\n\nCadastro inicial concluido com sucesso!\n\n");
                }

            } else {
                printf("Campo de batalha cheio, não é possível cadastrar mais territórios!\n");
            }

            // pausa para o usuário ler o registro antes de voltar ao menu
            printf("\nPrecione enter para continuar...\n");
            getchar();
            break;

            // lista os territórios cadastrados
            case 2:
            printf("\n\n==================================\n");
            printf("   MAPA DO MUNDO - ESTADO ATUAL\n");
            printf("==================================\n\n");

            if (totalTerritorios == 0) {
                printf("Nenhum território cadastrado!\n");

            } else {
                for (int i = 0; i < totalTerritorios; i++) {
                    printf("TERRITÓRIO %d\n", i + 1);
                    printf("  - Nome: %s\n", territorios[i].nome);
                    printf("  - Dominado por: Exército %s\n", territorios[i].cor);
                    printf("  - Tropas: %d\n\n", territorios[i].tropas);
                }
            }

            printf("\nPrecione enter para continuar...\n");
            getchar(); 
            break;

            // sai do programa
            case 0:
            printf("Saindo do programa...\n");
            break;

            default:
            printf("\nOpção inválida! Tente novamente\n");
            printf("Precione enter para continuar...\n");
            break;
            
        }
        
    } while (opcao != 0);

    return 0;
}