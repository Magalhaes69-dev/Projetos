#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define o tamanho máximo das constantes
#define MAX_ITENS 20 //quantidade máxima de itens
#define MAX_NOME 30 //tamanho máximo da strint nome do item
#define MAX_TIPO 20 //tamanho máximo da string tipo de item

// Estrutura que define os itens da mochila
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade;
} Item;

// Funções de gerenciamento do inventário
void inserirItem(Item mochila[], int *quantidade);
void removerItem(Item mochila[], int *quantidade);
void listarItens(const Item mochila[], int quantidade);

// Funções de ordenação com contagem de comparações
void bubbleSortNome(Item mochila[], int quantidade, long long int *comparacoes);
void insertionSortTipo(Item mochila[], int quantidade, long long int *comparacoes);
void selectionSortPrioridade(Item mochila[], int quantidade, long long int *comparacoes);

// Função de busca com contagem de comparações
int buscaBinariaPorNome(const Item mochila[], int quantidade, const char nomeBusca[], long long int *comparacoes);

int main() {
    Item mochila[MAX_ITENS]; //array mochila para receber os itens
    int quantidadeItens = 0; //contador de itens na mochila
    int opcao;

    /* Variável do estado da ordenação do vetor de itens.
       - 0: não ordenado, 1: ordenado por nome, 2: ordenado por tipo, 3: ordenado por
    prioridade.*/
    int ordenadoPor = 0;

    do {
        printf("\n=============================================================\n");
        printf("   MOCHILA DE SOBREVIVÊNCIA - CÓDIGO DA ILHA (NIVEL MESTRE)   \n");
        printf("=============================================================\n");
        printf("Itens na mochila: %d/%d\n", quantidadeItens, MAX_ITENS);
        
        printf("Status da Ordenacao: ");
        switch (ordenadoPor) {
            case 1: printf("Por nome (Bubble Sort)\n"); break;
            case 2: printf("Por tipo (Insertion Sort)\n"); break;
            case 3: printf("Por prioridade (Selection Sort)\n"); break;
            default: printf("Nao ordenado\n"); break;
        }

        printf("\n1. Adicionar Item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens (inventário)\n");
        printf("4. Organizar itens (ordenar)\n");
        printf("5. Buscar item-chave (busca binária por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA - Sair\n");
        printf("--------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidadeItens);                
                ordenadoPor = 0; // ao adicionar um novo item, a lista se desordena.
                break;

            case 2:
                removerItem(mochila, &quantidadeItens);
                ordenadoPor = 0; // ao remover um item, a lista se desordena.
                break;

            case 3:
                listarItens(mochila, quantidadeItens);
                break;

            // Implementa a ordenação
            case 4: {
                if (quantidadeItens == 0) {
                    printf("\nNao há itens para ordenar!\n");
                    printf("\nPressione enter para retornar...");
                    getchar();
                    break;

                }else{
                    
                    // Declara variáveis locais
                    int opcaoOrdenacao;
                    long long int comparacoes = 0;
                    clock_t inicio, fim;
                    double tempo_gasto;

                    printf("\n--- Estrategia de Organizacao ---\n");
                    printf("Como deseja ordenar os itens?\n");
                    printf("1. Por nome (Bubble Sort)\n");
                    printf("2. Por tipo (Insertion Sort)\n");
                    printf("3. Por prioridade (Selection Sort)\n");
                    printf("0. Cancelar");
                    printf("Escolha o critério: ");
                    scanf("%d", &opcaoOrdenacao);

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);

                    Item tempMochila[MAX_ITENS];
                    memcpy(tempMochila, mochila, sizeof(Item) * quantidadeItens); // copia os dados para um array temporário

                    inicio = clock(); // marca o início da contagem de tempo da ordenação

                    switch (opcaoOrdenacao) {
                        case 1:
                            bubbleSortNome(tempMochila, quantidadeItens, &comparacoes);
                            ordenadoPor = 1;
                            printf("\nItens organizados por NOME.\n");
                            break;

                        case 2:
                            insertionSortTipo(tempMochila, quantidadeItens, &comparacoes);
                            ordenadoPor = 2;
                                printf("\nItens organizados por TIPO.\n");
                            break;

                        case 3:
                            selectionSortPrioridade(tempMochila, quantidadeItens, &comparacoes);
                            ordenadoPor = 3;
                            printf("\nItens organizados por PRIORIDADE.\n");
                            break;

                        case 0:
                            printf("\nOrdenação cancelada!\n");
                            break;

                        default:
                            printf("Opcao de ordenacao invalida!\n");
                            printf("\nPressione enter para retornar...");
                            getchar();
                            continue;
                        
                        }
                        
                    fim = clock(); // marca o fim da contagem de tempo da ordenação
                    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                    // Exibe as métricas de desempenho.
                    printf("\n--- Analise de Desempenho ---\n");
                    printf("Numero de comparacoes realizadas: %lld\n", comparacoes);
                    printf("Tempo de execucao: %f segundos\n", tempo_gasto);
                    
                    // Atualiza o array original e exibe o resultado
                    memcpy(mochila, tempMochila, sizeof(Item) * quantidadeItens);
                    listarItens(mochila, quantidadeItens);
                }   
            }                
                break;            

            // Implementa a busca binária
            case 5: { 
                if (quantidadeItens == 0) {
                    printf("\nA mochila esta vazia. Nao há itens para buscar.\n");
                    printf("\nPressione enter para retornar...");
                    getchar();
                    break;
                }
                
                else if (ordenadoPor != 1) {
                    printf("\nALERTA: A busca binaria requer que os itens estejam ordenados por NOME.\n");
                    printf("Use a opcao 3 para organizar a lista primeiro.\n");
                    printf("\nPressione enter para retornar...");
                    getchar();
                    break;

                } else {
                    char nomeBusca[MAX_NOME];
                    long long int comparacoesBusca = 0;

                    printf("\n--- Busca por Item-Chave ---\n");
                    printf("Nome do item a buscar: ");
                    fgets(nomeBusca, MAX_NOME, stdin);
                    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

                    int indice = buscaBinariaPorNome(mochila, quantidadeItens, nomeBusca, &comparacoesBusca);

                    if (indice != -1) { // o item foi encontrado
                        printf("\n--- ITEM-CHAVE ENCONTRADO! ---\n");
                        printf("Nome: %s\n", mochila[indice].nome);
                        printf("Tipo: %s\n", mochila[indice].tipo);
                        printf("Prioridade: %d\n", mochila[indice].prioridade);
                        printf("-------------------------------------\n");

                    } else {
                        printf("\nERRO: item-chave '%s' nao encontrado.\n", nomeBusca);
                        printf("\nPressione enter para retornar...");
                        getchar();                        
                    }

                    printf("Numero de comparacoes na busca: %lld\n", comparacoesBusca);
                    printf("\nPressione enter para continuar...");
                    getchar();
                }                
                break;
            }

            case 0:
                printf("\nAbandonando a ilha!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                printf("\nPressione enter para retornar...");
                getchar();
                break;
        }

    } while (opcao != 0);

    return 0;
}

// Implementa as funções
void inserirItem(Item mochila[], int *quantidade) {
    if (*quantidade >= MAX_ITENS) {
        printf("\nERRO: Inventário de itens cheio!\n");
        printf("\nPressione enter para retornar...");
        getchar();
        return;
    }

    Item novoItem;
    printf("\n--- Adicionar Novo Item ---\n");
    printf("Nome do item: ");
    fgets(novoItem.nome, MAX_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // remove o caractere de nova linha (\n) que a função fgets adiciona ao final da string quando o usuário digita um texto e pressiona "Enter"

    printf("Tipo (controle, suporte, propulsao, etc.): ");
    fgets(novoItem.tipo, MAX_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    printf("Prioridade (1 a 10): ");
    scanf("%d", &novoItem.prioridade);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Adiciona o item na posição livre do array e a *quantidade obtem o valor
    mochila[*quantidade] = novoItem;
    (*quantidade)++; // incrementa a contagem

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
    printf("\nPressione enter para continuar...");
    getchar();
}

void removerItem(Item mochila[], int *quantidade) {
    if (*quantidade == 0) {
        printf("\nA mochila esta vazia. Nao há itens para remover!\n");
        printf("\nPressione enter para retornar...");
        getchar();
        return;
    }
    
    char nomeParaRemover[MAX_NOME]; // armazena o nome do item a ser removido
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0;

    int indice = -1; // flag => inicializa o íncide como ainda não encontrado
    for (int i = 0; i < *quantidade; i++) {
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nERRO: Item '%s' não foi encontrado na mochila!\n", nomeParaRemover);
        printf("\nPressione enter para retornar...");
        getchar();
        return;
    }

    // Move os itens para preencher o espaço vago deixado pelo item removido
    for (int i = indice; i < *quantidade - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    
    // Decrementa a contagem.
    (*quantidade)--;

    printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
    printf("\nPressione enter para continuar...");
    getchar();

}

void listarItens(const Item mochila[], int quantidade) {
    printf("\n--------------------------------------------------------------------------\n");
    printf("                       INVENTÁRIO ATUAL (%d/%d)                       \n", quantidade, MAX_ITENS);
    printf("--------------------------------------------------------------------------\n");

    if (quantidade == 0) {
        printf("Nenhum item coletado ainda!\n");

    } else {
        printf("%-20s | %-20s | %12s | %12s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
        printf("--------------------------------------------------------------------------\n");
        for (int i = 0; i < quantidade; i++) {
            printf("%-20s | %-20s | %12d | %12d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
        }
    }    
    printf("--------------------------------------------------------------------------\n");
    printf("\nPressione enter para continuar...");
    getchar();
}

void bubbleSortNome(Item mochila[], int quantidade, long long int *comparacoes) {
    if (quantidade == 0) {
        printf("não há íntens para ordenar!");
        printf("\nPressione enter para continuar...");
        getchar();
        return;
    }

    Item temp; // serve para armazenamento temporárrio durante a ordenação
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            (*comparacoes)++; // incrementa a cada comparação de strings

            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp; // insere o item na posição correta
            }
        }
    }
}

void insertionSortTipo(Item mochila[], int quantidade, long long int *comparacoes) {
    Item chave; // serve para armazenamento temporário durante a ordenação
    for (int i = 1; i < quantidade; i++) {
        chave = mochila[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++; // Incrementa antes de comparar.
            if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                mochila[j + 1] = mochila[j];
                j = j - 1;

            } else {
                break; // sai do while se o elemento já está na posição correta
            }
        }
        mochila[j + 1] = chave; // insere o item na posição correta
    }
}

void selectionSortPrioridade(Item mochila[], int quantidade, long long int *comparacoes) {
    Item temp; // serve para armazenamento temporário durante a ordenação
    for (int i = 0; i < quantidade - 1; i++) {
        int max_idx = i;

        for (int j = i + 1; j < quantidade; j++) {
            (*comparacoes)++; // incrementa a cada comparação de prioridade
            if (mochila[j].prioridade > mochila[max_idx].prioridade) {
                max_idx = j;
            }
        }

        if(max_idx != i){
            temp = mochila[max_idx];
            mochila[max_idx] = mochila[i];
            mochila[i] = temp; // insere o item na posição correta
        }
    }
}

int buscaBinariaPorNome(const Item mochila[], int quantidade, const char nomeBusca[], long long int *comparacoes) {
    int inicio = 0;
    int fim = quantidade - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++; // Conta cada comparação feita.
        int resultado = strcmp(nomeBusca, mochila[meio].nome);

        // Verifica se o nomeBusca está no meio
        if (resultado == 0)
            return meio;

        // Se nomeBusca é maior, ignora a metade esquerda
        if (resultado > 0)
            inicio = meio + 1;

        // Se nomeBusca é menor, ignora a metade direita
        else
            fim = meio - 1;
    }
    
    return -1; // retorna -1 se o item não for encontrado
}
