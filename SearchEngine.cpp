#include "SearchEngine.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

void SearchEngine::readFile(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::ifstream ifs(entry.path());
        if (!ifs) continue;

        std::ostringstream ss;
        ss << ifs.rdbuf();
        std::string content = ss.str();

        std::istringstream iss(content);
        std::string word;
        while (iss >> word) {
            std::string clean = CleanWord(word);
            if (clean.empty()) continue;
            OrderMapString[filename][clean]++;
        }
    }
}

void SearchEngine::buildIndex() {
    for (const auto& file_pair : OrderMapString) {
        const std::string& filename = file_pair.first;
        for (const auto& word_pair : file_pair.second) {
            const std::string& word = word_pair.first;
            int freq = word_pair.second;
            OrderMapStringIndex[word][filename] = freq;
        }
    }
}

std::string SearchEngine::CleanWord(const std::string& word) const {
    std::string result;
    for (char c : word) {
        if (std::isalnum(c)) result += std::tolower(c);
    }
    return result;
}

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
                const std::string& filename = p.first;
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

void SearchEngine::displayTopResult(const std::vector<std::pair<std::string,int>>& result) {
    if (result.empty()) {
        std::cout << "No se encontraron archivos relevantes.\n";
        return;
    }

    // priority_queue para top 3
    auto cmp = [](const std::pair<std::string,int>& a, const std::pair<std::string,int>& b){
        return a.second < b.second; // mayor frecuencia primero
    };
    std::priority_queue<std::pair<std::string,int>,
                        std::vector<std::pair<std::string,int>>,
                        decltype(cmp)> pq(cmp);

    for (auto& r : result) pq.push(r);

    int count = 0;
    while (!pq.empty() && count < 3) {
        auto top = pq.top(); pq.pop();
        std::cout << top.first << " (frecuencia: " << top.second << ")\n";
        count++;
    }
}
