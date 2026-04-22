#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

using namespace std;

void generarReportePacientes(vector<Token> tokens) {
    // -------- REPORTE DE PACIENTES --------
    // Este reporte extrae informacion semantica de los tokens ya reconocidos.
    // Recorre el vector de tokens buscando el patron que define a cada paciente:
    // paciente : "nombre" [ edad : numero , tipo_sangre : "tipo" , habitacion : numero ]
    //

    ofstream reportePac("reporte_pacientes.html");

    reportePac << "<!DOCTYPE html>\n";
    reportePac << "<html>\n";
    reportePac << "<head>\n";
    reportePac << "<meta charset='UTF-8'>\n";
    reportePac << "<title>Historial de Pacientes</title>\n";
    reportePac << "<style>\n";
    reportePac << "body { font-family: Arial; background-color: #f5f5f5; }\n";
    reportePac << "h2 { text-align: center; color: #1A4731; }\n";
    reportePac << "table { border-collapse: collapse; width: 85%; margin: auto; background: white; }\n";
    reportePac << "th { background-color: #1A4731; color: white; padding: 10px; }\n";
    reportePac << "td { border: 1px solid #ddd; padding: 8px; text-align: center; }\n";
    reportePac << "tr:nth-child(even) { background-color: #f2f2f2; }\n";
    reportePac << ".activo { color: green; font-weight: bold; }\n";
    reportePac << ".sin-diag { color: orange; font-weight: bold; }\n";
    reportePac << "</style>\n";
    reportePac << "</head>\n";
    reportePac << "<body>\n";

    reportePac << "<h2>Historial de Pacientes</h2>\n";

    reportePac << "<table>\n";
    reportePac << "<tr>\n";
    reportePac << "<th>#</th>\n";
    reportePac << "<th>Paciente</th>\n";
    reportePac << "<th>Edad</th>\n";
    reportePac << "<th>Tipo de Sangre</th>\n";
    reportePac << "<th>Habitacion</th>\n";
    reportePac << "<th>Estado</th>\n";
    reportePac << "</tr>\n";

    // Recorremos el vector de tokens buscando la palabra reservada "paciente"
    // Cuando la encontramos, los siguientes tokens siguen un patron predecible
    // gracias a la estructura del lenguaje MedLang.
    //
    // El indice 'i' avanza manualmente para ir extrayendo cada dato en orden.

    int numPaciente = 1;

    for (int i = 0; i < tokens.size(); i++) {

        // Buscamos un token IDENTIFICADOR cuyo lexema sea "paciente"
        if (tokens[i].tipo == "IDENTIFICADOR" && tokens[i].lexema == "paciente") {

            string nombre = "";
            string edad = "";
            string tipoSangre = "";
            string habitacion = "";

            // Avanzamos al siguiente token, esperamos ":"
            i++; // ahora deberia ser ":"

            // El siguiente token despues de ":" es el nombre del paciente (CADENA)
            i++; // ahora deberia ser la CADENA con el nombre
            if (i < tokens.size() && tokens[i].tipo == "CADENA") {
                // Quitamos las comillas del lexema para mostrarlo limpio
                nombre = tokens[i].lexema.substr(1, tokens[i].lexema.size() - 2);
            }

            // Ahora buscamos los atributos dentro de los corchetes [ ... ]
            // Avanzamos hasta encontrar cada palabra clave: edad, tipo_sangre, habitacion
            // No asumimos un orden fijo, sino que buscamos por nombre de atributo
            // hasta encontrar el cierre del corchete "]"

            while (i < tokens.size()) {
                i++;

                // Si encontramos el cierre del bloque de atributos, dejamos de buscar
                if (tokens[i].tipo == "SIMBOLO" && tokens[i].lexema == "]") {
                    break;
                }

                // Buscamos el atributo "edad"
                if (tokens[i].tipo == "IDENTIFICADOR" && tokens[i].lexema == "edad") {
                    i++; // saltamos el ":"
                    i++; // ahora deberia ser el ENTERO con la edad
                    if (i < tokens.size() && tokens[i].tipo == "ENTERO") {
                        edad = tokens[i].lexema;
                    }
                }

                // Buscamos el atributo "tipo_sangre"
                // Sin embargo, con tu lexer actual el '_' no es letra,
                // asi que "tipo_sangre" se tokeniza como "tipo" + error "_" + "sangre".
                // Por eso aqui buscamos solo "tipo" seguido eventualmente de "sangre".
                if (tokens[i].tipo == "IDENTIFICADOR" && tokens[i].lexema == "tipo_sangre") {
                    i++; // saltamos el ":"
                    i++; // ahora deberia ser la CADENA con el tipo de sangre
                    if (i < tokens.size() && tokens[i].tipo == "CADENA") {
                        tipoSangre = tokens[i].lexema.substr(1, tokens[i].lexema.size() - 2);
                    }
                }

                // Buscamos el atributo "habitacion"
                if (tokens[i].tipo == "IDENTIFICADOR" && tokens[i].lexema == "habitacion") {
                    i++; // saltamos el ":"
                    i++; // ahora deberia ser el ENTERO con el numero de habitacion
                    if (i < tokens.size() && tokens[i].tipo == "ENTERO") {
                        habitacion = tokens[i].lexema;
                    }
                }
            }

            // Escribimos la fila del paciente en la tabla HTML
            reportePac << "<tr>\n";
            reportePac << "<td>" << numPaciente++ << "</td>\n";
            reportePac << "<td>" << nombre << "</td>\n";
            reportePac << "<td>" << edad << "</td>\n";
            reportePac << "<td>" << tipoSangre << "</td>\n";
            reportePac << "<td>" << habitacion << "</td>\n";

            // El estado se determina aqui de forma simple:
            // Como solo tenemos el bloque PACIENTES (sin DIAGNOSTICOS),
            // todos los pacientes se marcan como "SIN DIAG."
            // En el proyecto completo, cruzarian esta info con el bloque DIAGNOSTICOS.
            reportePac << "<td class='sin-diag'>SIN DIAG.</td>\n";

            reportePac << "</tr>\n";
        }
    }

    reportePac << "</table>\n";
    reportePac << "</body>\n";
    reportePac << "</html>\n";

    reportePac.close();

    cout << "Reporte de pacientes generado: reporte_pacientes.html\n";
}

void generarReporteTokens(vector<Token> tokens) {
    cout << "----- TOKENS -----\n";

    for (Token t : tokens) {
        cout << t.numero << " | "
             << t.tipo << " | "
             << t.lexema << " | "
             << "Linea: " << t.linea << " | "
             << "Columna: " << t.columna << endl;
    }

    // -------- HTML --------
    ofstream archivo("tokens.html");

    archivo << "<!DOCTYPE html>\n";
    archivo << "<html>\n";
    archivo << "<head>\n";
    archivo << "<meta charset='UTF-8'>\n";
    archivo << "<title>Reporte de Tokens</title>\n";

    // CSS embebido para mejorar la apariencia de la tabla
    archivo << "<style>\n";
    archivo << "body { font-family: Arial; }\n";
    archivo << "table { border-collapse: collapse; width: 80%; margin: auto; }\n";
    archivo << "th, td { border: 1px solid black; padding: 8px; text-align: center; }\n";
    archivo << "th { background-color: #f2f2f2; }\n";
    archivo << "</style>\n";

    archivo << "</head>\n";
    archivo << "<body>\n";

    archivo << "<h2 style='text-align:center;'>Reporte de Tokens</h2>\n";

    archivo << "<table>\n";
    archivo << "<tr>\n";
    archivo << "<th>#</th>\n";
    archivo << "<th>Tipo</th>\n";
    archivo << "<th>Lexema</th>\n";
    archivo << "<th>Línea</th>\n";
    archivo << "<th>Columna</th>\n";
    archivo << "</tr>\n";

    for (Token t : tokens) {
        archivo << "<tr>\n";
        archivo << "<td>" << t.numero << "</td>\n";
        archivo << "<td>" << t.tipo << "</td>\n";
        archivo << "<td>" << t.lexema << "</td>\n";
        archivo << "<td>" << t.linea << "</td>\n";
        archivo << "<td>" << t.columna << "</td>\n";
        archivo << "</tr>\n";
    }

    archivo << "</table>\n";

    archivo << "</body>\n";
    archivo << "</html>\n";

    archivo.close();

    cout << "\nReporte HTML generado: tokens.html\n";
}

void generarReporteErrores(vector<Error> errores, string nombreArchivo = "errores.html") {
     // -------- ERRORES EN CONSOLA --------
    

    if (!errores.empty()) {
        cout << "\n----- ERRORES -----\n";
        int numError = 1;
        for (Error e : errores) {
            cout << numError++ << " | "
                << "Lexema: " << e.lexema << " | "
                << "Caracter: " << e.caracter << " | "
                << "Tipo: " << e.tipo << " | "
                << "Linea: " << e.linea << " | "
                << "Columna: " << e.columna << endl;
        }
    }

    // -------- HTML DE ERRORES --------
    if (!errores.empty()) {
        ofstream archivoErr(nombreArchivo);

        archivoErr << "<!DOCTYPE html>\n";
        archivoErr << "<html>\n";
        archivoErr << "<head>\n";
        archivoErr << "<meta charset='UTF-8'>\n";
        archivoErr << "<title>Reporte de Errores</title>\n";
        archivoErr << "<style>\n";
        archivoErr << "body { font-family: Arial; }\n";
        archivoErr << "table { border-collapse: collapse; width: 80%; margin: auto; }\n";
        archivoErr << "th, td { border: 1px solid black; padding: 8px; text-align: center; }\n";
        archivoErr << "th { background-color: #f8d7da; color: #721c24; }\n";
        archivoErr << "tr:nth-child(even) { background-color: #fff3f3; }\n";
        archivoErr << "</style>\n";
        archivoErr << "</head>\n";
        archivoErr << "<body>\n";

        archivoErr << "<h2 style='text-align:center; color: #721c24;'>Reporte de Errores Léxicos</h2>\n";

        archivoErr << "<table>\n";
        archivoErr << "<tr>\n";
        archivoErr << "<th>#</th>\n";
        archivoErr << "<th>Lexema</th>\n";
        archivoErr << "<th>Carácter</th>\n";
        archivoErr << "<th>Tipo</th>\n";
        archivoErr << "<th>Línea</th>\n";
        archivoErr << "<th>Columna</th>\n";
        archivoErr << "</tr>\n";

        int numError = 1;
        for (Error e : errores) {
            archivoErr << "<tr>\n";
            archivoErr << "<td>" << numError++ << "</td>\n";
            archivoErr << "<td>" << e.lexema << "</td>\n";
            archivoErr << "<td>" << e.caracter << "</td>\n";
            archivoErr << "<td>" << e.tipo << "</td>\n";
            archivoErr << "<td>" << e.linea << "</td>\n";
            archivoErr << "<td>" << e.columna << "</td>\n";
            archivoErr << "</tr>\n";
        }

        archivoErr << "</table>\n";
        archivoErr << "</body>\n";
        archivoErr << "</html>\n";

        archivoErr.close();
        cout << "Reporte de errores generado: " << nombreArchivo << "\n";
    }
}
int main() {
    
    string ejemplo = R"(
        TABLERO "Proyecto  LFP" $ {
            COLUMNA "Por Hacer" {
                tarea: "Diseñar AFD" [prioridad ALTA, responsable: "Jorge", fecha_limite: 2026-05-01],
                tarea: "Implementar Lexer" [prioridad: ALTA, responsable: "María", fecha_limite: 2026-05-08],
                tarea: "Escribir casos de prueba" [prioridad: MEDIA, responsable: "Carlos", fecha_limite: 2026-05-10],
            }
        };
    )";

    LexicalAnalyzer analyzer(ejemplo);

    analyzer.analizar();

    vector<Token> tokens = analyzer.getTokens();
    vector<Error> errores = analyzer.getErrors();

    generarReporteTokens(tokens);

    if (!analyzer.getErrors().empty()) {//si el vector de errores no esta vacio, generamos el reporte de errores
        generarReporteErrores(errores, "errores_lexicos.html");
    }

    
    SyntaxAnalyzer parser(tokens);
    parser.analizar();

    vector<Error> erroresSintacticos = parser.getErrors();

    if (erroresSintacticos.empty()) {
        cout << "\nAnálisis sintáctico exitoso.\n";
    } else {
        generarReporteErrores(erroresSintacticos, "errores_sintacticos.html");
    }

    return 0;

    
}