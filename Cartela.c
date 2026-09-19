#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define LINHAS          5
#define COLUNAS         5
#define MAX_CARTELAS    100
#define NUMEROS_POR_COL 15   /* cada coluna tem uma faixa de 15 numeros      */
#define LINHA_LIVRE     2    /* linha 3 (indice 2) -> espaco livre           */
#define ESPACO_LIVRE    0    /* valor que representa o espaco livre          */
#define MAX_TENTATIVAS  1000 /* limite de tentativas para evitar repeticao   */
 
/* ---------------------------------------------------------------------------
 *  Enumeracao das colunas da cartela
 * ------------------------------------------------------------------------ */
typedef enum
{
    COLUNA_B = 0,
    COLUNA_I,
    COLUNA_N,
    COLUNA_G,
    COLUNA_O,
    TOTAL_COLUNAS
} Coluna;
 
/* Strings com os nomes das colunas (indexadas pela enumeracao) */
static const char *NOMES_COLUNAS[TOTAL_COLUNAS] = { "B", "I", "N", "G", "O" };
 
/* ---------------------------------------------------------------------------
 *  Estrutura que representa uma cartela (Desafio 3)
 * ------------------------------------------------------------------------ */
typedef struct
{
    int numeros[LINHAS][COLUNAS];
} Cartela;
 
/* ---------------------------------------------------------------------------
 *  Prototipos das funcoes
 * ------------------------------------------------------------------------ */
int  valorMinimoDaColuna(Coluna coluna);
int  valorMaximoDaColuna(Coluna coluna);
int  gerarNumero(int minimo, int maximo);
int  numeroExiste(int cartela[LINHAS][COLUNAS], int coluna, int numero);
int  ehEspacoLivre(int linha, int coluna);
void ordenarColuna(int cartela[LINHAS][COLUNAS], int coluna);
void gerarCartela(int cartela[LINHAS][COLUNAS]);
int  cartelasSaoIguais(int a[LINHAS][COLUNAS], int b[LINHAS][COLUNAS]);
void gerarCartelaUnica(Cartela cartelas[], int quantidadeJaGerada);
void imprimirCabecalho(void);
void imprimirCartela(int cartela[LINHAS][COLUNAS]);
void imprimirTodasAsCartelas(Cartela cartelas[], int quantidade);
int  lerQuantidadeDeCartelas(void);
 
/* ---------------------------------------------------------------------------
 *  Faixa de valores de cada coluna:
 *      B -> 1..15   I -> 16..30   N -> 31..45   G -> 46..60   O -> 61..75
 * ------------------------------------------------------------------------ */
int valorMinimoDaColuna(Coluna coluna)
{
    return (int) coluna * NUMEROS_POR_COL + 1;
}
 
int valorMaximoDaColuna(Coluna coluna)
{
    return (int) coluna * NUMEROS_POR_COL + NUMEROS_POR_COL;
}
 
/* ---------------------------------------------------------------------------
 *  Sorteia um numero inteiro dentro do intervalo [minimo, maximo]
 * ------------------------------------------------------------------------ */
int gerarNumero(int minimo, int maximo)
{
    return minimo + rand() % (maximo - minimo + 1);
}
 
/* ---------------------------------------------------------------------------
 *  Retorna 1 se o numero ja existe na coluna informada, 0 caso contrario
 * ------------------------------------------------------------------------ */
int numeroExiste(int cartela[LINHAS][COLUNAS], int coluna, int numero)
{
    int linha;
 
    for (linha = 0; linha < LINHAS; linha++)
    {
        if (cartela[linha][coluna] == numero)
        {
            return 1;
        }
    }
 
    return 0;
}
 
/* ---------------------------------------------------------------------------
 *  Indica se a posicao e o espaco livre do centro da cartela
 * ------------------------------------------------------------------------ */
int ehEspacoLivre(int linha, int coluna)
{
    return (linha == LINHA_LIVRE && coluna == (int) COLUNA_N);
}
 
/* ---------------------------------------------------------------------------
 *  Desafio 4: ordena em ordem crescente os numeros de uma coluna.
 *  O espaco livre permanece no centro (ele nao participa da ordenacao).
 *  Utiliza um vetor auxiliar e o algoritmo de ordenacao por selecao.
 * ------------------------------------------------------------------------ */
void ordenarColuna(int cartela[LINHAS][COLUNAS], int coluna)
{
    int auxiliar[LINHAS];
    int quantidade = 0;
    int linha, i, j, menor, troca;
 
    /* copia apenas os numeros sorteados para o vetor auxiliar */
    for (linha = 0; linha < LINHAS; linha++)
    {
        if (!ehEspacoLivre(linha, coluna))
        {
            auxiliar[quantidade] = cartela[linha][coluna];
            quantidade++;
        }
    }
 
    /* ordenacao por selecao */
    for (i = 0; i < quantidade - 1; i++)
    {
        menor = i;
 
        for (j = i + 1; j < quantidade; j++)
        {
            if (auxiliar[j] < auxiliar[menor])
            {
                menor = j;
            }
        }
 
        if (menor != i)
        {
            troca           = auxiliar[i];
            auxiliar[i]     = auxiliar[menor];
            auxiliar[menor] = troca;
        }
    }
 
    /* devolve os numeros ordenados para a cartela */
    quantidade = 0;
 
    for (linha = 0; linha < LINHAS; linha++)
    {
        if (!ehEspacoLivre(linha, coluna))
        {
            cartela[linha][coluna] = auxiliar[quantidade];
            quantidade++;
        }
    }
}
 
/* ---------------------------------------------------------------------------
 *  Preenche uma cartela respeitando a faixa de cada coluna e sem repetir
 *  numeros dentro da mesma coluna
 * ------------------------------------------------------------------------ */
void gerarCartela(int cartela[LINHAS][COLUNAS])
{
    Coluna coluna;
    int    linha, numero, minimo, maximo;
 
    for (coluna = COLUNA_B; coluna < TOTAL_COLUNAS; coluna++)
    {
        minimo = valorMinimoDaColuna(coluna);
        maximo = valorMaximoDaColuna(coluna);
 
        for (linha = 0; linha < LINHAS; linha++)
        {
            if (ehEspacoLivre(linha, coluna))
            {
                cartela[linha][coluna] = ESPACO_LIVRE;
                continue;
            }
 
            /* sorteia ate encontrar um numero ainda nao usado na coluna */
            do
            {
                numero = gerarNumero(minimo, maximo);
            }
            while (numeroExiste(cartela, coluna, numero));
 
            cartela[linha][coluna] = numero;
        }
 
        ordenarColuna(cartela, coluna);
    }
}
 
/* ---------------------------------------------------------------------------
 *  Desafio 5: compara duas cartelas posicao a posicao
 * ------------------------------------------------------------------------ */
int cartelasSaoIguais(int a[LINHAS][COLUNAS], int b[LINHAS][COLUNAS])
{
    int linha, coluna;
 
    for (linha = 0; linha < LINHAS; linha++)
    {
        for (coluna = 0; coluna < COLUNAS; coluna++)
        {
            if (a[linha][coluna] != b[linha][coluna])
            {
                return 0;
            }
        }
    }
 
    return 1;
}
 
/* ---------------------------------------------------------------------------
 *  Gera uma nova cartela garantindo que ela seja diferente das anteriores
 * ------------------------------------------------------------------------ */
void gerarCartelaUnica(Cartela cartelas[], int quantidadeJaGerada)
{
    int tentativa, i, repetida;
 
    for (tentativa = 0; tentativa < MAX_TENTATIVAS; tentativa++)
    {
        gerarCartela(cartelas[quantidadeJaGerada].numeros);
 
        repetida = 0;
 
        for (i = 0; i < quantidadeJaGerada; i++)
        {
            if (cartelasSaoIguais(cartelas[i].numeros,
                                  cartelas[quantidadeJaGerada].numeros))
            {
                repetida = 1;
                break;
            }
        }
 
        if (!repetida)
        {
            return;
        }
    }
 
    printf("Aviso: nao foi possivel gerar uma cartela totalmente unica.\n");
}
 
/* ---------------------------------------------------------------------------
 *  Impressao
 * ------------------------------------------------------------------------ */
void imprimirCabecalho(void)
{
    Coluna coluna;
 
    for (coluna = COLUNA_B; coluna < TOTAL_COLUNAS; coluna++)
    {
        printf("%4s ", NOMES_COLUNAS[coluna]);
    }
 
    printf("\n");
}
 
void imprimirCartela(int cartela[LINHAS][COLUNAS])
{
    int linha, coluna;
 
    imprimirCabecalho();
 
    for (linha = 0; linha < LINHAS; linha++)
    {
        for (coluna = 0; coluna < COLUNAS; coluna++)
        {
            if (cartela[linha][coluna] == ESPACO_LIVRE)
            {
                printf("%4s ", "X");
            }
            else
            {
                printf("%4d ", cartela[linha][coluna]);
            }
        }
 
        printf("\n");
    }
}
 
void imprimirTodasAsCartelas(Cartela cartelas[], int quantidade)
{
    int i;
 
    for (i = 0; i < quantidade; i++)
    {
        printf("\n=============== CARTELA %d ===============\n\n", i + 1);
        imprimirCartela(cartelas[i].numeros);
    }
 
    printf("\n");
}
 
/* ---------------------------------------------------------------------------
 *  Desafio 1: leitura validada da quantidade de cartelas
 * ------------------------------------------------------------------------ */
int lerQuantidadeDeCartelas(void)
{
    int quantidade = 0;
    int lido;
    int caractere;
 
    do
    {
        printf("Quantas cartelas deseja gerar (1 a %d)? ", MAX_CARTELAS);
        lido = scanf("%d", &quantidade);
 
        /* descarta o restante da linha digitada */
        do
        {
            caractere = getchar();
        }
        while (caractere != '\n' && caractere != EOF);
 
        if (lido != 1 || quantidade < 1 || quantidade > MAX_CARTELAS)
        {
            printf("Valor invalido. Tente novamente.\n");
            quantidade = 0;
        }
    }
    while (quantidade == 0);
 
    return quantidade;
}
 
/* ---------------------------------------------------------------------------
 *  Programa principal
 * ------------------------------------------------------------------------ */
int main(void)
{
    Cartela cartelas[MAX_CARTELAS];   /* vetor de cartelas (Desafios 2 e 3) */
    int     quantidade;
    int     i;
 
    srand((unsigned int) time(NULL));
 
    printf("========================================\n");
    printf("      GERADOR DE CARTELAS DE BINGO      \n");
    printf("========================================\n\n");
 
    quantidade = lerQuantidadeDeCartelas();
 
    for (i = 0; i < quantidade; i++)
    {
        gerarCartelaUnica(cartelas, i);
    }
 
    imprimirTodasAsCartelas(cartelas, quantidade);
 
    printf("%d cartela(s) gerada(s) com sucesso!\n", quantidade);
 
    return 0;
}
