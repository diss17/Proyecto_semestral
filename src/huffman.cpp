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

struct node
{
    char simbolo;
    int frecuencia;
    node *izq, *der;
};

node *get_node(char simbolo, int frecuencia, node *izq, node *der)
{
    node *nodo = new node();
    nodo->simbolo = simbolo;
    nodo->frecuencia = frecuencia;
    nodo->izq = izq;
    nodo->der = der;

    return nodo;
}

struct comparacion
{
    bool operator()(node *i, node *d)
    {
        return i->frecuencia > d->frecuencia;
    }
};

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

    string str = "";
    for (char simbolo : text)
    {
        str += CodigoHuffman[simbolo];
    }

    ofstream output_file(output_file_path, ios::binary);
    if (!output_file.is_open())
    {
        cerr << "No se puede abrir el archivo de salida!" << endl;
        return;
    }

    size_t map_size = CodigoHuffman.size();
    output_file.write(reinterpret_cast<const char*>(&map_size), sizeof(map_size));

    for (auto pair : CodigoHuffman)
    {
        output_file.write(&pair.first, sizeof(char));
        size_t codigo_length = pair.second.size();
        output_file.write(reinterpret_cast<const char*>(&codigo_length), sizeof(codigo_length));
        output_file.write(pair.second.c_str(), codigo_length);
    }

    string byte_str;
    for (size_t i = 0; i < str.size(); i += 8)
    {
        bitset<8> bits(str.substr(i, 8));
        byte_str += char(bits.to_ulong());
    }

    output_file.write(byte_str.c_str(), byte_str.size());
    output_file.close();
}

void decodificar(const string &input_file_path, const string &output_file_path)
{
    ifstream input_file(input_file_path, ios::binary);
    if (!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo de entrada!" << endl;
        return;
    }

    unordered_map<string, char> CodigoHuffman;
    size_t map_size;
    input_file.read(reinterpret_cast<char*>(&map_size), sizeof(map_size));

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

    string codificado;
    char byte;
    while (input_file.read(&byte, sizeof(char)))
    {
        bitset<8> bits(byte);
        codificado += bits.to_string();
    }
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
