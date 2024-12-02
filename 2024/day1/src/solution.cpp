#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cassert>
#include <unordered_map>

/*
    * Take the path to input file as argument.
    * Each line contains two integers separated by a space, belonging to two different lists.
    * Read the file and parse it into two lists of integers.
*/
std::tuple<std::vector<int>, std::vector<int>> parseInput(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file.\n";
        std::exit(1);
    }

    std::vector<int> list1, list2;
    int num1, num2;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (iss >> num1) {
            if (iss >> num2) {
                list1.push_back(num1);
                list2.push_back(num2);
            } else {
                std::cerr << "Error: Line only contains one number. Exiting.\n";
                std::exit(1);
            }
        } else {
            std::cerr << "Warning: Line contains no numbers. Skipping.\n";
        }
    }
    
    return {list1, list2};
}

/*
    * Sort the two lists.
    * Iterate over both and add up the absolute differences between the corresponding numbers.
*/
uint64_t partA(std::vector<int> &list1, std::vector<int> &list2) {
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    uint64_t sum = 0;
    for (size_t i = 0; i < list1.size(); i++) {
        uint64_t diff = std::abs( static_cast<int64_t>(list1[i]) - static_cast<int64_t>(list2[i]) );
        sum += diff;
    }
    return sum;
}

/*
    * Create a vector to store counts in list2 of each number in list1.
    * Create an unordered_map to store a mapping from unique numbers in list1 to their index in the counts vector.
    * Iterate over list1 and fill in the map.
    * Iterate over list2 and fill in the counts vector.
    * Return sum of (num * count of num in list2) for each num in list1.
*/
int64_t partB(const std::vector<int> &list1, const std::vector<int> &list2) {
    std::vector<size_t> counts;
    counts.reserve(list1.size());
    std::unordered_map<int, size_t> numToCountsIdx;

    for (const int& i : list1) {
        if (numToCountsIdx.find(i) == numToCountsIdx.end()) {
            numToCountsIdx[i] = counts.size();
            counts.push_back(0);
        }
    }

    for (const int& i : list2) {
        if (numToCountsIdx.find(i) != numToCountsIdx.end()) {
            counts[numToCountsIdx[i]]++;
        }
    }

    int64_t sum = 0;
    for (const int& i : list1) {
        sum += i * counts[numToCountsIdx[i]];
    }

    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::vector<int> list1, list2;
    std::tie(list1, list2) = parseInput(argv[1]);
    assert(list1.size() == list2.size() && "Error: Lists are not of equal length.");

    uint64_t sum1 = partA(list1, list2);
    std::cout << "Part a: " << sum1 << '\n';

    int64_t sum2 = partB(list1, list2);
    std::cout << "Part b: " << sum2 << '\n';

    return 0;
}