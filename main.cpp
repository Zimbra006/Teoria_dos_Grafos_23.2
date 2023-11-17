#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <limits>
using namespace std;
using namespace std::chrono;

const float INF = numeric_limits<float>::max(); // Infinito

class MinHeap
{
private:
   vector<pair<float, int>> heap;
   vector<int> map; // Vetor de mapeamento

   // Função auxiliar para ajustar o heap após a inserção
   void heapifyUp(int index)
   {
      int parent = (index - 1) / 2;
      while (index > 0 && heap[index].first < heap[parent].first)
      {
         map[heap[index].second] = parent;
         map[heap[parent].second] = index;
         swap(heap[index], heap[parent]);
         index = parent;
         parent = (index - 1) / 2;
      }
   }

   // Função auxiliar para ajustar o heap após a remoção
   void heapifyDown(int index)
   {
      int left = 2 * index + 1;
      int right = 2 * index + 2;
      int smallest = index;

      if (left < heap.size() && heap[left].first < heap[smallest].first)
      {
         smallest = left;
      }
      if (right < heap.size() && heap[right].first < heap[smallest].first)
      {
         smallest = right;
      }

      if (smallest != index)
      {
         map[heap[index].second] = smallest;
         map[heap[smallest].second] = index;
         swap(heap[index], heap[smallest]);
         heapifyDown(smallest);
      }
   }

public:
   // Inserir um elemento no heap
   void inserir(float dist, int valor)
   {
      heap.push_back({dist, valor});
      map.push_back(valor);
      int index = heap.size() - 1;
      heapifyUp(index);
   }

   // Remover e retornar o elemento mínimo do heap
   int pop()
   {
      if (heap.empty())
      {
         throw out_of_range("Heap is empty");
      }

      int vertice = heap[0].second;
      map[vertice] = -1;
      heap[0] = heap.back();
      map[heap[0].second] = 0;
      heap.pop_back();
      heapifyDown(0);

      return vertice;
   }

   // Verificar se o heap está vazio
   bool vazio()
   {
      return heap.empty();
   }

   // Atualiza um valor do heap
   void atualizar(float novoValor, int indexGrafo)
   {
      int indexHeap = map[indexGrafo];
      heap[indexHeap].first = novoValor;

      int parent = (indexHeap - 1) / 2;
      if (heap[parent].first > novoValor)
      {
         heapifyUp(indexHeap);
      }
      else
      {
         heapifyDown(indexHeap);
      }
   }

   void printMap()
   {
      for (int i = 0; i < map.size(); i++)
      {
         cout << map[i] << " ";
      }
      cout << endl;
   }
};

// Valores usados para referenciar o tipo de representação
const int MATRIZ = 0; // Matriz de adjacência
const int VETOR = 1;  // Vetor de adjacência

// Define a representação que será utilizada
int repr = MATRIZ;

// Declaração das variáveis onde serão postos os valores carregados
int N, M;
vector<int> grau;
vector<vector<float>> grafo;

// Variável que armazena se o grafo possuí algum peso negativo
bool pesoNegativo = false;

// Declaração das funções usadas
void carregarValores(string path);
void carregarValoresComPesos(string path, bool direcionado = false);
void salvarValores();
int BFS(int comeco);
int DFS(int comeco);
void dist(int vertice1, int vertice2);
void diametro(bool aprox = false);
void CC();
void dijkstra(int start, int end, bool heap = false);
float ford_fulkerson(int source, int target);
vector<int> BFS_Ford_Fulkerson(vector<vector<float>> &grafo_r, int source, int target);
void aumentar_fluxo(vector<vector<float>> &fluxo, vector<vector<float>> &grafo_r, vector<int> &caminho);
float gargalo(vector<vector<float>> &grafo_r, vector<int> &caminho);
float capacidade(int i, int j);

int main()
{

   // Carrega os valores do grafo com base no arquivo texto
   // string grafo_analisado = "grafo_W_" + to_string(1);
   string caminho = "text.txt";
   carregarValoresComPesos(caminho, true);

   // Armazena o tempo inicial
   auto start = high_resolution_clock::now();

   ford_fulkerson(1, N);

   // Armazena o tempo final
   auto stop = high_resolution_clock::now();

   // Calcula a duração em milisegundos
   auto duration = duration_cast<milliseconds>(stop - start) / 100.0;

   cout << "A função levou " << duration.count() << " milisegundos." << endl;
}

void carregarValores(string path)
{
   string str;
   ifstream file(path);

   // Extraí o cabeçalho contendo o número de vértices
   getline(file, str);
   N = stoi(str);

   // Atualiza o vetor grau para conter N elementos
   for (int i = 0; i < N; i++)
      grau.push_back(0);

   // Atualiza o vetor grafo com base na sua representação
   for (int i = 0; i < N; i++)
   {
      if (repr == MATRIZ)
      {
         // Caso seja de matriz, adiciona um vetor inteiro só de 0's
         grafo.push_back(vector<float>(N, 0));
      }
      else
      {
         // Caso seja de vetor, adiciona um vetor vazio
         grafo.push_back(vector<float>(0));
      }
   }

   while (getline(file, str))
   {
      // Aumenta a contagem de arestas
      M += 1;

      // Atualiza os graus dos vértices em que a aresta incide
      int valor1 = 0;
      int valor2 = 0;

      string temp;
      int caractere = 0;
      while (str[caractere] != '\0')
      {
         if (str[caractere] != ' ')
         {
            // Append the char to the temp string.
            temp += str[caractere];
         }
         else
         {
            valor1 = stoi(temp);
            temp.clear();
         }
         caractere++;
      }

      valor2 = stoi(temp);

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
         grafo[valor1 - 1].push_back((float)valor2 - 1);
         grafo[valor2 - 1].push_back((float)valor1 - 1);
      }
   }

   file.close();
}

void carregarValoresComPesos(string path, bool direcionado)
{
   string str;
   ifstream file(path);

   // Extraí o cabeçalho contendo o número de vértices
   getline(file, str);
   N = stoi(str);

   // Atualiza o vetor grau para conter N elementos
   for (int i = 0; i < N; i++)
      grau.push_back(0);

   // Atualiza o vetor grafo com base na sua representação
   for (int i = 0; i < N; i++)
   {
      if (repr == MATRIZ)
      {
         // Caso seja de matriz, adiciona um vetor inteiro com o valor máximo
         // Usamos o valor máximo para dizer que não possui conexão entre os vértices
         grafo.push_back(vector<float>(N, INF));
      }
      else
      {
         // Caso seja de vetor, adiciona um vetor vazio
         grafo.push_back(vector<float>(0));
      }
   }

   while (getline(file, str))
   {
      // Aumenta a contagem de arestas
      M += 1;

      // Armazena o peso da aresta
      float peso = 0;

      // Atualiza os graus dos vértices em que a aresta incide
      int valor1 = 0;
      int valor2 = 0;

      string temp;
      int caractere = 0;
      while (str[caractere] != '\0')
      {
         if (str[caractere] != ' ')
         {
            // Append the char to the temp string.
            temp += str[caractere];
         }
         else
         {
            if (!valor1)
            {
               valor1 = stoi(temp);
            }
            else
            {
               valor2 = stoi(temp);
            }
            temp.clear();
         }
         caractere++;
      }

      peso = stof(temp);

      if (peso < 0)
      {
         pesoNegativo = true;
      }

      grau[valor1 - 1]++;
      grau[valor2 - 1]++;

      // Atualiza a representação do grafo
      if (repr == MATRIZ)
      {
         grafo[valor1 - 1][valor2 - 1] = peso;

         if (!direcionado)
         {
            grafo[valor2 - 1][valor1 - 1] = peso;
         }
      }
      else
      {
         grafo[valor1 - 1].push_back((float)valor2 - 1);
         grafo[valor1 - 1].push_back(peso);

         if (!direcionado)
         {

            grafo[valor2 - 1].push_back((float)valor1 - 1);
            grafo[valor2 - 1].push_back(peso);
         }
      }
   }

   file.close();
}

void salvarValores()
{
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
   int vec_aux[N]; // Usa um vetor auxiliar para não mexer com o original

   for (int i = 0; i < N; i++)
   {
      vec_aux[i] = grau[i];
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

int BFS(int comeco)
{
   // Realiza a busca em largura
   // Retorna o vértice mais distante da raiz

   // Trata o começo
   int raiz = comeco - 1;

   // Inicializa o vetor de marcação de vértices
   vector<bool> visitado(N, false);

   // Inicializa o vetor que indica quem é pai de quem
   // O valor de -1 indica que não tem pai, ou seja, é a raiz
   // -1 vai ficar marcado como 0 no arquivo de saída
   vector<int> pai(N, -1);

   // Inicializa o vetor de nível da árvore
   vector<int> nivel(N, 0);

   // Marca o vértice inicial como visitado
   visitado[raiz] = true;

   // Inicializa a fila
   queue<int> fila;
   fila.push(raiz);

   while (!fila.empty())
   {
      // Extrai o próximo vértice a ser analisado e retira ele da fila
      int verticeAtual = fila.front();
      fila.pop();

      // Percorre os vizinhos do vértice atual
      int len = grafo[verticeAtual].size();
      for (int i = 0; i < len; i++)
      {
         if (repr == MATRIZ)
         {
            // Pula se não houver aresta conectando o vértice atual ao vértice i
            if (!grafo[verticeAtual][i])
               continue;
            // Se i não foi visitado
            if (!visitado[i])
            {
               // Marcar como visitado e adicioná-lo à fila
               visitado[i] = true;
               fila.push(i);

               // Marca quem é o pai do vértice adicionado como o vértice atual
               pai[i] = verticeAtual;

               // Marca o nível do vértice adicionado
               nivel[i] = nivel[verticeAtual] + 1;
            }
         }
         else
         {
            // Extraí o vértice sendo visto agora
            int verticeVisto = (int)grafo[verticeAtual][i];
            // Se o vértice visto não foi visitado
            if (!visitado[verticeVisto])
            {
               // Marcar como visitado e adicioná-lo à fila
               visitado[verticeVisto] = true;
               fila.push(verticeVisto);

               // Marca quem é o pai do vértice adicionado como o vértice atual
               pai[verticeVisto] = verticeAtual;

               // Marca o nível do vértice adicionado
               nivel[verticeVisto] = nivel[verticeAtual] + 1;
            }
         }
      }
   }

   // Salva as informações em um arquivo
   ofstream file("BFS.txt");

   file << "Vértice / Nível / Pai" << endl;

   for (int i = 0; i < N; i++)
   {
      file << i + 1 << " / ";
      file << nivel[i] << " / ";
      file << pai[i] + 1 << endl;
   }

   file.close();

   // Retorna o vértice mais profundo na árvore
   int maxDist, maxV;
   maxDist = maxV = 0;
   int len = nivel.size();
   for (int i = 0; i < len; i++)
   {
      if (nivel[i] > maxDist)
      {
         maxDist = nivel[i];
         maxV = i;
      }
   }
   return maxV + 1;
}

int DFS(int comeco)
{
   // Realiza a busca em profundidade
   // Retorna o vértice mais distante da raiz

   // Trata o começo
   int raiz = comeco - 1;

   // Inicializa o vetor de marcação de vértices
   vector<bool> visitado(N, false);

   // Inicializa o vetor que indica quem é pai de quem
   // O valor de -1 indica que não tem pai, ou seja, é a raiz
   // -1 vai ficar marcado como 0 no arquivo de saída
   vector<int> pai(N, -1);

   // Inicializa o vetor de nível da árvore
   vector<int> nivel(N, 0);

   // Inicializa a pilha
   stack<int> pilha;
   pilha.push(raiz);

   while (!pilha.empty())
   {
      // Extrai o próximo vértice a ser analisado e retira ele da pilha
      int verticeAtual = pilha.top();
      pilha.pop();

      // Analisa só vértices não marcados, pois os vértices podem se repetir na pilha
      if (!visitado[verticeAtual])
      {
         // Marca o vértice atual
         visitado[verticeAtual] = true;

         // Percorre os vizinhos do vértice atual
         int len = grafo[verticeAtual].size();
         for (int i = 0; i < len; i++)
         {
            if (repr == MATRIZ)
            {
               // Pula se não houver aresta conectando o vértice atual ao vértice i
               if (!grafo[verticeAtual][i])
                  continue;

               // Adiciona o vizinho à pilha
               pilha.push(i);

               // Se o vértice ainda não teve o pai marcado, significa que ele
               // acabou de ser descoberto
               if (pai[i] == -1)
               {
                  pai[i] = verticeAtual;
                  nivel[i] = nivel[verticeAtual] + 1;
               }
            }
            else
            {
               // Extrai o vizinho sendo visto
               int verticeVisto = (int)grafo[verticeAtual][i];

               // Adiciona o vizinho à pilha
               pilha.push(verticeVisto);

               // Se o vértice ainda não teve o pai marcado, significa que ele
               // acabou de ser descoberto
               if (pai[verticeVisto] == -1)
               {
                  pai[verticeVisto] = verticeAtual;
                  nivel[verticeVisto] = nivel[verticeAtual] + 1;
               }
            }
         }
      }
   }

   // Corrige o pai e o nível da raiz
   pai[raiz] = -1;
   nivel[raiz] = 0;

   // Salva as informações em um arquivo
   ofstream file("DFS.txt");

   file << "Vértice / Nível / Pai" << endl;

   for (int i = 0; i < N; i++)
   {
      file << i + 1 << " / ";
      file << nivel[i] << " / ";
      file << pai[i] + 1 << endl;
   }

   file.close();

   // Retorna o vértice mais profundo na árvore
   int maxDist, maxV;
   maxDist = maxV = 0;
   int len = nivel.size();
   for (int i = 0; i < len; i++)
   {
      if (nivel[i] > maxDist)
      {
         maxDist = nivel[i];
         maxV = i;
      }
   }
   return maxV + 1;
}

void dist(int vertice1, int vertice2)
{
   // Encontra a distância entre dois vértices quaisquer

   // Verifica se os dois vértices são diferentes
   if (vertice1 == vertice2)
   {
      cout << "Por favor, insira vértices distintos" << endl;
      return;
   }

   // Realiza a BFS
   BFS(vertice1);

   // Busca a linha correta no arquivo texto gerado
   string str;
   ifstream file("BFS.txt");

   for (int i = 0; i <= vertice2; i++)
   {
      getline(file, str);
   }

   // Separa os valores do vértice e da ordem na linha atual
   string temp;

   int dist = 0;

   int valor = 0;
   int caractere = 0;

   while (valor < 2)
   {
      if (str[caractere] != '/')
      {
         temp += str[caractere];
      }
      else
      {
         if (valor == 1)
         {
            dist = stoi(temp);
         }

         valor++;
         temp.clear();
      }
      caractere++;
   }

   file.close();

   if (dist == 0)
   {
      cout << "Esses vértices não estão conectados" << endl;
      return;
   }

   cout << "A distância entre esses vértices é " << dist << endl;
}

void diametro(bool aprox)
{
   // Encontra o diâmetro do grafo

   if (!aprox) // Algoritmo para encontrar o diâmetro exato
   {
      int maxDist = 0;
      for (int i = 1; i <= N; i++)
      {
         // Encontra o vértice mais distante do atual
         int maxVertice = BFS(i);

         // Busca a linha correta no arquivo texto gerado para achar a distância entre eles
         string str;
         ifstream file("BFS.txt");

         for (int i = 0; i <= maxVertice; i++)
         {
            getline(file, str);
         }

         // Separa os valores do vértice e da ordem na linha atual
         string temp;

         int dist = 0;

         int valor = 0;
         int caractere = 0;

         while (valor < 2)
         {
            if (str[caractere] != '/')
            {
               temp += str[caractere];
            }
            else
            {
               if (valor == 1)
               {
                  dist = stoi(temp);
               }

               valor++;
               temp.clear();
            }
            caractere++;
         }

         file.close();

         if (dist > maxDist)
         {
            maxDist = dist;
         }
      }

      cout << "O diâmetro do grafo é " << maxDist << endl;
   }
   else // Algoritmo para encontrar um valor mínimo para o diâmetro
   {

      // Começamos realizando uma BFS a partir de um vértice qualquer
      // que vai nos retornar o vértice mais distante desse vértice qualquer
      int maxVertice1 = BFS(1);

      // Agora, realizamos outra BFS a partir desse novo vértice
      // e achamos o vértice mais distante dele
      int maxVertice2 = BFS(maxVertice1);

      // Busca a linha correta no arquivo texto gerado para achar a distância entre eles
      string str;
      ifstream file("BFS.txt");

      for (int i = 0; i <= maxVertice2; i++)
      {
         getline(file, str);
      }

      // Separa os valores do vértice e da ordem na linha atual
      string temp;

      int dist = 0;

      int valor = 0;
      int caractere = 0;

      while (valor < 2)
      {
         if (str[caractere] != '/')
         {
            temp += str[caractere];
         }
         else
         {
            if (valor == 1)
            {
               dist = stoi(temp);
            }

            valor++;
            temp.clear();
         }
         caractere++;
      }

      file.close();

      cout << "O diâmetro do grafo é pelo menos " << dist << endl;
      cout << "Entre os vértices " << maxVertice1 << " e " << maxVertice2 << endl;
   }
}

void CC()
{
   // Encontra (e lista!) as componentes conexas (CC's) do grafo

   // Armazena a quantidade de CC's no grafo
   int num_componentes = 0;

   // Declara um vetor para armazenar as componentes conexas
   vector<vector<int>> componentes;

   // Inicializa o vetor de marcação de vértices
   vector<bool> visitado(N, false);

   // Booleana que armazena se todos os vértices foram visitados
   // Atualizado ao fim de cada iteração
   bool totalmenteVisitado = false;

   // Guarda a raiz da próxima iteração
   int raiz = 1;

   // Começa a busca por componentes conexas: uma sequência de buscas em profundidade
   while (!totalmenteVisitado)
   {
      // Atualiza a quantidade de componentes conexas
      num_componentes++;

      // Adiciona um vetor representando a componente conexa atual
      componentes.push_back(vector<int>(0));

      // Marca a próxima raiz como visitado
      visitado[raiz] = true;

      // Inicializa a fila
      queue<int> fila;
      fila.push(raiz);

      while (!fila.empty())
      {
         // Extrai o próximo vértice a ser analisado e retira ele da fila
         int verticeAtual = fila.front();
         fila.pop();

         // Insere o vértice na componente conexa atual
         componentes[num_componentes - 1].push_back(verticeAtual);

         // Percorre os vizinhos do vértice atual
         int len = grafo[verticeAtual].size();
         for (int i = 0; i < len; i++)
         {
            if (repr == MATRIZ)
            {
               // Pula se não houver aresta conectando o vértice atual ao vértice i
               if (!grafo[verticeAtual][i])
                  continue;
               // Se i não foi visitado
               if (!visitado[i])
               {
                  // Marcar como visitado e adicioná-lo à fila
                  visitado[i] = true;
                  fila.push(i);
               }
            }
            else
            {
               // Extraí o vértice sendo visto agora
               int verticeVisto = grafo[verticeAtual][i];
               // Se o vértice visto não foi visitado
               if (!visitado[verticeVisto])
               {
                  // Marcar como visitado e adicioná-lo à fila
                  visitado[verticeVisto] = true;
                  fila.push(verticeVisto);
               }
            }
         }
      }

      // Assume que todos os vértices foram visitados
      totalmenteVisitado = true;

      // Agora verifica se todos os vértices foram visitados
      for (int i = 0; i < N; i++)
      {
         if (!visitado[i])
         {
            // A próxima BFS começará a partir do primeiro vértice não marcado
            // que foi encontrado
            raiz = i;
            totalmenteVisitado = false;
            break;
         }
      }

      /*
         Obs.: totalmenteVisitado é posto como true antes do loop pois é uma maneira eficiente
         de verificar se visitamos todos ou não: se sair do loop continuando verdadeiro, signi-
         fica que ele não achou um vértice não visitado!
      */
   }

   // Salva as informações num arquivo texto
   ofstream file("CC.txt");

   // Coloca a quantidade de CC's no cabeçalho
   file << num_componentes << endl;

   file << "Tamanho / Lista" << endl;

   // Lista as componentes em ordem decrescente de tamanho
   while (!componentes.empty())
   {
      // Encontra a CC de maior tamanho
      int maxSize, maxCC;
      maxSize = maxCC = 0;

      int len = componentes.size();
      for (int i = 0; i < len; i++)
      {
         int tamanhoAtual = componentes[i].size();
         if (tamanhoAtual > maxSize)
         {
            maxSize = tamanhoAtual;
            maxCC = i;
         }
      }

      // Lista ela
      file << maxSize << " / ";

      int lenCC = componentes[maxCC].size();
      for (int i = 0; i < lenCC; i++)
      {
         file << componentes[maxCC][i] + 1 << ", ";
      }

      file << endl;

      // Apaga ela
      componentes.erase(componentes.begin() + maxCC);
   }

   file.close();
}

void dijkstra(int start, int end, bool heap)
{

   if (pesoNegativo)
   {
      cout << "A biblioteca ainda não implementa caminhos mínimos com pesos negativos" << endl;
      return;
   }

   vector<bool> visitado(N, false); // Vetor que armazena os vértices que já foram visitados

   vector<int> pai(N, -1); // Vetor que armazena o pai de cada vértice na busca

   vector<float> dist(N); // Vetor que armazena as distância

   if (heap)
   {
      MinHeap minHeap;

      for (int i = 0; i < N; i++)
      {
         dist[i] = INF;
         minHeap.inserir(INF, i);
      }

      // Define a distância ao vértice inicial como 0
      dist[start - 1] = 0;
      minHeap.atualizar(0, start - 1);

      while (!minHeap.vazio())
      {
         // Retiramos o vértice com a menor distância (o ultimo da fila)
         int u = minHeap.pop();

         visitado[u] = true;

         // Atualiza a distância dos vizinhos
         int len = grafo[u].size();
         for (int v = 0; v < len; v++)
         {
            if (repr == MATRIZ)
            {
               if (visitado[v])
                  continue; // Pula vértices já visitados

               if (dist[v] > dist[u] + grafo[u][v])
               {
                  dist[v] = dist[u] + grafo[u][v];
                  minHeap.atualizar(dist[v], v);
                  pai[v] = u;
               }
            }
            else
            {
               if (v % 2 == 1)
                  continue; // Pula as vezes que grafo[u][v] é o peso da aresta

               int vertice = grafo[u][v];
               float peso = grafo[u][v + 1];

               if (visitado[vertice])
                  continue; // Pula vértices já visitados

               if (dist[vertice] > dist[u] + peso)
               {
                  dist[vertice] = dist[u] + peso;
                  minHeap.atualizar(dist[vertice], vertice);
                  pai[vertice] = u;
               }
            }
         }
      }
   }
   else
   {
      // Usando Vetor para armazenar a estimativa de distância

      for (int i = 0; i < N; i++)
      {
         dist[i] = INF;
      }

      dist[start - 1] = 0; // Define a distância do vértice de início como zero

      // Algoritmo executado para todos os vértices
      for (int count = 0; count < N; count++)
      {
         int u = -1; // Inicia como -1 poque ainda não tem vértice selecionado

         // Encontrando o vértice não visitado com a menor distância, armazena-lo em u
         for (int j = 0; j < N; j++)
         {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u]))
            {
               u = j;
            }
         }

         visitado[u] = true; // Marca o vértice u como visitado

         // Atualiza a distância dos vizinhos
         int len = grafo[u].size();
         for (int v = 0; v < len; v++)
         {
            if (repr == MATRIZ)
            {
               if (visitado[v])
                  continue; // Pula vértices já visitados

               if (dist[v] > dist[u] + grafo[u][v])
               {
                  dist[v] = dist[u] + grafo[u][v];
                  pai[v] = u;
               }
            }
            else
            {
               if (v % 2 == 1)
                  continue; // Pula as vezes que grafo[u][v] é igual ao peso da aresta

               int vertice = grafo[u][v];
               float peso = grafo[u][v + 1];

               if (visitado[vertice])
                  continue; // Pula vértices já visitados

               if (dist[vertice] > dist[u] + peso)
               {
                  dist[vertice] = dist[u] + peso;
                  pai[vertice] = u;
               }
            }
         }
      }
   }
   // Para quando encontra todas as menores distâncias

   cout << "Distância de " << start << " até " << end << ": " << dist[end - 1] << endl;

   cout << "Caminho Mínimo:" << endl;

   int vertice = end - 1;
   while (vertice != start - 1)
   {
      cout << vertice + 1 << endl;
      vertice = pai[vertice];
   }

   cout << vertice + 1 << endl;

   ofstream file("Dijkstra_" + to_string(start) + ".txt");

   file << "Vértice / Distância / Pai" << endl;
   // Arquivo com a menor distância para todos os vértices
   for (int i = 0; i < N; i++)
   {
      file << i + 1 << " / " << dist[i] << " / " << pai[i] + 1 << endl;
   }

   file.close();
}

float ford_fulkerson(int source, int target)
{
   // Inicializar vetor de fluxo
   vector<vector<float>> fluxo(N, vector<float>(0));

   // Inicializar grafo residual
   vector<vector<float>> grafo_r(N, vector<float>(0));

   for (int i = 0; i < N; i++)
   {
      for (int j = 0; j < N; j++)
      {  
         float c = capacidade(i, j);
         if (c != INF) // Se existir aresta (i,j)
         {
            // Atualizar ela com fluxo 0
            fluxo[i].push_back(j);
            fluxo[i].push_back(0);

            // Adicionar aresta original
            grafo_r[i].push_back(j);
            grafo_r[i].push_back(c);

            // Adicionar aresta reserva
            grafo_r[j].push_back(i);
            grafo_r[j].push_back(0);
         }
      }
   }

   // Encontrar caminho entre s e t no grafo residual (rodar uma BFS e achar o caminho)
   vector<int> caminho = BFS_Ford_Fulkerson(grafo_r, source, target);

   // Enquanto houver caminho:
   while (caminho.size() > 1)
   {
      aumentar_fluxo(fluxo, grafo_r, caminho);

      // Atualizar grafo residual
      for (int i = 0; i < N; i++)
      {
         int len = grafo_r[i].size();
         for (int j = 0; j < len; j++)
         {  
            if (j % 2 == 1) // Pula quando estivermos olhando para a capacidade da aresta
               continue;
            
            int verticeVisto = grafo_r[i][j]; // Extrai qual vértice vizinho de i estamos analisando
            float c = capacidade(i, verticeVisto); // Extrai a capacidade da aresta

            if (c != INF)
            {  
               // Encontrar o fluxo entre essas arestas
               int k = 0;
               while (fluxo[i][k] != verticeVisto)
               {
                  k += 2;
               }
               
               // Atualiza a aresta original
               grafo_r[i][j + 1] = c - fluxo[i][k + 1];

               // Atualiza aresta reversa
               int l = 0;
               while (grafo_r[verticeVisto][l] != i)
               {
                  l += 2;
               }
               grafo_r[verticeVisto][l + 1] = fluxo[i][k + 1];
            }
         }
      }

      // Encontrar novo caminho
      caminho = BFS_Ford_Fulkerson(grafo_r, source, target);
   }

   cout << endl << "Fluxo: " << endl;

   for (int i = 0; i < N; i++)
   {
      cout << i << " ";
      int len = fluxo[i].size();
      for (int j = 0; j < len; j++)
      {
         if (j % 2 == 0)
         {
            cout << "| ";
         }

         cout << fluxo[i][j] << " ";
      }

      cout << endl;
   }

   cout << "Grafo Residual: " << endl;

   for (int i = 0; i < N; i++)
   {
      cout << i << " ";
      int len = grafo_r[i].size();
      for (int j = 0; j < len; j++)
      {
         if (j % 2 == 0)
         {
            cout << "| ";
         }

         cout << grafo_r[i][j] << " ";
      }

      cout << endl;
   }

   // Retornar fluxo máximo
   float fluxoMax = 0;
   for (int i = 0; i < N; i++)
   {
      if (i % 2 == 0) // Pular quando fluxo[source - 1][i] for algum vértice vizinho de source
         continue;

      fluxoMax += fluxo[source - 1][i];
   }

   cout << "Fluxo máximo: " << fluxoMax << endl;

   return fluxoMax;
}

vector<int> BFS_Ford_Fulkerson(vector<vector<float>> &grafo_r, int source, int target)
{
   int raiz = source - 1;

   vector<bool> visitado(N, false);

   vector<int> pai(N, -1);

   visitado[raiz] = true;

   queue<int> fila;
   fila.push(raiz);

   while (!fila.empty())
   {
      // Extrai o próximo vértice a ser analisado e retira ele da fila
      int verticeAtual = fila.front();
      fila.pop();

      // Percorre os vizinhos do vértice atual
      int len = grafo_r[verticeAtual].size();
      for (int i = 0; i < len; i++)
      {
         if (i % 2 == 1) // Pula as vezes que olharíamos a capacidade da aresta
            continue;

         // Pula se a capacidade da aresta é 0
         if (!grafo_r[verticeAtual][i + 1])
            continue;

         // Extraí o vértice sendo visto agora
         int verticeVisto = (int)grafo_r[verticeAtual][i];

         // Se o vértice visto não foi visitado
         if (!visitado[verticeVisto])
         {
            // Marcar como visitado e adicioná-lo à fila
            visitado[verticeVisto] = true;
            fila.push(verticeVisto);

            // Marca quem é o pai do vértice adicionado como o vértice atual
            pai[verticeVisto] = verticeAtual;
         }
      }
   }

   vector<int> caminho;

   int vertice = target - 1;
   while (pai[vertice] != -1)
   {
      caminho.push_back(vertice);
      vertice = pai[vertice];
   }

   // Se houver caminho, adicionar vértice source à ele
   // Se não houver, adiciona o vértice target
   caminho.push_back(vertice);

   return caminho;
}

void aumentar_fluxo(vector<vector<float>> &fluxo, vector<vector<float>> &grafo_r, vector<int> &caminho)
{
   float g = gargalo(grafo_r, caminho);
   int tamanho = caminho.size();
   for (int i = tamanho - 1; i > 0; i--)
   {
      // Procurar pela aresta (caminho[i], caminho[i - 1]) no fluxo
      int j = 0;
      while (fluxo[caminho[i]][j] != caminho[i - 1])
      {
         j += 2;
      }

      if (capacidade(caminho[i], caminho[i - 1]) != INF)
      {
         fluxo[caminho[i]][j + 1] += g;
      }
      else
      {
         fluxo[caminho[i]][j + 1] -= g;
      }
   }
}

float gargalo(vector<vector<float>> &grafo_r, vector<int> &caminho)
{
   int tamanho = caminho.size();
   float menorCapacidade = INF;
   for (int i = tamanho - 1; i > 0; i--)
   {
      // Procurar pela aresta (caminho[i], caminho[i - 1]) no grafo residual
      int j = 0;
      while (grafo_r[caminho[i]][j] != caminho[i - 1])
      {
         j += 2;
      }

      // Acessar sua capacidade atual
      int capacidadeAtual = grafo_r[caminho[i]][j + 1];

      if (capacidadeAtual < menorCapacidade)
      {
         menorCapacidade = capacidadeAtual;
      }
   }

   return menorCapacidade;
}

float capacidade(int i, int j)
{
   // Uma função para encontrar a capacidade da aresta (i, j) no grafo
   // Retorna INF se não houver aresta (i, j)
   if (repr == MATRIZ)
   {
      return grafo[i][j];
   }
   else
   {
      // Busca pelo vértice no vetor de adjacência
      int len = grafo[i].size();
      for (int k = 0; k < len; k += 2)
      {
         if (grafo[i][k] == j)
         {
            return grafo[i][k + 1];
         }
      }
      
      return INF;
   }
}