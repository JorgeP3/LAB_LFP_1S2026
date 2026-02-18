#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <iostream>
#include <string>

using namespace std;

class Producto {
private:
    int id;
    string nombre;
    double precio;

public:
    Producto(int id, string nombre, double precio) {
        this->id = id;
        this->nombre = nombre;
        this->precio = precio;
    }

    void imprimir() const {
        cout << "ID: " << id 
             << " | Nombre: " << nombre 
             << " | Precio: Q" << precio << endl;
    }

    double getPrecio() const {
        return precio;
    }
};


#endif