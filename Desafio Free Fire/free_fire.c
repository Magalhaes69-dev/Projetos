#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define o tamanho máximo das constantes
#define MAX_ITENS 10 //quantidade máxima de itens
#define MAX_NOME 30 //tamanho máximo da strint nome do item
#define MAX_TIPO 20 //tamanho máximo da string tipo de item

// Estrutura que define os itens da mochila
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// Funções recebem o array e um ponteiro para o contador de quantidade
void inserirItem(Item mochila[], int *quantidade);
void removerItem(Item mochila[], int *quantidade);
void listarItens(const Item mochila[], int quantidade);
void buscarItem(const Item mochila[], int quantidade);

int main(){
    Item mochila[MAX_ITENS]; //array mochila para receber os itens
    int quantidadeDeItens = 0; //contador de itens na mochila
    int opcao;

    do{
        printf("\n==================================================\n");
        printf("   MOCHILA DE SOBREVIVÊNCIA - CÓDIGO DA ILHA   \n");
        printf("==================================================\n");
        printf("Itens na mochila: %d/%d\n", quantidadeDeItens, MAX_ITENS);
        printf("\n1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens da Mochila\n");
        printf("4. Buscar Item na Mochila\n");
        printf("0. Sair\n");
        printf("--------------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidadeDeItens);
                break;

            case 2:
                removerItem(mochila, &quantidadeDeItens);
                break;

            case 3:
                listarItens(mochila, quantidadeDeItens);
                break;

            case 4:
                buscarItem(mochila, quantidadeDeItens);
                break;

            case 0:
                printf("Saindo do sistema de inventário...\n");
                break;

            default:
                printf("Opção inválida, tente novamente!\n");
                break;
        }

    }while (opcao != 0);

    return 0;
}

// Implementa as funções
void inserirItem(Item mochila[], int *quantidade) {
    if (*quantidade == MAX_ITENS) {
        printf("\nERRO: Mochila cheia, impossível adicionar mais itens!\n");
        return;
    }

    Item novoItem;
    printf("\n--- Adicionar Novo Item ---\n");
    printf("Nome do item: ");
    fgets(novoItem.nome, MAX_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; //remove o caractere de nova linha (\n) que a função fgets adiciona ao final da string quando o usuário digita um texto e pressiona "Enter"

    printf("Tipo do item (arma, municao, cura, etc.): ");
    fgets(novoItem.tipo, MAX_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Adiciona o item na posição livre do array e a *quantidade obtem o valor
    mochila[*quantidade] = novoItem;
    
    // Incrementa a contagem
    (*quantidade)++;

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

void removerItem(Item mochila[], int *quantidade) {
    if (*quantidade == 0) {
        printf("\nA mochila esta vazia. Nao há itens para remover.\n");
        return;
    }
    
    char nomeParaRemover[MAX_NOME]; //armazena o nome do item a ser removido
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0;

    int indice = -1; //inicializa como não encontrado
    for (int i = 0; i < *quantidade; i++) {
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\nERRO: Item '%s' não foi encontrado na mochila.\n", nomeParaRemover);
        return;
    }

    // Move os itens para preencher o espaço vago deixado pelo item removido
    for (int i = indice; i < *quantidade - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    
    // Decrementa a contagem.
    (*quantidade)--;

    printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
}

void listarItens(const Item mochila[], int quantidade) {
    printf("\n-------------------------------------------------------------------");
    printf("\n    ITENS NA MOCHILA (%d/%d)    ", quantidade, MAX_ITENS);
    printf("\n-------------------------------------------------------------------\n");

    if (quantidade == 0) {
        printf("A mochila esta vazia.\n");

    } else {
        printf("%-30s | %-20s | %10s\n", "NOME", "TIPO", "QUANTIDADE"); //cria a tabela
        printf("-------------------------------------------------------------------\n");

        for (int i = 0; i < quantidade; i++) {
            printf("%-30s | %-20s | %10d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("-------------------------------------------------------------------\n");
}

void buscarItem(const Item mochila[], int quantidade) {
     if (quantidade == 0) {
        printf("\nA mochila esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    char nomeParaBuscar[MAX_NOME];
    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item a ser buscado: ");
    fgets(nomeParaBuscar, MAX_NOME, stdin);
    nomeParaBuscar[strcspn(nomeParaBuscar, "\n")] = 0;

    int indice = -1;

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mochila[i].nome, nomeParaBuscar) == 0) { //se True, encontrou o item
            indice = i;
            break;
        }
    }
    
    if (indice != -1) {
        printf("\n--- Item Encontrado ---\n");
        printf("Nome: %s\n", mochila[indice].nome);
        printf("Tipo: %s\n", mochila[indice].tipo);
        printf("Quantidade: %d\n", mochila[indice].quantidade);
        printf("-----------------------\n");

    } else {
        printf("\nERRO: Item '%s' nao foi encontrado na mochila.\n", nomeParaBuscar);
    }
}