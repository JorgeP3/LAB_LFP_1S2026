#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//mandamos a llamar la clase, tiene que estar en el mismo directorio que el main.cpp
//si no se tiene que ajustar la ruta
#include "Producto.h"

using namespace std;

// FUNCION PARA SEPARAR LOS CAMPOS DE UNA LINEA DE TEXTO (SPLIT)

vector<string> split(string linea, char delimitador){ //(linea, ",")
    vector <string> partes;
    string actual = ""; //25.50

    for (char c : linea) {
        if (c == delimitador) {
            partes.push_back(actual);
            actual = "";
        } else {
            actual += c;
        }
    }

    // Agregar la ultima parte
    // como la ultima parte no termina con el delimitador, se agrega despues del ciclo
    partes.push_back(actual);
    return partes;
}


int main() {

    vector<Producto> productos; // vector para almacenar los objetos Producto
    int opcion;

    

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Cargar archivo\n";
        cout << "2. Mostrar productos\n";
        cout << "3. Mostrar producto mas caro\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {

        case 1: {
            productos.clear();

            // Ruta relativa (mismo directorio del .exe)
            string ruta = "productos.txt";

            // Ruta por si quieren colocar el archivo en el escritorio (ajusten la ruta a su usuario)
            // string ruta = "C:\\Users\\TuUsuario\\Desktop\\productos.txt";

            ifstream archivo(ruta);

            if (!archivo.is_open()) {
                cout << "No se pudo abrir el archivo.\n";
                break;
            }

            string linea;

            while (getline(archivo, linea)) {//leer el archivo linea por linea

                vector<string> partes = split(linea, ',');//separamos la linea en partes usando la funcion split, el delimitador es la coma (,)
                //se iguala a 3 porque cada linea del archivo tiene 3 campos (id, nombre, precio)
                // si el archivo tiene 4 campos, se iguala a 4, etc
                if (partes.size() == 3) {
                    int id = stoi(partes[0]);//stoi para convertir la cadena a entero
                    string nombre = partes[1];//el nombre es una cadena, no necesita conversion
                    double precio = stod(partes[2]);//stod para convertir la cadena a double

                    Producto p(id, nombre, precio);//crear el objeto Producto con los datos del archivo
                    productos.push_back(p);//agregar el objeto al vector de productos
                }
            }

            archivo.close();
            cout << "Archivo cargado correctamente.\n";
            break;
        }

        case 2:
            if (productos.empty()) {
                cout << "No hay productos cargados.\n";
            } else {
                for (const Producto& p : productos) {
                    p.imprimir();//imprimir los datos del producto usando el metodo imprimir de la clase Producto
                }
            }
            break;

        case 3:
            if (productos.empty()) {
                cout << "No hay productos cargados.\n";
            } else {
                Producto masCaro = productos[0];//inicializamos el producto mas caro con el primer producto del vector

                for (const Producto& p : productos) {//iteramos sobre el vector de productos para encontrar el producto mas caro
                    if (p.getPrecio() > masCaro.getPrecio()) {//si el precio del producto actual es mayor que el precio del producto mas caro, se actualiza el producto mas caro
                        masCaro = p;
                    }
                }

                cout << "\nProducto mas caro:\n";
                masCaro.imprimir();
            }
            break;

        case 4:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opcion invalida.\n";
        }

    } while (opcion != 4);

    return 0;
}