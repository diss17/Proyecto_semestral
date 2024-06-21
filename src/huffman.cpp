#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>

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
void decodificar(node *raiz, int &index, string str, ofstream &output_file)
{
    if (raiz == nullptr)
    {
        return;
    }
    if (!raiz->izq && !raiz->der)
    {
        output_file << raiz->simbolo;
        return;
    }
    index++;
    if (str[index] == '0')
    {
        decodificar(raiz->izq, index, str, output_file);
    }
    else
    {
        decodificar(raiz->der, index, str, output_file);
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
void crear_arbol(const string &input_file_path, const string &output_file_path)
{
    ifstream input_file(input_file_path);
    if (!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo de entrada!" << endl;
        return;
    }

    string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    input_file.close();

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

    ofstream output_file(output_file_path, ios::binary);
    if (!output_file.is_open())
    {
        cerr << "No se puede abrir el archivo de salida!" << endl;
        return;
    }

    string str = "";
    for (char simbolo : text)
    {
        str += CodigoHuffman[simbolo];
    }

    // Guardar códigos de Huffman y el texto codificado en el archivo de salida
    output_file << CodigoHuffman.size() << endl;
    for (auto pair : CodigoHuffman)
    {
        output_file << pair.first << " " << pair.second << endl;
    }
    output_file << str;
    output_file.close();
}

void descomprimir(const string &input_file_path, const string &output_file_path)
{
    ifstream input_file(input_file_path, ios::binary);
    if (!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo de entrada!" << endl;
        return;
    }

    unordered_map<string, char> CodigoHuffman;
    size_t map_size;
    input_file >> map_size;
    input_file.ignore();

    for (size_t i = 0; i < map_size; ++i)
    {
        char simbolo;
        string codigo;
        input_file.get(simbolo);
        input_file.get(); // Leer el espacio
        getline(input_file, codigo);
        CodigoHuffman[codigo] = simbolo;
    }

    string codificado((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    input_file.close();

    ofstream output_file(output_file_path);
    if (!output_file.is_open())
    {
        cerr << "No se puede abrir el archivo de salida!" << endl;
        return;
    }

    string buffer = "";
    for (char bit : codificado)
    {
        buffer += bit;
        if (CodigoHuffman.find(buffer) != CodigoHuffman.end())
        {
            output_file << CodigoHuffman[buffer];
            buffer = "";
        }
    }

    output_file.close();
}

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " <comprimir/descomprimir> <archivo_entrada> <archivo_salida>" << endl;
        return 1;
    }

    string mode = argv[1];
    string input_file_path = argv[2];
    string output_file_path = argv[3];

    if (mode == "comprimir")
    {
        crear_arbol(input_file_path, output_file_path);
    }
    else if (mode == "descomprimir")
    {
        descomprimir(input_file_path, output_file_path);
    }
    else
    {
        cerr << "Modo desconocido: " << mode << endl;
        return 1;
    }

    return 0;
}
