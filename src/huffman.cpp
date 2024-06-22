#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
<<<<<<< Updated upstream
=======
#include <fstream>
#include <chrono>
#include <bitset>
>>>>>>> Stashed changes

using namespace std;

/* Estructura Node: Define un nodo del árbol de Huffman, 
que contiene un símbolo una frecuencia y punteros a los hijos izquierdo (izq) y derecho (der).
*/
struct node
{
    char simbolo;
    int frecuencia;
    node *izq, *der;
};

/*
get_node: Crea un nuevo nodo del árbol de Huffman con los parámetros dados y devuelve un puntero a este nodo.
*/
node *get_node(char simbolo, int frecuencia, node *izq, node *der)
{
    node *nodo = new node();
    nodo->simbolo = simbolo;
    nodo->frecuencia = frecuencia;
    nodo->izq = izq;
    nodo->der = der;

    return nodo;
}
/*
Estructura comparacion:
Define una estructura de comparación para los nodos del árbol de Huffman. 
Esto se utiliza en la priority_queue para asegurar que los nodos se ordenen por frecuencia ascendente.
*/
struct comparacion
{
    bool operator()(node *i, node *d)
    {
        return i->frecuencia > d->frecuencia;
    }
};
/*
Funcion codificar:
Recorre el árbol de Huffman recursivamente para generar códigos binarios (0 y 1) 
para cada símbolo y los almacena en el mapa CodigoHuffman.
*/
void codificar(node *raiz, string str, unordered_map<char, string> &CodigoHuffman)
{
    if (raiz == nullptr)
    {
        return;
    }
    if (!raiz->izq && !raiz->der)
    {
        CodigoHuffman[raiz->simbolo] = str;
    }
    codificar(raiz->izq, str + "0", CodigoHuffman);
    codificar(raiz->der, str + "1", CodigoHuffman);
}

/*
Funcion decodificar: Recibe una cadena binaria str y la decodifica según el árbol de Huffman raiz. 
Recorre el árbol siguiendo los bits (0 o 1) de str hasta llegar a un nodo hoja, 
imprimiendo el símbolo correspondiente.
*/
void decodificar(node *raiz, int &index, string str)
{
    if (raiz == nullptr)
    {
        return;
    }
    if (!raiz->izq && !raiz->der)
    {
        cout << raiz->simbolo;
        return;
    }
    index++;
    if (str[index] == '0')
    {
        decodificar(raiz->izq, index, str);
    }
    else
    {
        decodificar(raiz->der, index, str);
    }
}
/*
Funcion crear_arbol: Calcula las frecuencias de cada carácter en la cadena de texto proporcionada.
Utiliza una priority_queue (pq) para construir el árbol de Huffman: 
primero crea nodos hoja para cada carácter con su frecuencia, 
los inserta en la cola de prioridad y 
luego combina repetidamente los dos nodos con la menor frecuencia hasta que queda un único nodo en la cola, 
que es la raíz del árbol de Huffman.
*/
void crear_arbol(string text)
{
    unordered_map<char, int> frecuencia;
    for (char simbolo : text)
    {
        frecuencia[simbolo]++;
    }
    priority_queue<node *, vector<node *>, comparacion> pq;

    for (auto pair : frecuencia)
    {
        pq.push(get_node(pair.first, pair.second, nullptr, nullptr));
    }
    while (pq.size() != 1)
    {
        node *izq = pq.top();
        pq.pop();
        node *der = pq.top();
        pq.pop();
        int sum = izq->frecuencia + der->frecuencia;
        pq.push(get_node('\0', sum, izq, der));
    }
    node *raiz = pq.top();

    unordered_map<char, string> CodigoHuffman;
    codificar(raiz, "", CodigoHuffman);
    cout << "\n Los codigos de Huffman son: \n"
         << endl;
    for (auto pair : CodigoHuffman)
    {
        cout << pair.first << " " << pair.second << endl;
    }

    cout << "\n El mensaje original era: \n"
         << text << endl;
    string str = "";
    for (char simbolo : text)
    {
        str += CodigoHuffman[simbolo];
    }
<<<<<<< Updated upstream
    cout << "\n El texto codificado es: \n"
         << str << endl;
    int index = -1;
    cout << "\n El texto decodificado es: \n";
    while (index < (int)str.size() - 1)
    {
        decodificar(raiz, index, str);
    }
    cout << "\n\n\n";
=======

    // Guardar códigos de Huffman y el texto codificado en el archivo de salida
    output_file << CodigoHuffman.size() << endl;
    int sizet = 0;
    std::string ConvertiraBinario;
    for (auto pair : CodigoHuffman)
    {
        output_file << pair.first << " " << pair.second << endl;
        ConvertiraBinario.append(pair.second);

    } 
    ConvertiraBinario+=str;
    // inicio Prueba para convertir en Archivo binario.
    if (ConvertiraBinario.size() % 8 != 0)
    {
        size_t length = ConvertiraBinario.length();
        size_t padding = (8 - (length % 8)) % 8;  // Calcular el relleno necesario
        
        ConvertiraBinario = std::string(padding, '0') + ConvertiraBinario;
    }
    
     // Convertir la cadena binaria a bytes
    std::vector<unsigned char> byte_array;
    for (size_t i = 0; i < ConvertiraBinario.length(); i += 8) {
        std::bitset<8> byte(ConvertiraBinario.substr(i, 8));
        byte_array.push_back(static_cast<unsigned char>(byte.to_ulong()));
    }
    std::ofstream output1("funcionar.bin", std::ios::binary);
    if (!output1) {
        std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
    }
    output1.write(reinterpret_cast<const char*>(byte_array.data()), byte_array.size());
    output1.close();

    std::cout << "Archivo guardado correctamente." << std::endl;

    output_file << str;
    output_file.close();
>>>>>>> Stashed changes
}


int main(int argc, char const *argv[])
{
    string text = "tangananica-tanganana";

    crear_arbol(text);
    cout << "\n";
    return 0;
}
