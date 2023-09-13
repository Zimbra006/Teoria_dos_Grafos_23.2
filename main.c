#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N = 0;
int M = 0;

void main()
{
    FILE *ptr;
    char str[30];

    // Opening file in reading mode
    ptr = fopen("text.txt", "r");

    if (NULL == ptr)
    {
        printf("Arquivo não pode ser aberto\n");
    }

    fgets(str, 30, ptr);

    // Transforma o caractere do cabeçalho em um inteiro
    N = atoi(str);

    // Inicializa um array de grau para todos os vértices
    int grau[N];

    for (int i = 0; i < N; i++)
    {
        grau[i] = 0;
    }

    // Conta as arestas e atualiza o valor dos graus
    while (fgets(str, 30, ptr) != NULL)
    {
        // Atualiza os graus dos vértices em que a aresta incide
        int valor1 = str[0] - '0';
        int valor2 = str[2] - '0';

        grau[valor1 - 1]++;
        grau[valor2 - 1]++;

        M++;
    }

    // Fecha o arquivo de texto
    fclose(ptr);

    // Encontra os valores mínimo e máximo
    // Além do valor médio

    int max = grau[0];
    int min = grau[0];
    float mid = grau[0] / (float)N;

    for (int i = 1; i < N; i++)
    {
        int val = grau[i];
        mid += val / (float)N;

        if (val > max)
            max = val;
        if (val < min)
            min = val;
    }

    // Calcula a mediana
    int mean = 0;

    for (int i = 0; i < N - 1; i++)
    {
        for (int j = i + 1; j < N; j++)
        {

            if (grau[i] > grau[j])
            {

                int aux = grau[i];
                grau[i] = grau[j];
                grau[j] = aux;
            }
        }
    }

    if (N % 2)
    {
        mean = grau[N / 2];
    }
    else
    {
        mean = (grau[N / 2 - 1] + grau[N / 2]) / 2;
    }

    printf("Número de vértices: %d\n", N);
    printf("Número de arestas: %d\n", M);
    printf("Grau mínimo: %d\n", min);
    printf("Grau máximo: %d\n", max);
    printf("Grau médio: %f\n", mid);
    printf("Mediana do grau: %d\n", mean);
}