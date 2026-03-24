#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

class Token {
public:
    int numero;
    string tipo;
    string lexema;
    int linea;
    int columna;

    Token(string tipo, string lexema, int linea, int columna, int numero) {
        this->tipo = tipo;
        this->lexema = lexema;
        this->linea = linea;
        this->columna = columna;
        this->numero = numero;
    }
};

#endif