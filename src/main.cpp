#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include "../inc/node.h"

using namespace std;

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
// Creamos un mapa que asigna cada caracter a un codigo de Huffman
void CreateCodes(Node* raiz, const string &code , std::unordered_map<char, string> &codes)
{
    // Caso base, si el arbol esta vacio, no hacemos nada
    if(!raiz)
    {
        return;
    }

    //Estamos en una hoja, guardamos su caracter
    if(!raiz->left && !raiz->right)
    {
        codes[raiz->ch] = code;
    }

    CreateCodes(raiz->left, code + "0", codes);
    CreateCodes(raiz->right, code + "1", codes);
}

// Función que cuenta las veces que se repite cada caracter en string
std::unordered_map<char,int> FrCount( std::unordered_map<char,int> f, std::string t ){
    for ( auto ch : t)
    {
        f[ch]++;
    }
     std::cout << "Frecuencias de los caracteres:\n";
    for (const auto& pair : f) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
    
    return f;
}

string CodingText (string &text, std::unordered_map<char,string> &codes) 
{
    string EncodedText;
    for (auto c : text)
    {
        EncodedText +=codes.at(c);
    }
    
    return EncodedText;
}

string codificar(string text)
{   
    std::unordered_map<char,int> freq;
    std::unordered_map<char,string> codes;
    string encodeText;

    //Calculamos la frecuencia;
    freq = FrCount(freq, text);

    //Con la frecuencia calculada, creamos el arbol
    Node* root = HuffmanTree(freq);
    
    //Creamos la tabla de codificación que mapea entre posiciones.
    CreateCodes(root,"",codes);
    
    //Y por ultimo devolvemos el texto codificado
    std::cout << "Tabla de códigos de Huffman:\n";
    for (const auto& pair : codes) {
    std::cout << pair.first << ": " << pair.second << "\n";
    }
    return encodeText= CodingText(text,codes);

}

int main()
{  
    string text ="tangananica-tanganana";
    string encode;
    encode = codificar(text);

    cout<<encode;
    
    

    
    return 0;
}
