#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono> // Biblioteca para medir el tiempo
#include <string>
using namespace std;
/*
compresion: Esta función toma una cadena s1 como entrada y la comprime utilizando el algoritmo LempelZW.
Donde se inicializa table (un diccionario (unordered_map) que mapea cadenas a enteros) con todos los caracteres ASCII estándar (0-255) asignados a sus respectivos códigos.
Se inicia con p (string) como el primer carácter de s1 y se itera a través de la cadena.
Se construye la cadena p + c (donde c es el próximo carácter) y se verifica si está presente en table.
Si está presente, se expande p agregando c y se continúa.
Si no está presente, se agrega el código de p a output_code (vector de enteros), 
se agrega p + c a table con un nuevo código, y se actualiza p a c.

Luego se imprime la tabla de compresión paso a paso y devuelve output_code, 
que contiene los códigos de salida comprimidos.
*/

vector<int> comprimir(string s1)
{
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;

    for (int i = 0; i < s1.length(); i++)
    {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end())
        {
            p = p + c;
        }
        else
        {
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    output_code.push_back(table[p]);
    return output_code;
}
/*
Funcion descomprimir:
Esta funcion descomprime una secuencia de códigos op utilizando el algoritmo LempelZW.
Se inicializa table (Un diccionario (unordered_map) que mapea enteros a cadenas) con todos los caracteres ASCII estándar (0-255) asignados a sus respectivas cadenas.
Se comienza con el primer código de op, old (entero que representa codigo), y se recupera la cadena correspondiente s.
Se itera a través de op, y para cada código n, se verifica si está presente en table.
Si no está presente, se construye la cadena s + c, donde c es el primer carácter de s, y se agrega a table.
Se imprime la cadena s y se actualizan las variables c, count y old.
Luego se imprime la cadena descomprimida paso a paso.
*/
string descomprimir(vector<int> op)
{
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }

    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    string result = s;
    int count = 256;

    for (int i = 0; i < op.size() - 1; i++)
    {
        n = op[i + 1];
        if (table.find(n) == table.end())
        {
            s = table[old];
            s = s + c;
        }
        else
        {
            s = table[n];
        }
        result += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Uso: " << argv[0] << " <comprimir/descomprimir> <archivo_entrada> <archivo_salida>\n";
        return 1;
    }

    string modo = argv[1];
    string archivo_entrada = argv[2];
    string archivo_salida = argv[3];

    ifstream infile(archivo_entrada);
    if (!infile)
    {
        cerr << "Error al abrir el archivo de entrada.\n";
        return 1;
    }

    stringstream buffer;
    buffer << infile.rdbuf();
    string texto = buffer.str();
    infile.close();

    vector<int> codigos;

    if (modo == "comprimir")
    {
        
        
        ofstream outfile(archivo_salida);
        if (!outfile)
        {
            cerr << "Error al abrir el archivo de salida.\n";
            return 1;
        }
        // Archivo para guardar los tiempos de compresión
        ofstream timefile("resultados_tiempos_compresion.csv");
        if (!timefile)
        {
            cerr << "Error al abrir el archivo de resultados de tiempos.\n";
            return 1;
        }
        for (int i = 0; i < 20; ++i){
            auto start_time = chrono::high_resolution_clock::now(); // Medir tiempo inicio
            codigos = comprimir(texto);
            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            timefile << "Iteración " << (i + 1) << ": " << duration << " milisegundos" << endl;

            // Limpieza de la memoria de los códigos
            codigos.clear();
        }
        timefile.close();

        for (int codigo : codigos)
        {
            outfile << codigo << " ";
        }
        outfile.close();

        cout << "Archivo comprimido exitosamente.\n";
    }

    else if (modo == "descomprimir")
    {
        vector<int> codigos_descomprimir;
        int codigo;
        ifstream infile2(archivo_entrada);
        while (infile2 >> codigo)
        {
            codigos_descomprimir.push_back(codigo);
        }
        infile2.close();

      

        ofstream timefile("resultados_tiempos_descompresion.csv");
        if (!timefile)
        {
            cerr << "Error al abrir el archivo de resultados de tiempos.\n";
            return 1;
        }
        ofstream outfile(archivo_salida);
        if (!outfile)
        {
            cerr << "Error al abrir el archivo de salida.\n";
            return 1;
        }

        for (int i = 0; i < 20; ++i)
        {
            auto start_time = chrono::high_resolution_clock::now(); // Medir tiempo inicio

            string texto_descomprimido = descomprimir(codigos_descomprimir);

            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            timefile << "Iteración " << (i + 1) << ": " << duration << " milisegundos" << endl;

            // Limpieza de la memoria del texto descomprimido
            texto_descomprimido.clear();
        }
         // Realizar la descompresión una vez más y escribir en el archivo de salida
        string texto_descomprimido_final = descomprimir(codigos_descomprimir);
        ofstream outfile(archivo_salida);
        if (!outfile)
        {
            cerr << "Error al abrir el archivo de salida.\n";
            return 1;
        }

        timefile.close();
        outfile << texto_descomprimido_final;
        outfile.close();

        cout << "Archivo descomprimido exitosamente.\n";
    }
    else
    {
        cout << "Modo no válido. Debe ser 'comprimir' o 'descomprimir'.\n";
        return 1;
    }

    return 0;
}

