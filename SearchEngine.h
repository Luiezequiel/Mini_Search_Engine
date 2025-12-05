#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cctype>
#include <algorithm>

class SearchEngine {
public:
    SearchEngine() = default;

    // Lee todos los archivos del directorio, tokeniza y cuenta palabras
    void readFile(const std::string& directory);

    // Construye el índice invertido usando los datos leídos
    void buildIndex();

    // Limpia y normaliza una palabra (quita puntuación y pasa a minúscula)
    std::string CleanWord(const std::string& word) const;

    // Busca la consulta del usuario (1 o 2 palabras)
    void search(const std::string& query);

    // Muestra los 3 resultados más relevantes
    void displayTopResult(const std::vector<std::pair<std::string,int>>& result);

private:
    // Frecuencia de palabras por archivo: archivo -> (palabra -> frecuencia)
    std::unordered_map<std::string, std::unordered_map<std::string,int>> OrderMapString;

    // Índice invertido: palabra -> (archivo -> frecuencia)
    std::unordered_map<std::string, std::unordered_map<std::string,int>> OrderMapStringIndex;
};

#endif