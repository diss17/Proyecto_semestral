#ifndef NODE_H
#define NODE_H

#include <iostream>

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    // Declaración del constructor
    Node(char c, int f);
};

// Comparador personalizado para la cola de prioridad
struct Compare 
{
    bool operator()(Node* left, Node* right) 
    {
        return left->freq > right->freq;
    }
};

#endif