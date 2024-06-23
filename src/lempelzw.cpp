#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <string>
using namespace std;

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

    for (int i = 1; i < op.size(); i++)
    {
        n = op[i];
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
