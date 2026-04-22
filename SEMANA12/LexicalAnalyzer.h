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
    string text;
    vector<Token> tokens;
    vector<Error> errors;

    bool esLetra(char c) { return isalpha(c) || c == '_'; }
    bool esDigito(char c) { return isdigit(c); }
    bool esComilla(char c) { return c == '"'; }
    bool esOtro(char c) { return c != '"' && c != '\n'; }
    bool esSimbolo(char c) {
        return c == ':' || c == '{' || c == '}' ||
               c == ';' || c == ',' || c == '[' || c == ']';
    }

    /*
        clasificar: recibe el lexema acumulado en S1 y determina
        si es una palabra reservada del lenguaje TaskScript o un
        identificador generico.

        Se llama justo antes de crear el token, cuando S1 acepta.
    */
    string clasificar(string lexema) {
        if (lexema == "TABLERO")      return "TABLERO";
        if (lexema == "COLUMNA")      return "COLUMNA";
        if (lexema == "tarea")        return "TAREA";
        if (lexema == "prioridad")    return "PRIORIDAD";
        if (lexema == "responsable")  return "RESPONSABLE";
        if (lexema == "fecha_limite") return "FECHA_LIMITE";
        if (lexema == "ALTA")         return "ALTA";
        if (lexema == "MEDIA")        return "MEDIA";
        if (lexema == "BAJA")         return "BAJA";
        return "IDENTIFICADOR";
    }

    string clasificarSimbolo(char c) {
    if (c == '{') return "LLAVE_ABRE";
    if (c == '}') return "LLAVE_CIERRA";
    if (c == '[') return "CORCHETE_ABRE";
    if (c == ']') return "CORCHETE_CIERRA";
    if (c == ':') return "DOS_PUNTOS";
    if (c == ',') return "COMA";
    if (c == ';') return "PUNTO_COMA";
    return "SIMBOLO";
    }

public:
    LexicalAnalyzer(string text) {
        this->text = text;
    }

    void analizar() {
        int estado = 0;
        int linea = 1;
        int columna = 1;
        int indice = 0;

        string lexema = "";

        /*
            Estados del AFD:
            S0  - inicial
            S1  - identificador / palabra reservada  (L.(L|D)*)
            S2  - entero                             (D.D*)
            S3  - dentro de cadena                  (Q.O*)
            S4  - cadena cerrada, aceptacion
            S5  - fecha: leidos 4 digitos            (DDDD)
            S6  - fecha: leido primer guion          (DDDD-)
            S7  - fecha: leidos 2 digitos de mes     (DDDD-DD)
            S8  - fecha: leido segundo guion         (DDDD-DD-)
            S9  - fecha: aceptacion                  (DDDD-DD-DD)
        */

        for (int i = 0; i < (int)text.size(); i++) {
            char c = text[i];

            switch (estado) {

                // ── S0: estado inicial ─────────────────────────────────────
                case 0:
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
                        lexema += c;
                        tokens.push_back(Token(clasificarSimbolo(c), lexema, linea, columna, indice++));
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

                // ── S1: acumulando letras/digitos (identificador) ──────────
                case 1:
                    if (esLetra(c) || esDigito(c)) {
                        lexema += c;
                    }
                    else {
                        /*
                            Aceptacion en S1.
                            clasificar() decide si es palabra reservada o IDENTIFICADOR.
                        */
                        tokens.push_back(Token(clasificar(lexema), lexema, linea, columna - lexema.length(), indice++));
                        lexema = "";
                        estado = 0;
                        i--; // reprocesar caracter actual desde S0
                    }
                    break;

                // ── S2: acumulando digitos ─────────────────────────────────
                case 2:
                    if (esDigito(c)) {
                        lexema += c;
                    }
                    else if (c == '-' && lexema.length() == 4) {
                        /*
                            Transicion especial: si llevamos exactamente 4 digitos
                            y el siguiente caracter es '-', puede ser una fecha.
                            Pasamos a S5 para validarlo.
                        */
                        lexema += c;
                        estado = 5;
                    }
                    else {
                        // Aceptacion en S2: es un ENTERO normal
                        tokens.push_back(Token("ENTERO", lexema, linea, columna - lexema.length(), indice++));
                        lexema = "";
                        estado = 0;
                        i--;
                    }
                    break;

                // ── S3: dentro de cadena ───────────────────────────────────
                case 3:
                    if (esOtro(c)) {
                        lexema += c;
                    }
                    else if (esComilla(c)) {
                        lexema += c;
                        estado = 4;
                    }
                    else {
                        // Salto de linea dentro de cadena: error
                        errors.push_back(Error(lexema, string(1, c), "Léxico", linea, columna));
                        lexema = "";
                        estado = 0;
                    }
                    break;

                // ── S4: cadena cerrada, aceptacion ────────────────────────
                case 4:
                    tokens.push_back(Token("CADENA", lexema, linea, columna - lexema.length() + 1, indice++));
                    lexema = "";
                    estado = 0;
                    i--; // reprocesar caracter actual desde S0
                    break;

                // ── S5: leidos "DDDD-", esperamos 2 digitos de mes ────────
                case 5:
                    if (esDigito(c)) {
                        lexema += c;
                        // Cuando acumulamos los 2 digitos del mes pasamos a S6
                        if (lexema.length() == 7) { // 4 digitos + '-' + 2 digitos
                            estado = 6;
                        }
                    }
                    else {
                        // No era fecha, lo que teniamos era un ENTERO + caracter suelto
                        string anio = lexema.substr(0, 4);
                        tokens.push_back(Token("ENTERO", anio, linea, columna - lexema.length(), indice++));
                        lexema = "";
                        estado = 0;
                        i--; // reprocesar
                    }
                    break;

                // ── S6: leidos "DDDD-DD", esperamos el segundo '-' ────────
                case 6:
                    if (c == '-') {
                        lexema += c;
                        estado = 7;
                    }
                    else {
                        // Formato invalido
                        errors.push_back(Error(lexema, string(1, c), "Léxico", linea, columna));
                        lexema = "";
                        estado = 0;
                    }
                    break;

                // ── S7: leidos "DDDD-DD-", esperamos 2 digitos de dia ─────
                case 7:
                    if (esDigito(c)) {
                        lexema += c;
                        // Cuando acumulamos los 2 digitos del dia: aceptacion
                        if (lexema.length() == 10) { // DDDD-DD-DD
                            estado = 8;
                        }
                    }
                    else {
                        errors.push_back(Error(lexema, string(1, c), "Léxico", linea, columna));
                        lexema = "";
                        estado = 0;
                    }
                    break;

                // ── S8: aceptacion de FECHA ───────────────────────────────
                case 8:
                    tokens.push_back(Token("FECHA", lexema, linea, columna - lexema.length(), indice++));
                    lexema = "";
                    estado = 0;
                    i--; // reprocesar caracter actual desde S0
                    break;
            }

            // Control de linea y columna
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

        // Manejo de fin de entrada
        if (!lexema.empty()) {
            if (estado == 1) {
                tokens.push_back(Token(clasificar(lexema), lexema, linea, columna - lexema.length(), indice++));
            }
            else if (estado == 2) {
                tokens.push_back(Token("ENTERO", lexema, linea, columna - lexema.length(), indice++));
            }
            else if (estado == 8) {
                tokens.push_back(Token("FECHA", lexema, linea, columna - lexema.length(), indice++));
            }
            else {
                errors.push_back(Error(lexema, "", "Léxico", linea, columna));
            }
        }
    }

    vector<Token> getTokens() { return tokens; }
    vector<Error> getErrors() { return errors; }
};

#endif