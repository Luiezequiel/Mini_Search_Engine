#include "SearchEngine.h"
#include <iostream>

int main() {
    //Se crea motor de búsqueda
    SearchEngine engine;

    // Directorio con archivos
    std::string directory = "data/movies";
    
     // Lee archivos y construir índice
    engine.readFile(directory);
    engine.buildIndex();

    std::string query;

    // Permite múltiples búsquedas
    while (true) {
        std::cout << "Ingrese consulta (1 o 2 palabras, 'exit' para salir): ";
        std::getline(std::cin, query);
        if (query == "exit") break;
        
        //Busca y muestra resultados
        engine.search(query);
    }

    return 0;
}
