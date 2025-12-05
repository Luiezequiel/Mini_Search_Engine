#include "SearchEngine.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    SearchEngine engine;
#
    string directory = "data/movies";
    engine.readFile(directory);
    engine.buildIndex();

    string query;
    while (true) {
        cout << "Ingrese consulta (1 o 2 palabras, 'exit' para salir): ";
        getline(std::cin, query);
        if (query == "exit") break;
        engine.search(query);
    }

    return 0;
}
