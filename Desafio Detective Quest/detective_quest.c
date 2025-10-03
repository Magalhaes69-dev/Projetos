#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- ESTRUTURAS DE DADOS DO JOGO ---

// Cria a estrutura para os nós da árvore de pistas (BST).
typedef struct PistaNo
{
    char pista[150];
    struct PistaNo *esquerda;
    struct PistaNo *direita;
} PistaNo;

// Cria a estrutura dos cômodos da mansão.
typedef struct Sala
{
    char nome[50];
    char *pista; // ponteiro para permitir cômodos sem pistas (NULL)
    int pistaJaColetada; // impede que o jogador colete a mesma pista repetidamente
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

#define TAMANHO_TABELA 10

// Cria estrutura para tratar colisões.
typedef struct HashNo
{
    char pista[150];
    char suspeito[50];
    struct HashNo *proximo;
} HashNo;

// A tabela hash é um vetor de ponteiros para o início da lista.
HashNo *tabelaSuspeitos[TAMANHO_TABELA];

// --- FUNÇÕES DA ÁRVORE DE PISTAS (BST) ---

// Cria um novo nó para a árvore de pistas.
PistaNo *criarPistaNo(const char *textoPista)
{
    PistaNo *novo = (PistaNo *)malloc(sizeof(PistaNo));
    if (novo == NULL)
    {
        printf("Falha ao alocar memória para nova pista!\n");
        exit(1);
    }

    // Nós recém criados ainda não tem pistas
    strcpy(novo->pista, textoPista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere uma nova pista na árvore de pistas (BST) de forma ordenada.
PistaNo *inserirPista(PistaNo *raiz, const char *textoPista)
{
    // Se a árvore está vazia, cria o primeiro nó aqui.
    if (raiz == NULL)
    {
        return criarPistaNo(textoPista);
    }

    // Compara a nova pista com a do nó atual para decidir o caminho.
    if (strcmp(textoPista, raiz->pista) < 0)
    {
        raiz->esquerda = inserirPista(raiz->esquerda, textoPista);
    }

    else if (strcmp(textoPista, raiz->pista) > 0)
    {
        raiz->direita = inserirPista(raiz->direita, textoPista);
    }

    // Retorna a raiz (se a pista já existir, nada muda).
    return raiz;
}

// Imprime a árvore de pistas em ordem alfabética (em ordem).
void exibirPistas(PistaNo *raiz)
{
    if (raiz != NULL)
    {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Libera toda a memória alocada para a árvore de pistas.
void liberarPistas(PistaNo *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// --- FUNÇÕES DA TABELA HASH ---

// Calcula o índice hash para uma pista.
int funcao_hash(const char *pista)
{
    int soma = 0;

    for (int i = 0; pista[i] != '\0'; i++)
    {
        soma += pista[i];
    }

    return soma % TAMANHO_TABELA;
}

// Inicializa a tabela hash, definindo todos os ponteiros como NULL.
void inicializarTabelaHash()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        tabelaSuspeitos[i] = NULL;
    }
}

// Insere uma associação de pista e suspeito na tabela hash.
void inserirNaHash(const char *pista, const char *suspeito)
{
    int indice = funcao_hash(pista);
    HashNo *novo = (HashNo *)malloc(sizeof(HashNo));

    if (!novo)
    {
        printf("Erro de alocação na tabela hash!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaSuspeitos[indice];
    tabelaSuspeitos[indice] = novo;
}

// Consulta o suspeito correspondente a uma pista na tabela hash.
const char *encontrarSuspeito(const char *pista)
{
    int indice = funcao_hash(pista);
    HashNo *atual = tabelaSuspeitos[indice];

    while (atual != NULL)
    {
        if (strcmp(atual->pista, pista) == 0)
        {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }

    return NULL;
}

// Libera toda a memória alocada para a tabela hash.
void liberarTabelaHash()
{
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        HashNo *atual = tabelaSuspeitos[i];

        while (atual != NULL)
        {
            HashNo *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}

// Função para preencher a tabela com as pistas e suspeitos do jogo.
void alimentarTabelaHash()
{
    inserirNaHash("Marcas de pegadas de lama perto da lareira.", "Mordomo");
    inserirNaHash("Um livro de suspense fora do lugar, aberto na página 47.", "Governanta");
    inserirNaHash("Cheiro forte de café, mas nenhuma cafeteira ligada.", "Cozinheiro");
    inserirNaHash("Um convite rasgado no chão com a data de ontem.", "Dama de Vermelho");
    inserirNaHash("Uma rosa branca recém-colhida, deixada sobre um banco.", "Jardineiro");
}

// --- FUNÇÕES DO MAPA DA MANSÃO (ÁRVORE BINÁRIA) ---

// Cria os cômodos da mansão
Sala *criarSala(const char *nome, const char *pista)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        printf("Falha ao alocar memória para uma nova sala!\n");
        exit(1);
    }

    // Cômodos recém criados ainda não tem filhos
    strcpy(novaSala->nome, nome);
    novaSala->pistaJaColetada = 0;
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    // Se uma pista foi fornecida, aloca memória e a copia.
    if (pista != NULL)
    {
        novaSala->pista = (char *)malloc(strlen(pista) + 1);
        if (novaSala->pista != NULL)
        {
            strcpy(novaSala->pista, pista);
        }
    }

    else
    {
        novaSala->pista = NULL;
    }

    return novaSala;
}

// Controla a navegação entre salas, a coleta de pistas e o loop principal do jogo.
void explorarSalasComPistas(Sala *salaInicial, PistaNo **raizPistas)
{
    Sala *atual = salaInicial;
    char escolha;

    while (1) // enquanto estiver jogando
    {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se há uma pista no cômodo e se ela ainda não foi coletada.
        if (atual->pista != NULL && !atual->pistaJaColetada)
        {
            printf("A pista %s foi encontrada.\n", atual->pista);

            *raizPistas = inserirPista(*raizPistas, atual->pista);
            atual->pistaJaColetada = 1; // marca a pista como coletada

            printf("Pista adicionada ao seu diário.\n");
        }

        printf("\n--- PARA ONDE IR? ---\n");
        if (atual->esquerda)
        {
            printf(" (E)squerda: %s\n", atual->esquerda->nome);
        }

        if (atual->direita)
        {
            printf(" (D)ireita: %s\n", atual->direita->nome);
        }

        printf(" (S)air e ver relatório de pistas\n");

        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        escolha = tolower(escolha); // torna a escolha minuscula

        switch (escolha)
        {
        case 'e':
            if (atual->esquerda)
            {
                atual = atual->esquerda;
            }

            else
            {
                printf("\nNão há caminho por aí.\n");
            }
            break;

        case 'd':
            if (atual->direita)
            {
                atual = atual->direita;
            }

            else
            {
                printf("\nNão há caminho por aí.\n");
            }
            break;

        case 's':
            printf("\nInvestigação encerrada!.\n");
            return; // sai da função e do loop

        default:
            printf("\nOpção inválida, detetive. Mantenha o foco!\n");
            break;
        }
    }
}

// Libera toda a memória alocada para a mansão.
void liberarMapa(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        if (raiz->pista != NULL)
        {
            free(raiz->pista); // libera a string da pista primeiro...
        }

        free(raiz); // ...depois libera a struct da sala
    }
}

// --- FUNÇÕES DE JULGAMENTO FINAL ---

// Percorre a BST de pistas e conta as que apontam para o suspeito.
int contarPistasParaSuspeito(PistaNo *raiz, const char *suspeitoAcusado, int *contador)
{
    if (raiz != NULL)
        return;

    const char *suspeitoEncontrado = encontrarSuspeito(raiz->pista);

    if (suspeitoEncontrado != NULL && strcmp(suspeitoEncontrado, suspeitoAcusado) == 0)
    {
        (*contador)++;
    }

    contarPistasParaSuspeito(raiz->esquerda, suspeitoAcusado, contador);
    contarPistasParaSuspeito(raiz->direita, suspeitoAcusado, contador);
}

// Conduz à fase de julgamento final do jogo.
void verificarSuspeitoFinal(PistaNo *pistasColetadas)
{
    char acusacao[50];
    int contagemDePistas = 0;

    printf("\n--- HORA DO JULGAMENTO ---\n");
    printf("Com base nas pistas coletadas, quem é o culpado?\n");
    printf("Suspeitos possíveis: Mordomo, Governanta, Cozinheiro, Dama de Vermelho e Jardineiro\n");

    printf("Sua acusação: ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpa o buffer de entrada

    fgets(acusacao, sizeof(acusacao), stdin);
    acusacao[strcspn(acusacao, "\n")] = '\0'; // remove a quebra de linha

    contarPistasParaSuspeito(pistasColetadas, acusacao, &contagemDePistas);

    printf("\n--- VEREDITO ---\n");
    printf("Você acusou %s com base em %d pista(s) relevante(s).\n", acusacao, contagemDePistas);

    if (contagemDePistas >= 2)
    {
        printf("Evidências suficientes! Você desvendou o mistério e prendeu o culpado. Parabéns, detetive!\n");
    }

    else
    {
        printf("A acusação é fraca. O verdadeiro culpado escapou por falta de provas. O caso foi arquivado.\n");
    }
}

// --- FUNÇÃO PRINCIPAL ---

int main()
{
    // Inicializa e alimenta a tabela hash com as associações de pista/suspeito
    inicializarTabelaHash();
    alimentarTabelaHash();

    // Inicializa a árvore de pistas coletadas (começa vazia).
    PistaNo *pistasColetadas = NULL;

    // Monta o mapa da mansão, associando pistas aos cômodos.
    Sala *hallDeEntrada = criarSala("Hall de Entrada", "Um convite rasgado no chão com a data de ontem.");

    hallDeEntrada->esquerda = criarSala("Sala de Estar", "Marcas de pegadas de lama perto da lareira.");

    hallDeEntrada->direita = criarSala("Jardim de Inverno", "Uma rosa branca recém-colhida, deixada sobre um banco.");

    hallDeEntrada->esquerda->esquerda = criarSala("Biblioteca", "Um livro de suspense fora do lugar, aberto na página 47.");

    hallDeEntrada->esquerda->direita = criarSala("Cozinha", "Cheiro forte de café, mas nenhuma cafeteira ligada.");

    // Inicia o jogo.
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore os cômodos para desvendar seus segredos.\n");

    explorarSalasComPistas(hallDeEntrada, &pistasColetadas);

    // Exibe o relatório final e julgamento.
    printf("\n--- RELATÓRIO DE PISTAS COLETADAS ---\n");
    if (pistasColetadas == NULL)
    {
        printf("Nenhuma pista foi coletada.\n");
    }

    else
    {
        exibirPistas(pistasColetadas);
    }

    if (pistasColetadas != NULL)
    {
        verificarSuspeitoFinal(pistasColetadas);
    }

    else
    {
        printf("\nVocê encerrou a investigação sem pistas suficientes para fazer uma acusação.\n");
    }

    // Libera toda a memória alocada.
    liberarMapa(hallDeEntrada);
    liberarPistas(pistasColetadas);
    liberarTabelaHash();

    printf("\nJogo encerrado!\n");

    return 0;
}