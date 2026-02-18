#ifndef PRODUCTO_H // Evita la inclusión múltiple del archivo de encabezado
#define PRODUCTO_H // Definición de la clase Producto

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


#endif // Fin de la definición de la clase Producto