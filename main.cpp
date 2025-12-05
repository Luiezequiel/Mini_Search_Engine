#include "SearchEngine.h"
#include <iostream>

int main() {
    SearchEngine engine;

    std::string directory = "data/movies";
    engine.readFile(directory);
    engine.buildIndex();

    std::string query;
    while (true) {
        std::cout << "Ingrese consulta (1 o 2 palabras, 'exit' para salir): ";
        std::getline(std::cin, query);
        if (query == "exit") break;
        engine.search(query);
    }

    return 0;
}
