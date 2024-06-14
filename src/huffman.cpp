#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

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
    cout << "\n El texto codificado es: \n"
         << str << endl;
    int index = -1;
    cout << "\n El texto decodificado es: \n";
    while (index < (int)str.size() - 1)
    {
        decodificar(raiz, index, str);
    }
    cout << "\n\n\n";
}
int main(int argc, char const *argv[])
{
    string text = "tangananica-tanganana";

    crear_arbol(text);
    cout << "\n";
    return 0;
}
