#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <string>
using namespace std;

/**
 * @brief Comprime una cadena utilizando el algoritmo de compresión LZW.
 *
 * Esta función implementa el algoritmo de compresión LZW (Lempel-Ziv-Welch). Inicialmente, la tabla de códigos contiene
 * las entradas para todos los caracteres ASCII (0-255). La función construye progresivamente nuevas entradas en la tabla
 * para subsecuencias de caracteres encontradas en la cadena de entrada y devuelve una secuencia de códigos de salida.
 *
 * @param s1 La cadena de entrada que se va a comprimir.
 * @return vector<int> Un vector de enteros que representa la cadena de entrada comprimida.
 */
vector<int> comprimir(string s1)
{
    // Tabla de códigos para almacenar las subsecuencias y sus respectivos códigos
    unordered_map<string, int> table;
    
    // Inicializa la tabla con los caracteres ASCII
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    // Variables para almacenar la subsecuencia actual y la siguiente
    string p = "", c = "";
    p += s1[0]; // Inicializa 'p' con el primer carácter de la cadena de entrada
    int code = 256; // Código inicial para nuevas entradas en la tabla (códigos 0-255 ya están usados)
    vector<int> output_code; // Vector para almacenar los códigos de salida

    // Recorre la cadena de entrada carácter por carácter
    for (int i = 0; i < s1.length(); i++)
    {
        if (i != s1.length() - 1)
            c += s1[i + 1]; // 'c' obtiene el siguiente carácter si no es el final de la cadena

        // Si la combinación de 'p' y 'c' ya existe en la tabla
        if (table.find(p + c) != table.end())
        {
            p = p + c; // Extiende 'p' para incluir 'c'
        }
        else
        {
            // Si 'p + c' no está en la tabla, añade el código de 'p' a la salida
            output_code.push_back(table[p]);
            // Añade 'p + c' a la tabla con un nuevo código
            table[p + c] = code;
            code++;
            // 'p' ahora se convierte en 'c'
            p = c;
        }
        // Resetea 'c' para el siguiente carácter
        c = "";
    }
    // Añade el código de la última subsecuencia a la salida
    output_code.push_back(table[p]);
    return output_code; // Devuelve el vector de códigos de salida
}

/**
 * @brief Descomprime una secuencia de códigos utilizando el algoritmo LZW.
 *
 * Esta función implementa el algoritmo de descompresión LZW (Lempel-Ziv-Welch). Inicialmente, la tabla de códigos contiene
 * las entradas para todos los caracteres ASCII (0-255). La función reconstruye progresivamente las subsecuencias de caracteres
 * a partir de la secuencia de códigos de entrada y devuelve la cadena descomprimida.
 *
 * @param op Un vector de enteros que representa la secuencia de códigos comprimidos.
 * @return string La cadena de texto descomprimida.
 */
string descomprimir(vector<int> op)
{
    // Tabla de códigos para almacenar los códigos y sus respectivas subsecuencias
    unordered_map<int, string> table;
    
    // Inicializa la tabla con los caracteres ASCII
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }

    int old = op[0], n; // Código anterior y el código actual
    string s = table[old]; // Cadena correspondiente al código anterior
    string c = ""; // Primer carácter de la cadena actual
    c += s[0];
    string result = s; // Resultado de la descompresión
    int count = 256; // Código inicial para nuevas entradas en la tabla (códigos 0-255 ya están usados)

    // Recorre la secuencia de códigos de entrada
    for (int i = 1; i < op.size(); i++)
    {
        n = op[i]; // Código actual
        // Si el código no está en la tabla, construye la cadena manualmente
        if (table.find(n) == table.end())
        {
            s = table[old];
            s = s + c;
        }
        else
        {
            s = table[n]; // Recupera la cadena correspondiente al código actual
        }
        result += s; // Añade la cadena al resultado
        c = "";
        c += s[0]; // Actualiza 'c' con el primer carácter de la cadena actual
        // Añade la nueva entrada a la tabla
        table[count] = table[old] + c;
        count++;
        old = n; // Actualiza el código anterior
    }

    return result; // Devuelve la cadena descomprimida
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

    ifstream infile(archivo_entrada, ios::binary);
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
        ofstream outfile(archivo_salida, ios::binary);
        if (!outfile)
        {
            cerr << "Error al abrir el archivo de salida.\n";
            return 1;
        }

        ofstream timefile("resultados_tiempos_compresion.csv", ios::app);
        if (!timefile)
        {
            cerr << "Error al abrir el archivo de resultados de tiempos.\n";
            return 1;
        }

        for (int i = 0; i < 5; i++)
        {
            auto start_time = chrono::high_resolution_clock::now(); // Medir tiempo inicio
            codigos = comprimir(texto);
            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

            // Calcular el tamaño del archivo comprimido en bytes
            size_t compressed_size = codigos.size() * sizeof(codigos[0]);

            // Escribir información en el archivo CSV
            timefile << archivo_entrada << ";" << "iteracion" << (i + 1) << ";" << duration << ";" << "milisegundos" << ";" << compressed_size << " bytes" << endl;
        }

        for (size_t i = 0; i < codigos.size(); ++i)
        {
            outfile.write(reinterpret_cast<char *>(&codigos[i]), sizeof(codigos[i]));
        }

        outfile.close();
        timefile.close();

        cout << "Archivo comprimido exitosamente.\n";
    }

    else if (modo == "descomprimir")
    {
        vector<int> codigos_descomprimir;
        int codigo;
        ifstream infile2(archivo_entrada, ios::binary);
        while (infile2.read(reinterpret_cast<char *>(&codigo), sizeof(codigo)))
        {
            codigos_descomprimir.push_back(codigo);
        }
        infile2.close();

        ofstream timefile("resultados_tiempos_descompresion.csv",ios::app);
        if (!timefile)
        {
            cerr << "Error al abrir el archivo de resultados de tiempos.\n";
            return 1;
        }
        ofstream outfile(archivo_salida, ios::binary);
        if (!outfile)
        {
            cerr << "Error al abrir el archivo de salida.\n";
            return 1;
        }

        for (int i = 0; i < 5; ++i)
        {
            auto start_time = chrono::high_resolution_clock::now(); // Medir tiempo inicio

            string texto_descomprimido = descomprimir(codigos_descomprimir);

            auto end_time = chrono::high_resolution_clock::now(); // Medir tiempo final
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            timefile << archivo_entrada << ";" << "iteracion" << (i + 1) << ";" << duration << ";" << "milisegundos" << endl;

            // Limpieza de la memoria del texto descomprimido
            texto_descomprimido.clear();
        }

        // Realizar la descompresión una vez más y escribir en el archivo de salida
        string texto_descomprimido_final = descomprimir(codigos_descomprimir);
        timefile.close();
        outfile.write(texto_descomprimido_final.c_str(), texto_descomprimido_final.size());
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
