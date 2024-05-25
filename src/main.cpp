#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;
struct FreqCh
{
    char ch;
    int freq;

    FreqCh(char c, int f) : ch(c), freq(f){}
};

// Función que cuenta las veces que se repite cada caracter en string
std::unordered_map<char,int> FrCount( std::unordered_map<char,int> f, std::string t ){
    for ( auto ch : t)
    {
        f[ch]++;
    }
    
    return f;
}

int main(int argc, char const *argv[])
{  
    std::unordered_map<char,int> freq;
    string text ="tangananica-tanganana";
    freq = FrCount(freq, text);
    for (auto ch : freq)
    {
        cout<<"Caracter: "<< ch.first << " Frecuencia: "<<ch.second<<endl; 
    }
    
    

    
    return 0;
}
