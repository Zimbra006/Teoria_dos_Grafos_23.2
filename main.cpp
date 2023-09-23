#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void salvarValores(vector<int> &, int, int);
void reprMatriz(string, vector<vector<int>> &);
void reprVetor(string, vector<vector<int>> &);

int main()
{

   int N, M;
   N = M = 0;

   string str;
   // Abrindo um arquivo em modo de leitura
   ifstream file("text.txt");

   // Extraí o caractere no cabeçalho para colocá-lo em N
   getline(file, str);
   N = stoi(str);

   // Inicializa um array de grau para todos os vértices
   vector<int> grau(N, 0);

   while (getline(file, str))
   {
      // Atualiza os graus dos vértices em que a aresta incide
      int valor1 = str[0] - '0';
      int valor2 = str[2] - '0';

      grau[valor1 - 1]++;
      grau[valor2 - 1]++;

      M++;
   }

   file.close();

   // salvarValores(grau, N, M);

   // Representar o grafo com uma matriz
   vector<vector<int>> matriz(N, vector<int>(N, 0));
   reprMatriz("text.txt", matriz);

   // Representar o grafo com vetor de adjacência
   vector<vector<int>> vetor(N);
   reprVetor("text.txt", vetor);
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

   ofstream file("valores.txt");

   file << "Número de vértices: " << to_string(N) << endl;
   file << "Número de arestas: " << to_string(M) << endl;
   file << "Grau mínimo: " << to_string(min) << endl;
   file << "Grau máximo: " << to_string(max) << endl;
   file << "Grau médio: " << to_string(mid) << endl;
   file << "Mediana do grau: " << to_string(mean) << endl;

   file.close();
}

void reprMatriz(string path, vector<vector<int>> &M)
{

   string str;
   ifstream file(path);

   getline(file, str);

   while (getline(file, str))
   {
      int valor1 = str[0] - '0' - 1;
      int valor2 = str[2] - '0' - 1;

      M[valor1][valor2] = 1;
      M[valor2][valor1] = 1;
   }

   file.close();
}

void reprVetor(string path, vector<vector<int>> &V)
{
   string str;
   ifstream file(path);

   getline(file, str);

   while (getline(file, str))
   {
      int valor1 = str[0] - '0' - 1;
      int valor2 = str[2] - '0' - 1;

      V[valor1].push_back(valor2);
      V[valor2].push_back(valor1);
   }

   file.close();
}

void BFS() {}