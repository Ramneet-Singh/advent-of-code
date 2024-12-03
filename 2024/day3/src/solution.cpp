#include <iostream>
#include <fstream>
#include <regex>

int64_t partA(std::ifstream& file) {
    int64_t result = 0;
    // Search for mul(num1,num2) sequences in the file
    // If a sequence is found, increment result by num1*num2

    // construct string from file content
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::regex pattern("mul\\((\\d+),(\\d+)\\)");
    std::smatch matches;

    while (std::regex_search(content, matches, pattern)) {
        int64_t num1 = static_cast<int64_t> (std::stoi(matches[1].str()));
        int64_t num2 = static_cast<int64_t> (std::stoi(matches[2].str()));
        result += num1 * num2;
        content = matches.suffix().str();
    }

    return result;
}

int64_t partB(std::ifstream& file) {
    int64_t result = 0;
    // Search for mul(num1,num2) sequences in the file which are either not preceded by any do()/don't() or are preceded by do() most recently.
    // construct string from file content
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::regex doPattern("do\\(\\)");
    std::regex dontPattern("don't\\(\\)");
    std::regex mulPattern("mul\\((\\d+),(\\d+)\\)");
    std::smatch matches;
    std::smatch matchesInner;

    bool enabled = true;
    while (std::regex_search(content, matches, dontPattern)) {
        std::string rest = matches.suffix().str();
        enabled = false;
        content = matches.prefix().str();

        while (std::regex_search(content, matchesInner, mulPattern)) {
            int64_t num1 = static_cast<int64_t> (std::stoi(matchesInner[1].str()));
            int64_t num2 = static_cast<int64_t> (std::stoi(matchesInner[2].str()));
            result += num1 * num2;
            content = matchesInner.suffix().str();
        }

        // Search for the next do()
        content = rest;
        if (std::regex_search(content, matchesInner, doPattern)) {
            content = matchesInner.suffix().str();
            enabled = true;
        } else {
            break;
        }
    }

    // Check for mul() sequences after the last do()
    if (enabled) {
        while (std::regex_search(content, matches, mulPattern)) {
            int64_t num1 = static_cast<int64_t> (std::stoi(matches[1].str()));
            int64_t num2 = static_cast<int64_t> (std::stoi(matches[2].str()));
            result += num1 * num2;
            content = matches.suffix().str();
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'\n";
        return 1;
    }

    int64_t result = partA(file);
    std::cout << "Part A: " << result << "\n";

    file.clear();
    file.seekg(0);
    int64_t resultB = partB(file);
    std::cout << "Part B: " << resultB << "\n";

    return 0;
}