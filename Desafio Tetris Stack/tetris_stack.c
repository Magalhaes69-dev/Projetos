#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define a capacidade máxima da fila de peças.
#define MAX 5

// Estrutura que representa uma peça do jogo.
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da Fila de peças.
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// Inicializa os ponteiros e o contador da fila.
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia.
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia.
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere uma peça no final da fila (enqueue).
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\nA fila de peças está cheia! Nao é possivel inserir.\n");
    }
    
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX; // lógica para fila circular.
    f->total++;
    printf("\nNova peça '%c' (ID: %d) inserida na fila.\n", p.nome, p.id);
}

// Remove uma peça do início da fila (dequeue).
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("\nA fila de peças está vazia! Não há peça para jogar.\n");
        return;
    }

    // Pega a peça do início da fila para informar qual foi jogada.
    Peca pecaJogada = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    
    printf("\nA Peça '%c' (ID: %d) foi jogada.\n", pecaJogada.nome, pecaJogada.id);
}

// Mostra o conteúdo atual da fila.
void mostrarFila(Fila *f) {
    printf("\n     --- Situação Atual ---\n");
    printf("Fila de pecas: ");

    if (filaVazia(f)) {
        printf("Vazia\n");
        return;

    } else {

        // Loop que percorre a fila de forma circular para exibição.
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n-------------------------------------------\n");
}

// Gera uma nova peça com um tipo aleatório e um ID sequencial.
Peca gerarPeca() {
    
    static int proximoId = 0; // mantém o valor de 'proximoId' nas chamadas da função.
    Peca p;
    char tipos[] = {'I', 'C', 'T', 'L'};
    
    p.nome = tipos[rand() % 4]; // escolhe um dos 4 tipos de peça aleatoriamente.
    p.id = proximoId;
    
    proximoId++; // incrementa para que o próximo ID seja único.
    
    return p;
}

// Mostra o menu de opções para o jogador.
void mostrarMenu() {
    printf("\nOpções de ação:\n");
    printf("1 - Jogar peça.\n");
    printf("2 - Inserir nova peça.\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

int main() {

    // Inicializa o gerador de peças aleatórias.
    srand(time(NULL));

    Fila filaDePecas;
    inicializarFila(&filaDePecas);

    int opcao;

    // Preenche a fila com o número inicial de peças.
    printf("Iniciando o jogo e preenchendo a fila inicial de pecas...\n");
    for(int i = 0; i < MAX; i++) {
        Peca novaPeca = gerarPeca();
        inserir(&filaDePecas, novaPeca);
    }

    mostrarFila(&filaDePecas);

    // Loop principal do menu do jogo.
    do {
        mostrarMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&filaDePecas);
                mostrarFila(&filaDePecas);
                break;

            case 2:
                if (!filaCheia(&filaDePecas)) {
                    Peca novaPeca = gerarPeca();
                    inserir(&filaDePecas, novaPeca);
                    mostrarFila(&filaDePecas);

                } else {
                     printf("\nA fila está cheia! Jogue uma peça antes.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o programa. Até mais!\n");
                break;

            default:
                printf("\nOpção invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}