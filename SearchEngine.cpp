#include "SearchEngine.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
namespace fs = filesystem;

void SearchEngine::readFile(const string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) continue;

        string filename = entry.path().filename().string();
        ifstream ifs(entry.path());
        if (!ifs) continue;

        ostringstream ss;
        ss << ifs.rdbuf();
        string content = ss.str();

        istringstream iss(content);
        string word;
        while (iss >> word) {
            string clean = CleanWord(word);
            if (clean.empty()) continue;
            OrderMapString[filename][clean]++;
        }
    }
}

void SearchEngine::buildIndex() {
    for (const auto& file_pair : OrderMapString) {
        const string& filename = file_pair.first;
        for (const auto& word_pair : file_pair.second) {
            const string& word = word_pair.first;
            int freq = word_pair.second;
            OrderMapStringIndex[word][filename] = freq;
        }
    }
}

std::string SearchEngine::CleanWord(const string& word) const {
    string result;
    for (char c : word) {
        if (isalnum(c)) result += tolower(c);
    }
    return result;
}

void SearchEngine::search(const string& query) {
    string w1, w2;
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

    vector<pair<string,int>> results;

    if (!w1.empty() && w2.empty()) {
        // Una palabra
        if (OrderMapStringIndex.find(w1) != OrderMapStringIndex.end()) {
            for (auto& p : OrderMapStringIndex[w1])
                results.push_back(p);
        }
    } else if (!w1.empty() && !w2.empty()) {
        // Dos palabras
        if (OrderMapStringIndex.find(w1) != OrderMapStringIndex.end() &&
            OrderMapStringIndex.find(w2) != OrderMapStringIndex.end()) {

            for (auto& p : OrderMapStringIndex[w1]) {
                const string& filename = p.first;
                int freq1 = p.second;
                if (OrderMapStringIndex[w2].count(filename)) {
                    int freq2 = OrderMapStringIndex[w2][filename];
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


void SearchEngine::displayTopResult(const vector<pair<string,int>>& results) {
    if (results.empty()) {
        cout << "No se encontraron resultados.\n";
        return;
    }

    // priority_queue para ordenar de mayor a menor por frecuencia
    priority_queue<pair<int,string>> pq;
    for (const auto& r : results) {
        pq.push({r.second, r.first}); // primero frecuencia, luego nombre
    }

    cout << "Top 3 resultados:\n";
    int count = 0;
    while (!pq.empty() && count < 3) {
        auto top = pq.top();
        pq.pop();
        cout << top.second << " (frecuencia: " << top.first << ")\n";
        count++;
    }
}
