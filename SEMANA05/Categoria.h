#ifndef CATEGORIA_H // Evita la inclusión múltiple del archivo de encabezado
#define CATEGORIA_H // Definición de la clase Categoria

#include <iostream>
#include <string>

using namespace std;

class Categoria {

private:
    int id;
    string nombre;

public:
    Categoria(int id, string nombre) {
        this->id = id;
        this->nombre = nombre;
    }

    void imprimir() const {
        cout << "ID: " << id << " | Nombre: " << nombre << endl;
    }

    int getId() const {
        return id;
    }

    string getNombre() const {
        return nombre;
    }
};

#endif