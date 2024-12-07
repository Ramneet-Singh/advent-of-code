#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

const std::pair<
    const std::unordered_map<int, std::unordered_set<int>>,
    std::vector<std::vector<int>>
> parseInput(std::ifstream& input) {
    std::pair<
        std::unordered_map<int, std::unordered_set<int>>,
        std::vector<std::vector<int>>
    > result;
    auto& lessThanElemsMap = result.first;
    auto& updates = result.second;

    std::string line;
    while (std::getline(input, line) && !line.empty()) {
        auto it = line.find("|");
        if (it == std::string::npos) {
            throw std::invalid_argument("Invalid input format. Expected '|'\n");
        }
        int n1 = std::stoi(line.substr(0, it));
        int n2 = std::stoi(line.substr(it + 1));

        if (lessThanElemsMap.find(n2) == lessThanElemsMap.end()) {
            lessThanElemsMap[n2] = std::unordered_set<int>();
        }
        if (lessThanElemsMap.find(n1) == lessThanElemsMap.end()) {
            lessThanElemsMap[n1] = std::unordered_set<int>();
        }
        lessThanElemsMap[n2].insert(n1);
    }

    while (std::getline(input, line)) {
        std::vector<int> update;
        std::istringstream iss(line);
        int n;
        char delimiter;
        do {
            iss >> n;
            update.push_back(n);
        } while (iss >> delimiter);
        updates.push_back(std::move(update));
    }

    return result;
}

bool isLessThan(const std::unordered_map<int, std::unordered_set<int>>& lessThanElemsMap, const int a, const int b) {
    if (lessThanElemsMap.find(b) == lessThanElemsMap.end()) {
        return false;
    }
    return lessThanElemsMap.at(b).find(a) != lessThanElemsMap.at(b).end();
}

uint64_t partA(const std::unordered_map<int, std::unordered_set<int>>& lessThanElemsMap, std::vector<std::vector<int>>& updates) {
    uint64_t result = 0;
    for (const auto& update : updates) {
        bool correct = true;
        std::unordered_set<int> excluded;
        for (const auto& n : update) {
            if (excluded.find(n) != excluded.end()) {
                correct = false;
                break;
            }
            if (lessThanElemsMap.find(n) == lessThanElemsMap.end()) {
                continue;
            }
            excluded.insert(lessThanElemsMap.at(n).begin(), lessThanElemsMap.at(n).end());
        }

        if (correct) {
            result += update[update.size() / 2];
        }
    }
    return result;
}

uint64_t partB(const std::unordered_map<int, std::unordered_set<int>>& lessThanElemsMap, std::vector<std::vector<int>>& updates) {
    uint64_t result = 0;
    for (auto& update : updates) {
        bool correct = true;
        std::unordered_set<int> excluded;
        for (const auto& n : update) {
            if (excluded.find(n) != excluded.end()) {
                correct = false;
                break;
            }
            if (lessThanElemsMap.find(n) == lessThanElemsMap.end()) {
                continue;
            }
            excluded.insert(lessThanElemsMap.at(n).begin(), lessThanElemsMap.at(n).end());
        }

        if (!correct) {
            std::sort(update.begin(), update.end(), [&lessThanElemsMap](const int a, const int b) {
                return isLessThan(lessThanElemsMap, a, b);
            });
            result += update[update.size() / 2];
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
        return 1;
    }

    auto [lessThanElemsMap, updates] = parseInput(input_file);

    uint64_t result = partA(lessThanElemsMap, updates);
    std::cout << "Part A: " << result << "\n";

    result = partB(lessThanElemsMap, updates);
    std::cout << "Part B: " << result << "\n";

    return 0;
}
