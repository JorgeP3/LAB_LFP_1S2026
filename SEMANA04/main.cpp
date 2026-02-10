#include <iostream>
#include <string>

//estructuras
#include <vector>
#include <map>
//Lectura escritura de archivos
#include <fstream>

using namespace std;

int main(){

    // 1. ARREGLO UNICIMENSIONAL
    int numeros[5] = {1, 2, 3, 4, 5};

    //acceder a un elemento
    cout << "\nAcceso a un elemento:" << endl;
    cout << "Elemento en la posicion 2: " << numeros[2] << "\n"; // Imprime 3

    //modificar un elemento
    numeros[2] = 10;
    cout << "Elemento en la posicion 2 (modificado): " << numeros[2] << "\n";

    // 2. VECTOR (ARREGLO DINAMICO)

    int matriz[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    //acceder a un elemento
    cout << "\nAcceso a un elemento en matriz:" << endl;
    cout << "Elemento en la fila 1, columna 2: " << matriz[1][2] << "\n"; // Imprime 6
    //modificar un elemento
    matriz[1][2] = 20;
    cout << "Elemento en la fila 1, columna 2 (modificado): " << matriz[1][2] << "\n";

    // 3. ITERACION CON FOR 
    int datos[5] = {10, 20, 30, 40, 50};
    cout << "\nIteracion con for:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << datos[i] << "\n";
    }

    // 4. ITERACION CON FOR RANGE (C++11 en adelante)
    int valores[5] = {5, 10, 15, 20, 25};

    cout << "\nIteracion con for range:" << endl;
    for (int elemento : valores) {
        cout << elemento << "\n";
    }

    // 5. VECTORES (ARREGLOS DINAMICOS)
    vector<int> numerosVector;

    cout << "\nElementos en el vector(vacio):" << endl;
    for (int num : numerosVector) {
        cout << num << "\n";
    }

    //push_back para agregar elementos al vector
    numerosVector.push_back(100);// posicion 0
    numerosVector.push_back(200);// posicion 1
    numerosVector.push_back(300);// posicion 2
    
    cout << "\nElementos en el vector:" << endl;
    for (int num : numerosVector) {
        cout << num << "\n";
    }

    //metodo para eliminar el ultimo elemento del vector
    numerosVector.pop_back();
     cout << "\nElementos en el vector:" << endl;
    for (int num : numerosVector) {
        cout << num << "\n";
    }

    // 6. ESCRITURA DE ARCHIVO

    ofstream salida("archivo.txt");

    if (!salida.is_open()) {
        cout << "Error al crear el archivo.\n";
        return 1;
    }

    salida << "Hola, este es un archivo de texto.\n";
    salida << "Escrito desde C++.\n";

    salida.close();//cerrar el archivo despues de escribirlo
    cout << "\nArchivo creado y escrito exitosamente.\n";

    // 7. LECTURA DE ARCHIVO
    ifstream entrada("archivo.txt");
    string linea;

    if (!entrada.is_open()) {
        cout << "Error al abrir el archivo.\n";
        return 1;
    }

    cout << "\nContenido del archivo:\n";
    while (getline(entrada, linea)) {//leer el archivo linea por linea
        cout << linea << "\n";
    }

    entrada.close();//cerrar el archivo despues de leerlo
    cout << "\nArchivo leido exitosamente.\n";

    return 0;
}