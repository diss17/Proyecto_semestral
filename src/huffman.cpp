#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <vector>
#include <bitset>
#include <filesystem> // Para obtener el tamaño del archivo 

using namespace std;

/**
 * @brief Representa un nodo en el árbol de Huffman.
 */
struct node
{
    char simbolo;    ///< El símbolo almacenado en el nodo
    int frecuencia;  ///< La frecuencia del símbolo
    node *izq;       ///< Puntero al hijo izquierdo
    node *der;       ///< Puntero al hijo derecho
};

/**
 * @brief Crea un nuevo nodo del árbol de Huffman.
 * 
 * @param simbolo El símbolo que representa el nodo
 * @param frecuencia La frecuencia del símbolo
 * @param izq Puntero al hijo izquierdo
 * @param der Puntero al hijo derecho
 * @return node* Puntero al nuevo nodo
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
/** 
* @brief Define una estructura de comparación para los nodos del árbol de Huffman. 
* Esto se utiliza en la priority_queue para asegurar que los nodos se ordenen por frecuencia ascendente.
*/
struct comparacion
{
    bool operator()(node *i, node *d)
    {
        return i->frecuencia > d->frecuencia;
    }
};

/**
 * @brief Codifica el árbol de Huffman y almacena los códigos de Huffman para cada símbolo.
 * 
 * Esta función recorre el árbol de Huffman de manera recursiva y genera los códigos de Huffman para cada símbolo.
 * 
 * @param raiz Puntero a la raíz del árbol de Huffman
 * @param str Cadena de bits que representa el código de Huffman actual
 * @param CodigoHuffman Mapa para almacenar los códigos de Huffman generados para cada símbolo
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

/**
 * @brief Crea un árbol de Huffman a partir de un archivo de texto, codifica el texto y guarda el resultado en un archivo binario.
 *
 * Esta función lee un archivo de texto, construye un árbol de Huffman basado en la frecuencia de los caracteres,
 * codifica el texto utilizando el árbol de Huffman y escribe el mapa de códigos junto con el texto codificado en un archivo binario.
 *
 * @param input_file_path La ruta del archivo de texto de entrada.
 * @param output_file_path La ruta del archivo binario de salida donde se guardará el mapa de códigos y el texto codificado.
 */
void crear_arbol(const string &input_file_path, const string &output_file_path)
{
    // Abre el archivo de entrada en modo texto
    ifstream input_file(input_file_path);
    if (!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo de entrada!" << endl;
        return;
    }

    // Lee todo el contenido del archivo de entrada en una sola cadena
    string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    input_file.close();

    // Mapa para almacenar la frecuencia de cada símbolo en el texto
    unordered_map<char, int> frecuencia;
    for (char simbolo : text)
    {
        frecuencia[simbolo]++;
    }

    // Cola de prioridad para construir el árbol de Huffman
    priority_queue<node *, vector<node *>, comparacion> pq;

    // Crea un nodo hoja para cada símbolo y lo agrega a la cola de prioridad
    for (auto pair : frecuencia)
    {
        pq.push(get_node(pair.first, pair.second, nullptr, nullptr));
    }

    // Construye el árbol de Huffman combinando los nodos de menor frecuencia
    while (pq.size() != 1)
    {
        node *izq = pq.top();
        pq.pop();
        node *der = pq.top();
        pq.pop();
        int sum = izq->frecuencia + der->frecuencia;
        pq.push(get_node('\0', sum, izq, der));
    }

    // El nodo raíz del árbol de Huffman
    node *raiz = pq.top();

    // Mapa para almacenar los códigos de Huffman de cada símbolo
    unordered_map<char, string> CodigoHuffman;
    codificar(raiz, "", CodigoHuffman);

    // Cadena para almacenar el texto codificado
    string str = "";
    for (char simbolo : text)
    {
        str += CodigoHuffman[simbolo];
    }

    // Abre el archivo de salida en modo binario
    ofstream output_file(output_file_path, ios::binary);
    if (!output_file.is_open())
    {
        cerr << "No se puede abrir el archivo de salida!" << endl;
        return;
    }

    // Escribe el tamaño del mapa de códigos de Huffman en el archivo de salida
    size_t map_size = CodigoHuffman.size();
    output_file.write(reinterpret_cast<const char*>(&map_size), sizeof(map_size));

    // Escribe cada par (símbolo, código de Huffman) en el archivo de salida
    for (auto pair : CodigoHuffman)
    {
        output_file.write(&pair.first, sizeof(char));
        size_t codigo_length = pair.second.size();
        output_file.write(reinterpret_cast<const char*>(&codigo_length), sizeof(codigo_length));
        output_file.write(pair.second.c_str(), codigo_length);
    }

    // Convierte la cadena de bits en bytes y la escribe en el archivo de salida
    string byte_str;
    for (size_t i = 0; i < str.size(); i += 8)
    {
        bitset<8> bits(str.substr(i, 8));
        byte_str += char(bits.to_ulong());
    }

    output_file.write(byte_str.c_str(), byte_str.size());
    output_file.close();
}

/**
 * @brief Decodifica un archivo codificado usando un árbol de Huffman y escribe el resultado en un archivo de salida.
 *
 * Esta función lee un archivo codificado en formato binario que contiene un mapa de códigos de Huffman 
 * seguido de datos codificados. Utiliza este mapa para decodificar los datos y escribir el resultado 
 * en un archivo de texto de salida.
 *
 * @param input_file_path La ruta del archivo de entrada codificado.
 * @param output_file_path La ruta del archivo de salida donde se escribirá el texto decodificado.
 */
void decodificar(const string &input_file_path, const string &output_file_path)
{
    // Abre el archivo de entrada en modo binario
    ifstream input_file(input_file_path, ios::binary);
    if (!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo de entrada!" << endl;
        return;
    }

    // Mapa para almacenar los códigos de Huffman y sus correspondientes símbolos
    unordered_map<string, char> CodigoHuffman;
    size_t map_size;

    // Lee el tamaño del mapa de códigos de Huffman
    input_file.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));

    // Lee cada par (código de Huffman, símbolo) del archivo de entrada y los almacena en el mapa
    for (size_t i = 0; i < map_size; ++i)
    {
        char simbolo;
        input_file.read(&simbolo, sizeof(char));
        size_t codigo_length;
        input_file.read(reinterpret_cast<char*>(&codigo_length), sizeof(codigo_length));
        string codigo(codigo_length, ' ');
        input_file.read(&codigo[0], codigo_length);
        CodigoHuffman[codigo] = simbolo;
    }

    // Variable para almacenar la cadena codificada leída del archivo
    string codificado;
    char byte;

    // Lee el archivo codificado byte por byte y convierte cada byte a su representación binaria
    while (input_file.read(&byte, sizeof(char)))
    {
        bitset<8> bits(byte);
        codificado += bits.to_string();
    }
    input_file.close();

    // Abre el archivo de salida en modo texto
    ofstream output_file(output_file_path);
    if (!output_file.is_open())
    {
        cerr << "No se puede abrir el archivo de salida!" << endl;
        return;
    }

    // Variable buffer para acumular bits y buscar en el mapa de códigos de Huffman
    string buffer = "";

    // Recorre la cadena codificada bit por bit
    for (char bit : codificado)
    {
        buffer += bit;

        // Si el buffer coincide con un código de Huffman en el mapa, escribe el símbolo correspondiente
        if (CodigoHuffman.find(buffer) != CodigoHuffman.end())
        {
            output_file << CodigoHuffman[buffer];
            buffer = "";
        }
    }

    // Cierra el archivo de salida
    output_file.close();
}


int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "Uso: " << argv[0] << " <codificar/decodifcar> <archivo_entrada> <archivo_salida>" << endl;
        return 1;
    }

    string mode = argv[1];
    string input_file_path = argv[2];
    string output_file_path = argv[3];

    if (mode == "codificar")
    {
        ofstream timefile("resultados_tiempos_codificacionhuffman.csv", ios::app);
        for (int i = 0; i < 5; ++i) {
            auto start_time = chrono::high_resolution_clock::now();
            crear_arbol(input_file_path, output_file_path);
            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            // Obtener tamaño del archivo comprimido
            auto file_size = filesystem::file_size(output_file_path);
            timefile << input_file_path << ";" << "iteracion" << (i + 1) << ";" << duration << ";" << "milisegundos" << ";" << file_size << ";" << "bytes" << endl;
        }
    }
    else if (mode == "decodifcar")
    {
        ofstream timefile("resultados_tiempos_decodificacionhuffman.csv",ios::app);
        for (int i = 0; i < 2; ++i) {
            auto start = chrono::high_resolution_clock::now();
            decodificar(input_file_path, output_file_path);
            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start).count();
            timefile << input_file_path << ";" << "iteracion" << (i + 1) << ";" << duration << ";" << "milisegundos" << endl;
        }
    }
    else
    {
        cerr << "Modo desconocido: " << mode << endl;
        return 1;
    }
    return 0;
}
