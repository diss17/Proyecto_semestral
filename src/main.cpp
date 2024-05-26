#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
using namespace std;

//Nodo para la cola de prioridad 
struct Node
{
    char ch;
    int freq;
    Node* left;
    Node* right;


    Node(char c, int f) : ch(c), freq(f),left(nullptr), right(nullptr){}
};
// Comparador personalizado para la cola de prioridad
struct Compare 
{
    bool operator()(Node* left, Node* right) 
    {
        return left->freq > right->freq;
    }
};

Node* HuffmanTree(std::unordered_map<char,int> &f)
{
    // Cola de prioridad que almacená los nodos
    std::priority_queue<Node*, std::vector<Node*>, Compare> queue;

    // Agregamos los nodos a la queue
    for (auto node : f)
    {
        queue.push(new Node(node.first, node.second));
    }

    while (queue.size() > 1)
    {
        // Sacamos los nodos que menos se repiten
        Node* left = queue.top();
        queue.pop();
        Node* right = queue.top();
        queue.pop();

        // Los nodos anteriores como hijos, le creamos un padre que sea la suma de las frecuencias
        Node* father = new Node('\0',left->freq + right->freq);
        father->left = left;
        father->right = right;

        queue.push(father);

    }
    //devolvemos la raiz de nuestro arbol
    return queue.top();

};

// Función que cuenta las veces que se repite cada caracter en string
std::unordered_map<char,int> FrCount( std::unordered_map<char,int> f, std::string t ){
    for ( auto ch : t)
    {
        f[ch]++;
    }
    
    return f;
}

int main()
{  
    std::unordered_map<char,int> freq;


    string text ="tangananica-tanganana";
    
    freq = FrCount(freq, text);

    for (auto ch : freq)
    {
        cout<<"Caracter: "<< ch.first << " Frecuencia: "<<ch.second<<endl; 
    }
    
    

    
    return 0;
}
