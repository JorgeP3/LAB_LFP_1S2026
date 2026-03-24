#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>
#include <cctype>
#include "Token.h"
#include "Error.h"

using namespace std;

class LexicalAnalyzer {

private:
    string text; // Texto de entrada para el analizador léxico
    vector<Token> tokens; // Vector para almacenar los tokens generados
    vector<Error> errors;

     // Funciones auxiliares para clasificar caracteres según el autómata
    bool esLetra(char c) { // Representa L en la ER
        return isalpha(c) || c == '_'; // Permitir guiones bajos en identificadores/palabras reservadas
    }

    bool esDigito(char c) {
        return isdigit(c);
    }

    bool esComilla(char c) {
        return c == '"';
    }

    bool esOtro(char c) {
        // Representa O en la ER (cualquier carácter válido dentro de cadena)
        return c != '"' && c != '\n';
    }

    bool esSimbolo(char c) {
        // Representa C en la ER
        return c == ':' || c == '{' || c == '}' ||
               c == ';' || c == ',' || c == '[' || c == ']';
    }

public:
    LexicalAnalyzer(string text) {
        this->text = text;
    }

    void analizar() {
        int estado = 0;   // S0
        int linea = 1;
        int columna = 1;
        int indice = 0;

        string lexema = "";// Para construir el lexema actual

        for (int i = 0; i < text.size(); i++){
            char c = text[i]; //caracter actual

            switch (estado){

                /*
                S0: Estado inicial

                Transiciones:
                L -> S1
                D -> S2
                Q -> S3
                C -> S4 (aceptación directa)
                */
                case 0 :
                    if (esLetra(c)) {
                        estado = 1;
                        lexema += c;
                    }
                    else if (esDigito(c)) {
                        estado = 2;
                        lexema += c;
                    }
                    else if (esComilla(c)) {
                        estado = 3;
                        lexema += c;
                    }
                    else if (esSimbolo(c)) {
                        // C es aceptación inmediata (S4)
                        lexema += c;
                        tokens.push_back(Token("SIMBOLO", lexema, linea, columna, indice++));
                        lexema = "";
                        estado = 0;
                    }
                    else if (c == ' ' || c == '\n' || c == '\t') {
                        // ignorar espacios
                    }
                    else {
                        errors.push_back(Error(string(1, c), string(1, c), "Léxico", linea, columna));
                    }
                    break;
                
                /*
                S1: L.(L|D)*

                Estado de aceptación (identificadores)
                */
                case 1:
                    if (esLetra(c)) {
                        lexema += c;
                    }
                    else {
                        // Aceptación en S1
                        tokens.push_back(Token("PALABRA_RESERVADA", lexema, linea, columna - lexema.length(), indice++));

                        lexema = "";
                        estado = 0;

                        // Reprocesar carácter actual desde S0
                        i--;
                    }
                    break;
                
                /*
                S2: D.D*

                Estado de aceptación (números)
                */
                case 2:
                    if (esDigito(c)) {
                        lexema += c;
                    }
                    else {
                        // Aceptación en S2
                        tokens.push_back(Token("ENTERO", lexema, linea, columna - lexema.length(), indice++));

                        lexema = "";
                        estado = 0;

                        i--;
                    }
                    break;

                /*
                    S3: Q.O*.Q

                    Estado dentro de cadena
                */
                case 3:
                    if (esOtro(c)) {
                        lexema += c;
                    }
                    else if (esComilla(c)) {
                        lexema += c;
                        estado = 4; // transición a estado final de cadena
                    }
                    else {
                        // Error: salto de línea dentro de cadena
                        errors.push_back(Error(lexema, string(1, c), "Léxico", linea, columna));
                        lexema = "";
                        estado = 0;
                    }
                    break;

                /*
                    S4: estado final de cadena

                    Aceptación explícita (como en el autómata)
                */
                case 4:
                    tokens.push_back(Token("CADENA", lexema, linea, columna - lexema.length() + 1, indice++));

                    lexema = "";
                    estado = 0;

                    // Reprocesar carácter actual (ya que este estado no consume nuevo símbolo)
                    i--;
                    break; 
            }

             /*
                Control de línea y columna
            */
            if (c == '\n') {
                linea++;
                columna = 1;
            }
            else if (c == '\t') {
                columna += 4;
            }
            else {
                columna++;
            }

        }

        /*
            Manejo de fin de entrada

            Si el análisis termina en un estado de aceptación,
            se debe generar el token correspondiente.
        */
        if (!lexema.empty()) {
            if (estado == 1) {
                tokens.push_back(Token("PALABRA_RESERVADA", lexema, linea, columna - lexema.length(), indice++));
            }
            else if (estado == 2) {
                tokens.push_back(Token("ENTERO", lexema, linea, columna - lexema.length(), indice++));
            }
            else {
                // Cualquier otro caso es error (cadena no cerrada, etc.)
                errors.push_back(Error(lexema, "", "Léxico", linea, columna));
            }
        }

    }

    vector<Token> getTokens() {
        return tokens;
    }

    vector<Error> getErrors() {
        return errors;
    }

};

#endif