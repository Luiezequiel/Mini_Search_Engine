#include "SearchEngine.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    //Se crea motor de búsqueda
    SearchEngine engine;
    
    // Directorio con archivos
    string directory = "data/movies";

    // Lee archivos y construir índice
    engine.readFile(directory);
    engine.buildIndex();

    string query;

    // Permitir múltiples búsquedas
    while (true) {
        cout << "Ingrese consulta (1 o 2 palabras, 'exit' para salir): ";
        getline(std::cin, query);
        if (query == "exit") break;
        
        //Busca y muestra resultados
        engine.search(query);
    }

    return 0;
}
