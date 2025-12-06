#include "SearchEngine.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Lee todos los archivos del directorio y cuenta frecuencia de palabras
void SearchEngine::readFile(const std::string& directory) {
    // Itera sobre cada archivo en el directorio
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::ifstream ifs(entry.path());
        if (!ifs) continue;

        // Lee todo el contenido del archivo
        std::ostringstream ss;
        ss << ifs.rdbuf();
        std::string content = ss.str();

        // Divide el contenido en palabras individuales
        std::istringstream iss(content);
        std::string word;
        while (iss >> word) {
            std::string clean = CleanWord(word);
            if (clean.empty()) continue;
             // Incrementa frecuencia de la palabra en el archivo
            OrderMapString[filename][clean]++;
        }
    }
}
// Construye el índice invertido a partir de los datos leídos
void SearchEngine::buildIndex() {
    for (const auto& file_pair : OrderMapString) {
        const std::string& filename = file_pair.first;
        for (const auto& word_pair : file_pair.second) {
            const std::string& word = word_pair.first;
            int freq = word_pair.second;
            // Invierte la estructura: ahora cada palabra apunta a sus archivos
            OrderMapStringIndex[word][filename] = freq;
        }
    }
}

// Limpia una palabra: quita puntuación y convierte a minúsculas
std::string SearchEngine::CleanWord(const std::string& word) const {
    std::string result;
    for (char c : word) {
        // Verifica si es letra (a-z, A-Z) o número (0-9)
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            // Convierte a minúscula si es letra mayúscula
            if (c >= 'A' && c <= 'Z')
                result += c + 32;  // convierte a minúscula
            else
                result += c;
        }
    }
    return result;
}

// Procesa la consulta del usuario y busca archivos relevantes
void SearchEngine::search(const std::string& query) {
    std::string w1, w2;
    bool foundSpace = false;

    // Recorremos cada carácter
    for (char c : query) {
        if (c == ' ' && !foundSpace) {
            foundSpace = true; // encontramos el espacio entre palabras
            continue;
        }
        if (!foundSpace)
            w1 += c; // construye primera palabra
        else
            w2 += c; // construye segunda palabra
    }

    // Limpiar palabras
    w1 = CleanWord(w1);
    w2 = CleanWord(w2);

    std::vector<std::pair<std::string,int>> results;

    // Búsqueda de una sola palabra
    if (!w1.empty() && w2.empty()) {
        // Una palabra
        if (OrderMapStringIndex.find(w1) != OrderMapStringIndex.end()) {
            for (auto& p : OrderMapStringIndex[w1])
                results.push_back(p);
        }

    } 
    // Búsqueda de dos palabras
    else if (!w1.empty() && !w2.empty()) {
       
        if (OrderMapStringIndex.find(w1) != OrderMapStringIndex.end() &&
            OrderMapStringIndex.find(w2) != OrderMapStringIndex.end()) {

            for (auto& p : OrderMapStringIndex[w1]) {
                const std::string& filename = p.first;
                int freq1 = p.second;
                if (OrderMapStringIndex[w2].count(filename)) {
                    int freq2 = OrderMapStringIndex[w2][filename];
                    // Suma las frecuencias de ambas palabras
                    results.push_back({filename, freq1 + freq2});
                }
            }
        }
    } else {
        std::cout << "Consulta vacía.\n";
        return;
    }

    displayTopResult(results);
}

// Muestra los top 3 archivos más relevantes
void SearchEngine::displayTopResult(const std::vector<std::pair<std::string,int>>& results) {
    if (results.empty()) {
        std::cout << "No se encontraron resultados.\n";
        return;
    }

    // priority_queue para ordenar de mayor a menor por frecuencia
    std::priority_queue<std::pair<int,std::string>> pq;
    for (const auto& r : results) {
        pq.push({r.second, r.first}); // primero frecuencia, luego nombre
    }

    std::cout << "Top 3 resultados:\n";
    int count = 0;
    while (!pq.empty() && count < 3) {
        auto top = pq.top();
        pq.pop();
        std::cout << top.second << " (frecuencia: " << top.first << ")\n";
        count++;
    }
}
