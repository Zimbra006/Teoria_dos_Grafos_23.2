#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Valores usados para referenciar o tipo de representação
const int MATRIZ = 0; // Matriz de adjacência
const int VETOR = 1; // Vetor de adjacência

// Declaração das funções usadas
void carregarValores(string, vector<vector<int>> &, int *, int *, vector<int> &, int);
void salvarValores(vector<int> &, int, int);

int main()
{

   // Declaração das variáveis onde serão postos os valores carregados
   int N, M;
   vector<int> grau;
   vector<vector<int>> grafo;

   carregarValores("text.txt", grafo, &N, &M, grau, MATRIZ);

}

void carregarValores(string path, vector<vector<int>> &grafo, int *N, int *M, vector<int> &grau, int repr)
{

   string str;
   ifstream file(path);

   // Extraí o cabeçalho contendo o número de vértices
   getline(file, str);
   *N = stoi(str);

   // Atualiza o vetor grau para conter N elementos
   for (int i = 0; i < *N; i++)
      grau.push_back(i);

   // Atualiza o vetor grafo com base na sua representação
   for (int i = 0; i < *N; i++)
      if (repr == MATRIZ)
      {
         // Caso seja de matriz, adiciona um vetor inteiro só de 0's
         grafo.push_back(vector<int>(*N, 0));
      }
      else
      {
         // Caso seja de vetor, adiciona um vetor vazio
         grafo.push_back(vector<int>(0));
      }

   while (getline(file, str))
   {
      // Aumenta a contagem de arestas
      *M += 1;

      // Atualiza os graus dos vértices em que a aresta incide
      int valor1 = str[0] - '0';
      int valor2 = str[2] - '0';

      grau[valor1 - 1]++;
      grau[valor2 - 1]++;

      // Atualiza a representação do grafo
      if (repr == MATRIZ)
      {
         grafo[valor1 - 1][valor2 - 1] = 1;
         grafo[valor2 - 1][valor1 - 1] = 1;
      }
      else
      {
         grafo[valor1 - 1].push_back(valor2 - 1);
         grafo[valor2 - 1].push_back(valor1 - 1);
      }
   }

   file.close();
}

void salvarValores(vector<int> &valor, int N, int M)
{
   // Encontra os valores mínimo e máximo
   // Além do valor médio

   int max = valor[0];
   int min = valor[0];
   float mid = valor[0] / (float)N;

   for (int i = 1; i < N; i++)
   {
      int val = valor[i];
      mid += val / (float)N;

      if (val > max)
         max = val;
      if (val < min)
         min = val;
   }

   // Calcula a mediana
   int mean = 0;
   int vec_aux[N]; // Usa um vetor auxiliar para não mexer com o original

   for (int i = 0; i < N; i++)
   {
      vec_aux[i] = valor[i];
   }

   for (int i = 0; i < N - 1; i++)
   {
      for (int j = i + 1; j < N; j++)
      {

         if (vec_aux[i] > vec_aux[j])
         {

            int aux = vec_aux[i];
            vec_aux[i] = vec_aux[j];
            vec_aux[j] = aux;
         }
      }
   }

   if (N % 2)
   {
      mean = vec_aux[N / 2];
   }
   else
   {
      mean = (vec_aux[N / 2 - 1] + vec_aux[N / 2]) / 2;
   }

   // Escreve esses valores em um arquivo de texto
   ofstream file("valores.txt");

   file << "Número de vértices: " << to_string(N) << endl;
   file << "Número de arestas: " << to_string(M) << endl;
   file << "Grau mínimo: " << to_string(min) << endl;
   file << "Grau máximo: " << to_string(max) << endl;
   file << "Grau médio: " << to_string(mid) << endl;
   file << "Mediana do grau: " << to_string(mean) << endl;

   file.close();
}

void BFS(){}