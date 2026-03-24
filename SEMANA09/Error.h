#ifndef ERROR_H
#define ERROR_H

#include <string>
using namespace std;

class Error {
public:
    string lexema;
    string caracter;
    string tipo;
    int linea;
    int columna;

    Error(string lexema, string caracter, string tipo, int linea, int columna) {
        this->lexema = lexema;
        this->caracter = caracter;
        this->tipo = tipo;
        this->linea = linea;
        this->columna = columna;
    }
};

#endif