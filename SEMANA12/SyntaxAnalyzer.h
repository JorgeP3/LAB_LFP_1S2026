#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <string>
#include <vector>
#include <iostream>
#include "Token.h"
#include "Error.h"

using namespace std;

//Gramatica que vamos a implementar en este parser:
/*
    <programa>        ::= TABLERO CADENA LLAVE_ABRE <columnas> LLAVE_CIERRA PUNTO_COMA

    <columnas>        ::= <columna> <columnas>
                        | <columna>

    <columna>         ::= COLUMNA CADENA LLAVE_ABRE <tareas> LLAVE_CIERRA PUNTO_COMA

    <tareas>          ::= <tarea> COMA <tareas>
                        | <tarea>

    <tarea>           ::= TAREA DOS_PUNTOS CADENA CORCHETE_ABRE <atributos> CORCHETE_CIERRA

    <atributos>       ::= <atributo> COMA <atributos>
                        | <atributo>

    <atributo>        ::= PRIORIDAD    DOS_PUNTOS <valor_prioridad>
                        | RESPONSABLE  DOS_PUNTOS CADENA
                        | FECHA_LIMITE DOS_PUNTOS FECHA

    <valor_prioridad> ::= ALTA
                        | MEDIA
                        | BAJA
*/

class SyntaxAnalyzer {

private:

    vector<Token> tokens; // Lista de tokens producidad por el analizador.
    vector<Error> errors; //errores sintaxticos
    int pos; // Posición actual en la lista de tokens.

    // ─────────────────────────────────────────────────────────────────────────
    // tokenActual
    //
    // Retorna el token en la posicion actual sin avanzar el puntero.
    // Es como "ver" el siguiente token sin consumirlo todavia.
    // En la literatura esto se llama "lookahead de 1 token".
    // ─────────────────────────────────────────────────────────────────────────
    Token tokenActual() {
        return tokens[pos];
    }

    // ─────────────────────────────────────────────────────────────────────────
    // hayTokens
    //
    // Verifica que el indice pos no se haya salido del vector.
    // Siempre hay que llamar esto antes de tokenActual() para evitar
    // acceder a una posicion invalida del vector.
    // ─────────────────────────────────────────────────────────────────────────
    bool hayTokens() {
        return pos < (int)tokens.size();
    }

    // ─────────────────────────────────────────────────────────────────────────
    // sincronizar
    //
    // Se llama cuando consume() falla. Avanza el dedo token por token
    // hasta encontrar un PUNTO_COMA, que en TaskScript siempre marca
    // el fin de una estructura (columna o tablero).
    //
    // El dedo se queda apuntando AL punto y coma, no despues de el,
    // para que el parser pueda consumirlo normalmente cuando le toque.
    //
    // Efecto: un error dentro de una columna descarta esa columna completa
    // y el parser retoma desde la siguiente.
    
    void sincronizar() {
        while (hayTokens() && tokenActual().tipo != "PUNTO_COMA") {
            pos++;
        }
    }

    
    // ─────────────────────────────────────────────────────────────────────────
    // consume
    //
    // Esta es la funcion central del parser. Representa el acto de
    // "comerse" un token de la lista.
    //
    // Recibe el tipo que ESPERAMOS ver segun la gramatica.
    // Compara ese tipo con el token actual:
    //   - Si coincide  -> avanza pos (consume el token) y retorna true
    //   - Si no coincide -> registra un error sintactico y retorna false
    //
    // Cada vez que en la gramatica aparece un terminal (TABLERO, CADENA,
    // LLAVE_ABRE, etc.), en el codigo aparece una llamada a consume().
    // ─────────────────────────────────────────────────────────────────────────
    bool consume(string tipoEsperado) {
        if (!hayTokens()) {
            // El archivo termino antes de que la gramatica lo esperara
            errors.push_back(Error(
                "FIN DE ARCHIVO",
                "Se esperaba: " + tipoEsperado,
                "Sintáctico",
                -1, -1
            ));
            return false;
        }

        if (tokenActual().tipo == tipoEsperado) {
            // Token correcto: avanzamos al siguiente
            pos++;
            return true;
        }
        else {
            // Token incorrecto: guardamos el error con posicion exacta
            // para que el reporte indique exactamente donde fallo
            errors.push_back(Error(
                tokenActual().lexema,           // lo que encontramos
                "Se esperaba: " + tipoEsperado, // lo que esperabamos
                "Sintáctico",
                tokenActual().linea,
                tokenActual().columna
            ));
            sincronizar(); // avanzamos hasta el siguiente punto y coma para retomar el parseo
            return false;
        }
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parsePrograma
    //
    // Produccion raiz de la gramatica. Es la primera funcion que se llama
    // y describe la estructura completa del archivo .task.
    //
    // Gramatica:
    //   <programa> ::= TABLERO CADENA LLAVE_ABRE <columnas> LLAVE_CIERRA PUNTO_COMA
    //
    // Notar como cada terminal se traduce en consume()
    // y cada no terminal se traduce en una llamada a su funcion.
    // Esa correspondencia directa es la esencia del parser descendente recursivo.
    // ─────────────────────────────────────────────────────────────────────────
    void parsePrograma() {
        consume("TABLERO");     // palabra reservada TABLERO
        consume("CADENA");      // nombre del tablero entre comillas
        consume("LLAVE_ABRE");  // apertura del bloque {
        parseColumnas();        // una o mas columnas (no terminal)
        consume("LLAVE_CIERRA");// cierre del bloque }
        consume("PUNTO_COMA");  // ; obligatorio al final
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseColumnas
    //
    // Maneja una o mas columnas dentro del tablero.
    //
    // Gramatica:
    //   <columnas> ::= <columna> <columnas> | <columna>
    //
    // En lugar de implementar las dos producciones por separado,
    // usamos un while: parseamos la primera columna obligatoria,
    // y mientras el token actual siga siendo COLUMNA, seguimos parseando.
    // Cuando deja de ser COLUMNA, el while termina naturalmente.
    //
    // Este patron (parsear el primero fuera del while, el resto dentro)
    // es el idioma estandar para manejar "una o mas" en un parser recursivo.
    // ─────────────────────────────────────────────────────────────────────────
    void parseColumnas() {
        parseColumna(); // debe haber al menos una columna

        // mientras el token actual sea COLUMNA, hay mas columnas por parsear
        while (hayTokens() && tokenActual().tipo == "COLUMNA") {
            parseColumna();
        }
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseColumna
    //
    // Parsea una columna individual del tablero Kanban.
    //
    // Gramatica:
    //   <columna> ::= COLUMNA CADENA LLAVE_ABRE <tareas> LLAVE_CIERRA PUNTO_COMA
    //
    // Estructura identica a parsePrograma: terminales con consume(),
    // no terminales con su funcion correspondiente.
    // ─────────────────────────────────────────────────────────────────────────
    void parseColumna() {
        consume("COLUMNA");     // palabra reservada COLUMNA
        consume("CADENA");      // nombre de la columna entre comillas
        consume("LLAVE_ABRE");  // apertura del bloque {
        parseTareas();          // una o mas tareas (no terminal)
        consume("LLAVE_CIERRA");// cierre del bloque }
        consume("PUNTO_COMA");  // ; obligatorio al final
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseTareas
    //
    // Maneja una o mas tareas dentro de una columna.
    //
    // Gramatica:
    //   <tareas> ::= <tarea> COMA <tareas> | <tarea>
    //
    // Las tareas se separan por comas. Hay un caso especial:
    // la "coma trailing" — una coma despues de la ultima tarea antes del }.
    // El texto de ejemplo la tiene, asi que hay que manejarla sin error.
    //
    // La estrategia: cuando vemos una COMA, la consumimos y luego
    // verificamos si lo que sigue es TAREA. Si no lo es (es LLAVE_CIERRA),
    // era la coma trailing y salimos del while sin error.
    // ─────────────────────────────────────────────────────────────────────────
    void parseTareas() {
        parseTarea(); // debe haber al menos una tarea

        while (hayTokens() && tokenActual().tipo == "COMA") {
            consume("COMA");

            // Si despues de la coma viene una tarea, la parseamos
            // Si viene LLAVE_CIERRA, es la coma trailing: salimos
            if (hayTokens() && tokenActual().tipo == "TAREA") {
                parseTarea();
            }
            else {
                break; // coma trailing, salida normal sin error
            }
        }
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseTarea
    //
    // Parsea una tarea individual con todos sus atributos.
    //
    // Gramatica:
    //   <tarea> ::= TAREA DOS_PUNTOS CADENA CORCHETE_ABRE <atributos> CORCHETE_CIERRA
    // ─────────────────────────────────────────────────────────────────────────
    void parseTarea() {
        consume("TAREA");           // palabra reservada tarea
        consume("DOS_PUNTOS");      // :
        consume("CADENA");          // nombre de la tarea entre comillas
        consume("CORCHETE_ABRE");   // apertura de atributos [
        parseAtributos();           // lista de atributos (no terminal)
        consume("CORCHETE_CIERRA"); // cierre de atributos ]
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseAtributos
    //
    // Maneja uno o mas atributos de una tarea.
    //
    // Gramatica:
    //   <atributos> ::= <atributo> COMA <atributos> | <atributo>
    //
    // Mismo patron que parseTareas: primer atributo obligatorio fuera
    // del while, el resto dentro mientras haya COMA seguida de atributo.
    // ─────────────────────────────────────────────────────────────────────────
    void parseAtributos() {
        parseAtributo(); // debe haber al menos un atributo

        while (hayTokens() && tokenActual().tipo == "COMA") {
            consume("COMA");

            // Verificamos que lo que sigue sea uno de los tres atributos validos
            if (hayTokens() && (
                tokenActual().tipo == "PRIORIDAD"   ||
                tokenActual().tipo == "RESPONSABLE" ||
                tokenActual().tipo == "FECHA_LIMITE"
            )) {
                parseAtributo();
            }
            else {
                break;
            }
        }
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseAtributo
    //
    // Parsea un atributo individual. Aqui el parser toma una decision
    // basandose en el token actual (lookahead).
    //
    // Gramatica:
    //   <atributo> ::= PRIORIDAD    DOS_PUNTOS <valor_prioridad>
    //                | RESPONSABLE  DOS_PUNTOS CADENA
    //                | FECHA_LIMITE DOS_PUNTOS FECHA
    //
    // Esta es la funcion que mejor ilustra el concepto de
    // "parser descendente predictivo": con solo ver el token actual
    // sabemos exactamente cual de las tres producciones aplicar.
    // No hay ambiguedad porque cada produccion empieza con un token distinto.
    // ─────────────────────────────────────────────────────────────────────────
    void parseAtributo() {
        if (!hayTokens()) return;

        if (tokenActual().tipo == "PRIORIDAD") {
            // produccion: PRIORIDAD DOS_PUNTOS <valor_prioridad>
            consume("PRIORIDAD");
            consume("DOS_PUNTOS");
            parseValorPrioridad(); // no terminal: ALTA | MEDIA | BAJA
        }
        else if (tokenActual().tipo == "RESPONSABLE") {
            // produccion: RESPONSABLE DOS_PUNTOS CADENA
            consume("RESPONSABLE");
            consume("DOS_PUNTOS");
            consume("CADENA");
        }
        else if (tokenActual().tipo == "FECHA_LIMITE") {
            // produccion: FECHA_LIMITE DOS_PUNTOS FECHA
            consume("FECHA_LIMITE");
            consume("DOS_PUNTOS");
            consume("FECHA");
        }
        else {
            // El token actual no es ninguno de los tres atributos conocidos
            errors.push_back(Error(
                tokenActual().lexema,
                "Se esperaba: PRIORIDAD, RESPONSABLE o FECHA_LIMITE",
                "Sintáctico",
                tokenActual().linea,
                tokenActual().columna
            ));
        }
    }

    // ─────────────────────────────────────────────────────────────────────────
    // parseValorPrioridad
    //
    // Parsea el valor de la prioridad.
    //
    // Gramatica:
    //   <valor_prioridad> ::= ALTA | MEDIA | BAJA
    //
    // Los tres son terminales, asi que simplemente verificamos cual es
    // y avanzamos el puntero. Si no es ninguno de los tres, es un error.
    // ─────────────────────────────────────────────────────────────────────────
    void parseValorPrioridad() {
        if (!hayTokens()) return;

        if (tokenActual().tipo == "ALTA"  ||
            tokenActual().tipo == "MEDIA" ||
            tokenActual().tipo == "BAJA") {
            pos++; // consumimos el valor de prioridad
        }
        else {
            errors.push_back(Error(
                tokenActual().lexema,
                "Se esperaba: ALTA, MEDIA o BAJA",
                "Sintáctico",
                tokenActual().linea,
                tokenActual().columna
            ));
        }
    }

public:

    SyntaxAnalyzer(vector<Token> tokens) {
        this->tokens = tokens;
        this->pos = 0; // empezamos a parsear desde el primer token
    }


    // ─────────────────────────────────────────────────────────────────────────
    // analizar
    //
    // Punto de entrada publico del parser.
    // Simplemente arranca el analisis desde la produccion raiz.
    // Todo lo demas ocurre por las llamadas recursivas entre funciones.
    // ─────────────────────────────────────────────────────────────────────────
    void analizar() {
        parsePrograma(); // iniciamos el parseo desde la produccion raiz
    }

    vector<Error> getErrors() {
        return errors;
    }

};


#endif