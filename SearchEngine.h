#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cctype>
#include <algorithm>

using namespace std;

class SearchEngine {
public:
    SearchEngine() = default;

    // Lee todos los archivos del directorio, tokeniza y cuenta palabras
    void readFile(const string& directory);

    // Construye el índice invertido usando los datos leídos
    void buildIndex();

    // Limpia y normaliza una palabra (quita puntuación y pasa a minúscula)
    string CleanWord(const string& word) const;

    // Busca la consulta del usuario (1 o 2 palabras)
    void search(const string& query);

    // Muestra los 3 resultados más relevantes
    void displayTopResult(const vector<pair<string,int>>& result);

private:
    // Frecuencia de palabras por archivo: archivo -> (palabra -> frecuencia)
    unordered_map<string, unordered_map<string,int>> OrderMapString;

    // Índice invertido: palabra -> (archivo -> frecuencia)
    unordered_map<string, unordered_map<string,int>> OrderMapStringIndex;
};

#endif