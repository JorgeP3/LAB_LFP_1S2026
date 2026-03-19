#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//mandamos a llamar la clase, tiene que estar en el mismo directorio que el main.cpp
//si no se tiene que ajustar la ruta
#include "Producto.h"
#include "Categoria.h"//mandamos a llamar la clase Categoria, tiene que estar en el mismo directorio que el main.cpp

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
    vector<Categoria> categorias; // vector para almacenar los objetos Categoria
    //vector<Categoria> cat5eg5orias132;
    int opcion;

    

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Cargar productos\n";
        cout << "2. Cargar categorias\n";
        cout << "3. Mostrar productos\n";
        cout << "4. Mostrar productos con su categoria\n";
        cout << "5. Mostrar productos por categoria\n";
        cout << "6. $ Mostrar producto mas caro\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {

        //CARGAR PRODUCTOS
        case 1: {
            productos.clear();

            // Ruta relativa (mismo directorio del .exe)
            // AJUSTA LA RUTA SEGUN TU CONFIGURACION
            string ruta = "C:\\Users\\lmpgp\\Documents\\1. Ingenieria en sistemas\\1. 1S 2026\\Auxiliatura\\1. Repositorios\\LAB_LFP_1S2026\\SEMANA05\\output\\productos.txt";

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
                if (partes.size() == 4) {
                    int id = stoi(partes[0]);//stoi para convertir la cadena a entero
                    string nombre = partes[1];//el nombre es una cadena, no necesita conversion
                    double precio = stod(partes[2]);//stod para convertir la cadena a double
                    int idCategoria = stoi(partes[3]);//stoi para convertir la cadena a entero

                    Producto p(id, nombre, precio, idCategoria);//crear el objeto Producto con los datos del archivo
                    productos.push_back(p);//agregar el objeto al vector de productos
                }
            }

            archivo.close();
            cout << "Archivo cargado correctamente.\n";
            break;
        }

        //CARGAR CATEGORIAS
        case 2: {
            categorias.clear();

            // AJUSTAR LA RUTA DEL ARCHIVO SEGUN TU CONFIGURACION
            ifstream archivo("C:\\Users\\lmpgp\\Documents\\1. Ingenieria en sistemas\\1. 1S 2026\\Auxiliatura\\1. Repositorios\\LAB_LFP_1S2026\\SEMANA05\\output\\categorias.lfp");

            if (!archivo.is_open()) {
                cout << "No se pudo abrir categorias.lfp\n";
                break;
            }

            string linea;

            while (getline(archivo, linea)) {
                vector<string> partes = split(linea, ',');

                if (partes.size() == 2) {
                    int id = stoi(partes[0]);
                    string nombre = partes[1];

                    categorias.push_back(Categoria(id, nombre));
                }
            }

            archivo.close();
            cout << "Categorias cargadas correctamente.\n";
            break;
        }

        //MOSTRAR PRODUCTOS
        case 3:
            if (productos.empty()) {
                cout << "No hay productos cargados.\n";
            } else {
                for (const Producto& p : productos) {
                    p.imprimir();//imprimir los datos del producto usando el metodo imprimir de la clase Producto
                }

                // CREA EL ARCHIVO HTML (AJUSTA LA RUTA SEGUN TU CASO)
                ofstream html("C:\\Users\\lmpgp\\Documents\\1. Ingenieria en sistemas\\1. 1S 2026\\Auxiliatura\\1. Repositorios\\LAB_LFP_1S2026\\SEMANA05\\output\\productos.html");

                if (!html.is_open()) {
                    cout << "No se pudo crear el archivo HTML.\n";
                    break;
                }

                // ESTRUCTURA BASICA DEL HTML
                html << "<!DOCTYPE html>\n";
                html << "<html>\n";
                html << "<head>\n";
                html << "<meta charset='UTF-8'>\n";
                html << "<title>Lista de Productos</title>\n";
                html << "<style>\n";
                html << "table { border-collapse: collapse; width: 60%; }\n";
                html << "th, td { border: 1px solid black; padding: 8px; text-align: center; }\n";
                html << "th { background-color: #f2f2f2; }\n";
                html << "</style>\n";
                html << "</head>\n";
                html << "<body>\n";

                html << "<h2>Lista de Productos</h2>\n";
                html << "<table>\n";
                html << "<tr>\n";
                html << "<th>ID</th>\n";
                html << "<th>Nombre</th>\n";
                html << "<th>Precio</th>\n";
                html << "</tr>\n";

                // LLENAR LA TABLA CON LOS PRODUCTOS
                for (const Producto& p : productos) {
                    html << "<tr>\n";
                    html << "<td>" << p.getId() << "</td>\n";
                    html << "<td>" << p.getNombre() << "</td>\n";
                    html << "<td>Q" << p.getPrecio() << "</td>\n";
                    html << "</tr>\n";
                }

                html << "</table>\n";
                html << "</body>\n";
                html << "</html>\n";

                html.close();

                cout << "Archivo productos.html generado correctamente.\n";
    
            }
            break;
        
        //MOSTRAR PRODUCTOS CON SU CATEGORIA
        case 4:
            if (productos.empty() || categorias.empty()) {
                cout << "No hay productos o categorias cargados cargados.\n";
            } else {
                cout << "Productos con su categoria:\n";

                for (const Producto& p : productos){
                    string nombreCategoria = "No encontrada";

                    for (const Categoria& c : categorias) {
                        if (p.getIdCategoria() == c.getId()) {
                            nombreCategoria = c.getNombre();
                            break;
                        }
                    }

                    cout << "ID: " << p.getIdCategoria() 
                         << " | Nombre: " << p.getNombre() 
                         << " | Precio: Q" << p.getPrecio() 
                         << " | Categoria: " << nombreCategoria << endl;
                }
            }
            break;

        //MOSTRAR PRODUCTOS POR CATEGORIA
        case 5:
            if (productos.empty() || categorias.empty()) {
                cout << "Debe cargar productos y categorias primero.\n";
            } else {

                cout << "\n===== PRODUCTOS AGRUPADOS POR CATEGORIA =====\n";

                for (const Categoria& c : categorias) {

                    cout << "\nCategoria: " << c.getNombre() << endl;
                    cout << "--------------------------\n";

                    bool tieneProductos = false;

                    for (const Producto& p : productos) {
                        if (p.getIdCategoria() == c.getId()) {
                            cout << "- " << p.getNombre()
                                 << " (Q" << p.getPrecio() << ")\n";
                            tieneProductos = true;
                        }
                    }

                    if (!tieneProductos) {
                        cout << "No hay productos en esta categoria.\n";
                    }
                }
            }
            break;

        case 6:
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

        case 7:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opcion invalida.\n";
        }

    } while (opcion != 7);

    return 0;
}