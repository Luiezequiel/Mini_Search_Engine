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
    std::istringstream iss(query);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(CleanWord(token));

    std::vector<std::pair<std::string,int>> results;

    if (tokens.size() == 1) {
        std::string w = tokens[0];
        if (OrderMapStringIndex.find(w) != OrderMapStringIndex.end()) {
            for (auto& pair : OrderMapStringIndex[w])
                results.push_back(pair);
        }
    } else if (tokens.size() == 2) {
        std::string w1 = tokens[0];
        std::string w2 = tokens[1];
        if (OrderMapStringIndex.find(w1) != OrderMapStringIndex.end() &&
            OrderMapStringIndex.find(w2) != OrderMapStringIndex.end()) {

            for (auto& pair1 : OrderMapStringIndex[w1]) {
                const std::string& filename = pair1.first;
                int freq1 = pair1.second;
                if (OrderMapStringIndex[w2].find(filename) != OrderMapStringIndex[w2].end()) {
                    int freq2 = OrderMapStringIndex[w2][filename];
                    results.push_back({filename, freq1 + freq2});
                }
            }
        }
    } else {
        std::cout << "Máximo 2 palabras en la búsqueda.\n";
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
