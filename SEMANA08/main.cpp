#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;


int main() {
    
    string bloquePacientes = R"(HOSPITAL {
    PACIENTES {
        paciente: "Maria Garcia" [edad: 45, tipo_sangre: "A+", habitacion: 302],
        paciente: "Carlos Mendoza" [edad: 62, tipo_sangre: "O-", habitacion: 410],
    };
};)";

    LexicalAnalyzer analyzer(bloquePacientes);

    analyzer.analizar();

    vector<Token> tokens = analyzer.getTokens();

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
    return 0;
}