#include <iostream>
#include <string>

using namespace std;

//Definir la clase

class Usuario{
    //atributos
    private:
        string nombre;
        int edad; 
    //metodos
    public:
        Usuario(string nombre, int edad){
            this->nombre = nombre;
            this->edad = edad;
        }
        
        void mostrarInfo(){
            cout << "Nombre: " << nombre << "\n";
            cout << "Edad: " << edad << "\n";
        }
};

int main(){
    Usuario usuario1("Juan", 25);

    usuario1.mostrarInfo();
    
    return 0;
}