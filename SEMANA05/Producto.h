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
    int idCategoria; // Nuevo campo para la categoría del producto

public:
    Producto(int id, string nombre, double precio, int idCategoria) {
        this->id = id;
        this->nombre = nombre;
        this->precio = precio;
        this->idCategoria = idCategoria;
    }

    void imprimir() const {
        cout << "ID: " << id 
             << " | Nombre: " << nombre 
             << " | Precio: Q" << precio 
             << " | Categoría: " << idCategoria << endl;
    }

    double getPrecio() const {
        return precio;
    }

    int getIdCategoria() const {
        return idCategoria;
    }

    string getNombre() const {
        return nombre;
    }

    int getId() const {
        return id;
    }
};


#endif // Fin de la definición de la clase Producto