#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Define a estrutura tipo território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Designação das funções
void listarTerritorios(const struct Territorio* mapa, int totalTerritorios);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    struct Territorio *mapa = NULL;
    int totalTerritorios = 0;
    int territoriosCadastrados = 0;
    int opcao;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    printf("====================================\n");
    printf("    BEM-VINDO AO WAR ESTRUTURADO\n");
    printf("====================================\n");
    printf("Quantos territórios terá o mapa? ");
    scanf("%d", &totalTerritorios);
    limparBuffer();

    // Aloca memória dinamicamente para o mapa de territórios
    mapa = (struct Territorio*) calloc(totalTerritorios, sizeof(struct Territorio));

    // Verifica se a alocação de memória foi bem-sucedida
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }

    // laço principal do menu
    do {
        printf("=============================\n");
        printf("       MENU DE OPÇÕES\n");
        printf("=============================\n");
        printf("1. Cadastrar novo território\n");
        printf("2. Listar todos os territórios\n");
        printf("3. Atacar um território\n");
        printf("0. Sair\n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: ");

        // lê a opção do usuário
        scanf("%d", &opcao);
        limparBuffer();

        // processa a opção escolhida
        switch (opcao) {
            case 1:
                printf("\n\n---Cadastrando Território %d---\n", territoriosCadastrados + 1);

            if (territoriosCadastrados < totalTerritorios) {
                printf("Nome do território: ");
                fgets(mapa[territoriosCadastrados].nome,30, stdin);

                printf("Cor do exército (Ex. azul, vermelho, verde): ");
                fgets(mapa[territoriosCadastrados].cor, 10, stdin);

                // remove o caractere de nova linha (\n)
                mapa[territoriosCadastrados].nome[strcspn(mapa[territoriosCadastrados].nome, "\n")] = '\0';

                mapa[territoriosCadastrados].cor[strcspn(mapa[territoriosCadastrados].cor, "\n")] = '\0';

                printf("Número de tropas: ");
                scanf("%d", &mapa[territoriosCadastrados].tropas);
                limparBuffer();

                territoriosCadastrados++;
                
                if (territoriosCadastrados == totalTerritorios) {
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
            if (territoriosCadastrados == 0) {
                printf("Nenhum território cadastrado!\n");

                printf("\nPrecione enter para continuar...\n");
                getchar();
                break;
            }
            
            listarTerritorios(mapa, totalTerritorios);
            
            printf("\nPrecione enter para continuar...\n");
            getchar(); 
            break;

            // Realiza o ataque
            case 3:        
                { 
                    int idxAtacante, idxDefensor; //declaração de variáveis locais
                    
                    printf("\n--- FASE DE ATAQUE ---\n");

                    if (territoriosCadastrados == 0) {
                        printf("Nenhum território cadastrado!\n");

                        printf("\nPrecione enter para continuar...\n");
                        getchar();
                        break;
                    }

                    listarTerritorios(mapa, totalTerritorios);
                    
                    printf("Escolha o numero do territorio atacante: ");
                    scanf("%d", &idxAtacante);
                    limparBuffer();
                    
                    printf("Escolha o numero do territorio defensor: ");
                    scanf("%d", &idxDefensor);
                    limparBuffer();

                    // Converte para índice do vetor (usuário digita 1, vira índice 0)
                    idxAtacante--; 
                    idxDefensor--;

                    // Validações
                    if (idxAtacante < 0 || idxAtacante >= totalTerritorios ||
                        idxDefensor < 0 || idxDefensor >= totalTerritorios) {
                        printf("\nNumero de territorio invalido!\n");

                    } else if (idxAtacante == idxDefensor) {
                        printf("\nUm territorio nao pode atacar a si mesmo!\n");

                    } else if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
                        printf("\nVoce nao pode atacar um territorio que ja lhe pertence!\n");

                    } else if (mapa[idxAtacante].tropas <= 1) {
                        printf("\nO territorio atacante precisa ter mais de 1 tropa para atacar!\n");

                    } else {
                        // Se todas as validações passarem, executa o ataque
                        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
                        printf("\n--- ESTADO DO MAPA APOS O ATAQUE ---\n");
                        listarTerritorios(mapa, totalTerritorios);
                    }
                }

                printf("\nPrecione enter para continuar...\n");
                getchar();
                break;

            // sai do programa
            case 0:
            liberarMemoria(mapa);
            printf("\nMemória liberada com sucesso!\n");
            printf("\nSaindo do programa...\n");
            break;

            default:
            printf("\nOpção inválida! Tente novamente\n");
            printf("Precione enter para continuar...\n");
            break;            
        }
        
    } while (opcao != 0);

    return 0;
}

void listarTerritorios(const struct Territorio* mapa, int totalTerritorios) {
    printf("\n\n==================================\n");
    printf("   MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("==================================\n\n");

    if (totalTerritorios == 0) {
        printf("Nenhum território cadastrado!\n");

    } else {
        for (int i = 0; i < totalTerritorios; i++) {
            printf("TERRITÓRIO %d\n", i + 1);
            printf("  - Nome: %s\n", mapa[i].nome);
            printf("  - Dominado por: Exército %s\n", mapa[i].cor);
            printf("  - Tropas: %d\n\n", mapa[i].tropas);
        }
    }
}

void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    
    // Simula a rolagem de um dado de 6 lados para cada um
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- SIMULACAO DE BATALHA ---\n");
    printf("%s (%s) ataca %s (%s)!\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado do Atacante: %d\n", dadoAtacante);
    printf("Dado do Defensor: %d\n", dadoDefensor);

    // Compara os resultados dos dados
    if (dadoAtacante > dadoDefensor) {
        printf("\nVITORIA DO ATACANTE!\n");
        printf("%s conquistou %s!\n", atacante->nome, defensor->nome);
        
        // Muda a cor (dono) do território defensor
        strcpy(defensor->cor, atacante->cor);
        
        // Metade das tropas do atacante se movem
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0 && atacante->tropas > 1) {
            tropasMovidas = 1; //garante que pelo menos uma tropa se mova se possível
        }

        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

    } else {
        printf("\nVITORIA DO DEFENSOR!\n");
        printf("O ataque de %s falhou. O atacante perde 1 tropa.\n", atacante->nome);
        
        // Atacante perde uma tropa se o ataque falhar
        atacante->tropas--;
    }
}

void liberarMemoria(struct Territorio* mapa) {
    printf("\nLiberando memoria do mapa...\n");
    if (mapa != NULL) {
        free(mapa);
    }
}