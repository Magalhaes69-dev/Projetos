#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define as capacidades máximas da fila e da pilha.
#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa uma peça do jogo.
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da Fila de peças.
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha de peças.
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Inicializa os ponteiros e o contador da fila.
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia.
int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

// Verifica se a fila está vazia.
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere uma peça no final da fila (enqueue).
void inserirNaFila(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\nA fila de peças está cheia! Não é possivel inserir.\n");
        return;
    }
    
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA; // lógica para fila circular
    f->total++;
}

// Remove uma peça do início da fila (dequeue).
Peca removerDaFila(Fila *f) {
    Peca pecaRemovida; // peça que será retornada

    if (filaVazia(f)) {
        printf("\nA fila de peças está vazia! Não há peça para jogar.\n");

        // Retorna uma peça vazia em caso de erro.
        pecaRemovida.id = -1; 
        pecaRemovida.nome = ' ';
        return pecaRemovida;
    }

    // Pega a peça do início da fila para informar qual foi jogada.
    pecaRemovida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    
    return pecaRemovida;
}

// Inicializa o topo da pilha.
void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica que a pilha está vazia.
}

// Verifica se a pilha está cheia.
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia.
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adiciona uma peça ao topo da pilha (push).
void empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("\nA pilha de reserva está cheia!\n");
        return;
    }

    p->topo++;
    p->itens[p->topo] = peca;
}

// Remove e retorna a peça do topo da pilha (pop).
Peca desempilhar(Pilha *p) {
    Peca pecaRemovida;

    if (pilhaVazia(p)) {
        printf("\nA pilha de reserva está vazia!\n");

        // Retorna uma peça vazia em caso de erro.
        pecaRemovida.id = -1;
        pecaRemovida.nome = ' ';
        return pecaRemovida;
    }

    pecaRemovida = p->itens[p->topo];
    p->topo--;
    return pecaRemovida;
}

// Gera uma nova peça com um tipo aleatório e um ID sequencial.
Peca gerarPeca() {
    
    static int proximoId = 0; // mantém o valor de "proximoId" nas chamadas da função
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    p.nome = tipos[rand() % 4]; // escolhe um dos 4 tipos de peça aleatoriamente
    p.id = proximoId;
    
    proximoId++; // incrementa para que o próximo ID seja único
    
    return p;
}

// Joga uma peça da fila e gera uma nova no lugar.
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("\nA fila de peças está vazia! Não há peça para jogar.\n");
        return;
    }

    Peca pecaJogada = removerDaFila(f);
    printf("\nA peça '%c' (ID: %d) foi jogada.\n", pecaJogada.nome, pecaJogada.id);

    // Gera e insere uma nova peça para manter a fila cheia.
    Peca novaPeca = gerarPeca();
    inserirNaFila(f, novaPeca);
    printf("Nova peça '%c' (ID: %d) foi gerada e inserida na fila.\n", novaPeca.nome, novaPeca.id);
}

// Move a peça da fila para a pilha de reserva.
void reservarPeca(Fila *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("\nA pilha de reserva está cheia! Jogue uma peça reservada antes.\n");
        return;
    }

    if (filaVazia(f)) {
        printf("\nA fila está vazia! Não há peças para reservar.\n");
        return;
    }

    Peca pecaReservada = removerDaFila(f);
    empilhar(p, pecaReservada);
    printf("\nA peça '%c' (ID: %d) foi movida para a reserva.\n", pecaReservada.nome, pecaReservada.id);

    // Gera e insere uma nova peça para manter a fila cheia.
    Peca novaPeca = gerarPeca();
    inserirNaFila(f, novaPeca);
    printf("Nova peça '%c' (ID: %d) foi gerada e inserida na fila.\n", novaPeca.nome, novaPeca.id);
}

// Joga uma peça da pilha de reserva.
void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nA pilha de reserva está vazia! Nenhuma peça para jogar.\n");
        return;
    }

    Peca pecaUsada = desempilhar(p);
    printf("\nFoi usada a peça reservada '%c' (ID: %d).\n", pecaUsada.nome, pecaUsada.id);
}

// Mostra o estado atual da fila e da pilha.
void mostrarEstadoAtual(Fila *f, Pilha *p) {
    printf("\n--- ESTADO ATUAL DO JOGO ---\n");

    // Mostra Fila.
    printf("Fila de pecas: ");
    if (filaVazia(f)) {
        printf("Vazia");

    } else {
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }

    printf("\n");

    // Mostra Pilha.
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("Vazia");

    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }

    printf("\n----------------------------\n");
}

// Mostra o menu de opções para o jogador.
void mostrarMenu() {
    printf("\nOpções de ação:\n");
    printf("1 - Jogar peça.\n");
    printf("2 - Reservar peça.\n");
    printf("3 - Usar peça reservada.\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

int main() {

    // Inicializa o gerador de peças aleatórias.
    srand(time(NULL));

    Fila filaDePecas;
    inicializarFila(&filaDePecas);

    Pilha pilhaDeReserva;
    inicializarPilha(&pilhaDeReserva);

    int opcao;

    // Preenche a fila com o número inicial de peças.
    printf("Iniciando o jogo e preenchendo a fila inicial de pecas...\n");
    for(int i = 0; i < MAX_FILA; i++) {
        Peca novaPeca = gerarPeca();
        inserirNaFila(&filaDePecas, novaPeca);
    }

    mostrarEstadoAtual(&filaDePecas, &pilhaDeReserva);

    // Loop principal do menu do jogo.
    do {
        mostrarMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&filaDePecas);
                break;

            case 2:
                reservarPeca(&filaDePecas, &pilhaDeReserva);
                break;
            
            case 3:
                usarPecaReservada(&pilhaDeReserva);
                break;

            case 0:
                printf("\nEncerrando o programa. Até mais!\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

        // Mostra o estado do jogo após cada ação (exceto ao sair).
        if (opcao != 0) {
            mostrarEstadoAtual(&filaDePecas, &pilhaDeReserva);
        }

    } while (opcao != 0);

    return 0;
}