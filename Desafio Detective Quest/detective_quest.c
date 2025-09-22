#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura dos cômodos da mansão.
typedef struct Sala
{
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria os cômodos da mansão
Sala *criarSala(const char *nome)
{
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL)
    {
        printf("Falha ao alocar memória para uma nova sala!\n");
        exit(1);
    }

    // Cômodos recém criados ainda não tem filhos
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Controla o loop principal do jogo, registrando e exibindo o caminho percorrido.
void explorarSalas(Sala *salaInicial)
{
    Sala *atual = salaInicial;
    char escolha;

    // Novo array para guardar histórico de vizitas e contador
    char historico[20][50]; // suporta até 20 salas com até 50 caracteres cada
    int salasVisitadas = 0;

    if (atual != NULL)
    {
        // Registra a primeira sala no histórico
        strcpy(historico[salasVisitadas], atual->nome);
        salasVisitadas++;
    }

    while (1) // enquanto estiver jogando
    {
        printf("\n----------------------------------------\n");
        printf("Você está em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL)
        {
            printf("Este cômodo não tem saídas. Fim da exploração!\n");
            break;
        }

        printf("\n--- ENCONTRE AS PISTAS ---\n");

        if (atual->esquerda)
        {
            printf(" (E)squerda: %s\n", atual->esquerda->nome);
        }

        if (atual->direita)
        {
            printf(" (D)ireita: %s\n", atual->direita->nome);
        }

        printf(" (S)air da investigação\n");
        printf("Sua escolha: ");

        scanf(" %c", &escolha);

        int c;
        while ((c = getchar()) != '\n' && c != EOF); // limpa o buffer

        escolha = tolower(escolha); // torna a escolha minuscula

        int moveu = 0; // flag para saber se o jogador mudou de sala

        switch (escolha)
        {
        case 'e':
            if (atual->esquerda)
            {
                atual = atual->esquerda;
                moveu = 1; // marca que o jogador se moveu
            }

            else
            {
                printf("\nNão há cômodo por aí.\n");
            }
            break;

        case 'd':
            if (atual->direita)
            {
                atual = atual->direita;
                moveu = 1;
            }

            else
            {
                printf("\nNão há cômodo por aí.\n");
            }
            break;

        case 's':
            printf("\nInvestigação encerrada!\n");
            goto fim_exploracao; // pula para o final da função para exibir o relatório de cômodos vizitados

        default:
            printf("\nOpção inválida, detetive. Mantenha o foco!\n");
            break;
        }

        // Se o jogador se moveu, registra a nova sala no histórico.
        if (moveu)
        {
            strcpy(historico[salasVisitadas], atual->nome);
            salasVisitadas++;
        }
    }

    fim_exploracao: // termina o loop naturalmente

    // Exibe o relatório com os cômodos vizitados
    printf("\n--- Cômodos vizitados ---\n");
    for (int i = 0; i < salasVisitadas; i++)
    {
        printf("%s", historico[i]);

        if (i < salasVisitadas - 1)
        {
            printf(" -> "); // adiciona a seta "->" entre os nomes
        }
    }

    printf("\n");
}

// Libera toda a memória alocada para a mansão.
void liberarMapa(Sala *raiz)
{
    if (raiz != NULL)
    {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

// Função principal: monta a árvore e inicia a exploração.
int main()
{
    Sala *hallDeEntrada = criarSala("Hall de Entrada");
    hallDeEntrada->esquerda = criarSala("Sala de Estar");
    hallDeEntrada->direita = criarSala("Jardim");
    hallDeEntrada->esquerda->esquerda = criarSala("Dispensa");
    hallDeEntrada->esquerda->direita = criarSala("Quarto");

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Você está na entrada da mansão. Explore os cômodos para desvendar seus segredos.\n");

    explorarSalas(hallDeEntrada);

    liberarMapa(hallDeEntrada);

    printf("\nJogo encerrado!\n");

    return 0;
}