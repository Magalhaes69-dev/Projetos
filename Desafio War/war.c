#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Define a estrutura tipo território
typedef struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
}Territorio;

// Cria vetor das missões
const char* missoes[] = {
    "Conquistar 3 territorios.",
    "Eliminar o exercito Vermelho.",
    "Eliminar o exercito Azul.",
    "Ter um total de 25 tropas.",
    "Ter 15 tropas em um unico territorio."
};
const int TOTAL_MISSOES = 5;

// Designação das funções
void listarTerritorios(const Territorio* mapa, int totalTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanhoMapa, const char* corJogador);
void exibirMissao(const char* corJogador, const char* missao);
void exibirMenu(int turnoJogador);
void menuInicial(Territorio** mapa, int* totalTerritorios, char** missaoAzul, char** missaoVermelho);
void processarCadastro(Territorio* mapa, int* territoriosCadastrados, int totalTerritorios);
void processarAtaque(Territorio* mapa, int totalTerritorios, int* turnoJogador, int* jogoTerminou, const char* missaoAzul, const char* missaoVermelho);
void limparBuffer();
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2);

int main() {
    struct Territorio *mapa = NULL;
    char *missaoJogadorAzul = NULL;
    char *missaoJogadorVermelho = NULL;
    int totalTerritorios = 0;
    int territoriosCadastrados = 0;
    int turnoJogador = 1; // 1 para Azul, 2 para Vermelho
    int jogoTerminou = 0;
    int opcao;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Configura o estado inicial do jogo (mapa e missões)
    menuInicial(&mapa, &totalTerritorios, &missaoJogadorAzul, &missaoJogadorVermelho);

    // laço principal do menu
    do {
        exibirMenu(turnoJogador);
        scanf("%d", &opcao);
        limparBuffer();

        // Processa a opção escolhida
        switch (opcao) {
            case 1:
                processarCadastro(mapa, &territoriosCadastrados, totalTerritorios);
                break;
            
            // Lista os territórios cadastrados
            case 2:
                listarTerritorios(mapa, territoriosCadastrados);
                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;

            // Realiza o ataque
            case 3:
                if (territoriosCadastrados < totalTerritorios) {
                     printf("\nCadastre todos os territorios antes de atacar!\n");

                } else {
                    processarAtaque(mapa, totalTerritorios, &turnoJogador, &jogoTerminou, missaoJogadorAzul, missaoJogadorVermelho);
                }

                printf("\nPressione Enter para continuar...\n");
                getchar();
                break;

            // Sai do programa
            case 0:
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }
        
        if (jogoTerminou) {
            opcao = 0; //força a saída do loop
        }

    } while (opcao != 0);

    // Libera toda a memória alocada
    liberarMemoria(mapa, missaoJogadorAzul, missaoJogadorVermelho);

    return 0;
}

void exibirMenu(int turnoJogador) {
    printf("\n============================================\n");
    printf("TURNO DO JOGADOR: %s\n", (turnoJogador == 1) ? "Azul" : "Vermelho");
    printf("============================================\n");
    printf("       MENU DE OPCOES\n");
    printf("-----------------------------\n");
    printf("1. Cadastrar novo territorio\n");
    printf("2. Listar todos os territorios\n");
    printf("3. Atacar um territorio\n");
    printf("0. Sair\n");
    printf("-----------------------------\n");
    printf("Escolha uma opcao: ");
}

void menuInicial(Territorio** mapa, int* totalTerritorios, char** missaoAzul, char** missaoVermelho) {
    printf("====================================\n");
    printf("    BEM-VINDO AO WAR ESTRUTURADO\n");
    printf("====================================\n");
    printf("Quantos territorios tera o mapa? ");
    scanf("%d", totalTerritorios);
    limparBuffer();

    *mapa = (Territorio*) calloc(*totalTerritorios, sizeof(Territorio));

    if (*mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        exit(1);
    }

    atribuirMissao(missaoAzul, missoes, TOTAL_MISSOES);
    atribuirMissao(missaoVermelho, missoes, TOTAL_MISSOES);

    printf("\n--- MISSOES SECRETAS DISTRIBUIDAS ---\n");
    exibirMissao("Azul", *missaoAzul);
    exibirMissao("Vermelho", *missaoVermelho);
    printf("---------------------------------------\n\n");
    printf("Pressione Enter para comecar...\n");
    getchar();
}

void processarCadastro(Territorio* mapa, int* territoriosCadastrados, int totalTerritorios) {
    if (*territoriosCadastrados < totalTerritorios) {
        printf("\n\n--- Cadastrando Territorio %d ---\n", *territoriosCadastrados + 1);
        
        printf("Nome do territorio: ");
        fgets(mapa[*territoriosCadastrados].nome, 30, stdin);
        mapa[*territoriosCadastrados].nome[strcspn(mapa[*territoriosCadastrados].nome, "\n")] = '\0';

        printf("Cor do exercito (Azul, Vermelho): ");
        fgets(mapa[*territoriosCadastrados].cor, 10, stdin);
        mapa[*territoriosCadastrados].cor[strcspn(mapa[*territoriosCadastrados].cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        scanf("%d", &mapa[*territoriosCadastrados].tropas);
        limparBuffer();
        (*territoriosCadastrados)++;

    } else {
        printf("Mapa completo! Nao e possivel cadastrar mais territorios.\n");
    }

    printf("\nPressione Enter para continuar...\n");
    getchar();
}

void listarTerritorios(const Territorio* mapa, int totalTerritorios) {
    printf("\n\n==================================\n");
    printf("   MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("==================================\n\n");

    if (totalTerritorios == 0) {
        printf("Nenhum territorio cadastrado!\n");

    } else {
        for (int i = 0; i < totalTerritorios; i++) {
            printf("TERRITORIO %d\n", i + 1);
            printf("  - Nome: %s\n", mapa[i].nome);
            printf("  - Dominado por: Exercito %s\n", mapa[i].cor);
            printf("  - Tropas: %d\n\n", mapa[i].tropas);
        }
    }
}

void processarAtaque(Territorio* mapa, int totalTerritorios, int* turnoJogador, int* jogoTerminou, const char* missaoAzul, const char* missaoVermelho) {
    int idxAtacante, idxDefensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    listarTerritorios(mapa, totalTerritorios);

    printf("Escolha o numero do territorio atacante: ");
    scanf("%d", &idxAtacante);
    idxAtacante--;

    printf("Escolha o numero do territorio defensor: ");
    scanf("%d", &idxDefensor);
    idxDefensor--;
    limparBuffer();

    const char* corDoAtacante = (*turnoJogador == 1) ? "Azul" : "Vermelho";

    // Validações
    if (idxAtacante < 0 || idxAtacante >= totalTerritorios || idxDefensor < 0 || idxDefensor >= totalTerritorios) {
        printf("\nNumero de territorio invalido!\n");

    } else if (strcmp(mapa[idxAtacante].cor, corDoAtacante) != 0) {
        printf("\nVoce so pode atacar com um territorio da sua cor (%s)!\n", corDoAtacante);

    } else if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
        printf("\nVoce nao pode atacar um territorio que ja lhe pertence!\n");

    } else if (mapa[idxAtacante].tropas <= 1) {
        printf("\nO territorio atacante precisa ter mais de 1 tropa para atacar!\n");

    } else {
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        printf("\n--- ESTADO DO MAPA APOS O ATAQUE ---\n");
        listarTerritorios(mapa, totalTerritorios);

        // Verifica a missão do jogador atual
        if (*turnoJogador == 1) {
            if (verificarMissao(missaoAzul, mapa, totalTerritorios, "Azul")) {
                printf("\n\n========================================\n");
                printf("VITORIA! O JOGADOR AZUL COMPRIU SUA MISSAO!\n");
                printf("MISSAO: %s\n", missaoAzul);
                printf("========================================\n");
                *jogoTerminou = 1;
            }

        } else {
             if (verificarMissao(missaoVermelho, mapa, totalTerritorios, "Vermelho")) {
                printf("\n\n========================================\n");
                printf("VITORIA! O JOGADOR VERMELHO COMPRIU SUA MISSAO!\n");
                printf("MISSAO: %s\n", missaoVermelho);
                printf("========================================\n");
                *jogoTerminou = 1;
            }
        }
        
        if (!(*jogoTerminou)) {
           *turnoJogador = (*turnoJogador == 1) ? 2 : 1; //passa o turno
        }
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- SIMULACAO DE BATALHA ---\n");
    printf("%s (%s) ataca %s (%s)!\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);    
    printf("Dado do Atacante: %d\n", dadoAtacante);
    printf("Dado do Defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nVITORIA DO ATACANTE!\n");
        printf("%s conquistou %s!\n", atacante->nome, defensor->nome);

        strcpy(defensor->cor, atacante->cor);

        int tropasMovidas = atacante->tropas / 2;

        if (tropasMovidas == 0 && atacante->tropas > 1) {
            tropasMovidas = 1;
        }

        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

    } else {
        printf("\nVITORIA DO DEFENSOR!\n");
        printf("O ataque de %s falhou. O atacante perde 1 tropa.\n", atacante->nome);
        atacante->tropas--;
    }
}

void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;

    const char* missaoEscolhida = missoes[indiceSorteado];
    *destino = (char*) malloc(strlen(missaoEscolhida) + 1);

    if (*destino == NULL) {
        printf("Falha ao alocar memoria para a missao!\n");
        exit(1);
    }

    strcpy(*destino, missaoEscolhida);
}

int verificarMissao(const char* missao, const Territorio* mapa, int tamanhoMapa, const char* corJogador) {
    if (strcmp(missao, "Conquistar 3 territorios.") == 0) {
        int contagem = 0;

        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contagem++;
            }
        }
        return contagem >= 3;
    }

    if (strcmp(missao, "Eliminar o exercito Vermelho.") == 0) {
        if(strcmp(corJogador, "Vermelho") == 0) return 0;

        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
        }
        return 1;
    }

    if (strcmp(missao, "Eliminar o exercito Azul.") == 0) {
        if(strcmp(corJogador, "Azul") == 0) return 0;

        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) return 0;
        }
        return 1;
    }

    if (strcmp(missao, "Ter um total de 25 tropas.") == 0) {
        int totalTropas = 0;

        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        return totalTropas >= 25;
    }

    if (strcmp(missao, "Ter 15 tropas em um unico territorio.") == 0) {
        for (int i = 0; i < tamanhoMapa; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 15) {
                return 1;
            }
        }
        return 0;
    }    
    return 0;
}

void exibirMissao(const char* corJogador, const char* missao) {
    if (missao != NULL) {
        printf("Missao do Jogador %s: %s\n", corJogador, missao);
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2) {
    printf("\nLiberando memoria...\n");
    
    if (mapa != NULL) {
        free(mapa);
        printf("Memoria do mapa liberada.\n");
    }

    if (missaoJogador1 != NULL) {
        free(missaoJogador1);
        printf("Memoria da missao do jogador 1 liberada.\n");
    }

    if (missaoJogador2 != NULL) {
        free(missaoJogador2);
        printf("Memoria da missao do jogador 2 liberada.\n");
    }
}