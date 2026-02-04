#include <iostream>
#include <string>

using namespace std; // importante para no usar el std:: en cada línea

int sumar(int a, int b) {
    return a + b;
}

int main() {
    // comentario de una sola línea
    /*
        comentario
        de 
        múltiples
        líneas
    */

    cout << "==== Practica: fundamentos de c++ === \n" << endl;
    // 1. TIPOS DE DATOS
   {
        int edad=20;
        long long poblacion = 7800000000;
        float pi = 3.14f;
        double promedio = 85.75;
        char inicial = 'L';
        bool esEstudiante = false;
        string nombre = "Jorge";

        cout << "--- Tipos y variables ---\n";
        cout << "edad: " << edad << "\n";
        cout << "poblacion: " << poblacion << "\n";
        cout << "pi: " << pi << "\n";
        cout << "promedio: " << promedio << "\n";
        cout << "inicial: " << inicial << "\n";
        cout << "nombre: " << nombre << "\n";
        cout << "esEstudiante: " << esEstudiante << "\n";
   }

   // 2. ENTRADA Y SALIDA

    /*{
        cout << "\n--- Entrada y salida ---\n";
        string userName;
        int edad;

        cout << "Ingrese su nombre: ";
        cin >> userName; // cin se utiliza para la entrada de datos por consola

        cout << "Ingrese su edad: ";
        cin >> edad;

        cout << "usuario: " << userName << ", edad: " << edad << "\n";
    }*/

    // 3. CONDICIONALES (IF /ELSE IF / ELSE)
    /*{
        cout << "\n--- Condicionales ---\n";
        
        int numero;
        cout << "Ingrese un numero entero: ";
        cin >> numero;

        if (numero > 0) {
            cout << "El numero es positivo.\n";
        } else if (numero < 0) {
            cout << "El numero es negativo.\n";
        } else {
            cout << "El numero es cero.\n";
        }
    }*/

    //4. SWITCH 
    /*{
        cout << " \n--- Estructura Switch ---\n";

        string userName="Alan";
        int edad=21;

        int opcion;

        cout << "Menu de opciones:\n";
        cout << "1. Saludar\n";
        cout << "2. Mostrar edad\n";
        cout << "3. Despedirse\n";
        cout << "Seleccione una opcion (1-3): ";
        cin >> opcion; 
        
        switch (opcion)
        {
            case 1:
                cout << "Hola, " << userName << "!\n";
                break;// break para salir del switch
            case 2:
                cout << "Tu edad es: " << edad << "\n";
                break;
            case 3:
                cout << "Adios!\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
    }*/

    // 5. CICLOS 
    /*{
        cout << "--- Ciclo for ---\n";
        for (int i = 1; i <= 5; i++) {
            cout << "Iteracion " << i << "\n";
        }

        cout << "\n--- Ciclo while ---\n";

        int n;

        cout << "Ingrese n: ";
        cin >> n;

        int contador = 1;
        while (contador <= n) {
            cout << "Contador: " << contador << "\n";
            contador++;
        }
    }*/

    // 6. FUNCIONES
    {
        cout << "\n--- Funciones ---\n";

        int a, b;
        cout << "Ingrese a: ";
        cin >> a;
        cout << "Ingrese b: ";
        cin >> b;

        int resultado = sumar(a, b);
        cout << "La suma de " << a << " y " << b << " es: " << resultado << "\n";
    }



    return 0;
}