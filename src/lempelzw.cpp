#include <bits/stdc++.h>
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




vector<int> compresion(string s1)
{
    cout << "Codificando\n";
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
    cout << "String\tOutput_Code\tAddition\n";
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
            cout << p << "\t" << table[p] << "\t\t"
                 << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    cout << p << "\t" << table[p] << endl;
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
void descompresion(vector<int> op)
{
    cout << "\nDecoding\n";
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
    cout << s;
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
        cout << s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
}


int main()
{
    string text = "tangananica-tanganana";
    vector<int> output_code = compresion(text);
    cout << "Output Codes are: ";
    for (int i = 0; i < output_code.size(); i++)
    {
        cout << output_code[i] << " ";
    }
    cout << endl;
    descompresion(output_code);
}
