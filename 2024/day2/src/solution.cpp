#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

uint64_t partA(std::ifstream& file) {
    uint64_t result = 0;
    std::string line;
    while (std::getline(file, line)) {
        // If line is empty, continue
        if (line.empty()) {
            continue;
        }

        // Check if line is safe.
        // 1. All numbers are either gradually increasing or decreasing.
        // 2. Adjacent numbers differ by at least 1 and at most 3.
        int num1, num2;
        std::istringstream iss(line);
        iss >> num1;
        if (!(iss >> num2)) {
            std::cerr << "Warning: Only one number found in line '" << line << "'\n";
            result += 1;
            continue;
        }

        if (num1 == num2) continue;
        bool increasing = (num2 > num1);
        bool safe = true;
        // INV: After i iterations,
        // If increasing is true, then all numbers from 0 ... (i+1) are increasing. Otherwise, all numbers from 0 ... (i+1) are decreasing.
        // ^ All adjacent numbers in 0 to i differ by at least 1 and at most 3. 
        // ^ 0 <= i < n, where n is the number of numbers in the line.
        // TERM: 2 numbers have been read. One more is read after each iteration. So, the loop terminates after n-1 iterations.
        do {
            // (num2 > num1) iff increasing
            if ((num2 > num1) != increasing) {
                safe = false;
                break;
            }
            uint64_t diff = std::abs(static_cast<int64_t>(num2) - static_cast<int64_t>(num1));
            if (diff < 1 || diff > 3) {
                safe = false;
                break;
            }

            num1 = num2;
        } while (iss >> num2);

        if (safe) result += 1;
    }

    return result;
}

bool isValid(int a, int b, bool increasing) {
    if ((b > a) != increasing) return false;
    uint64_t diff = std::abs(static_cast<int64_t>(a) - static_cast<int64_t>(b));
    return diff >= 1 && diff <= 3;
}

uint64_t partB(std::ifstream& file) {
    uint64_t result = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> nums;
        int num;
        while (iss >> num) nums.push_back(num);
        size_t n = nums.size();
        if (n <= 2) {
            result += 1;
            continue;
        }

        // Check if this vector is safe, or can be made safe by removing one number. Safe means:
        // 1. All numbers are either gradually increasing or decreasing.
        // 2. Adjacent numbers differ by at least 1 and at most 3.

        /*
         * Let the vector be V[l...r]. Let S1[i] be 1 if V[l...i] is increasing safe, -1 if V[l...i] is decreasing safe, 0 otherwise. Let S2[i] be 1/-1 if V[i ... r] is increasing/decreasing, 0 otherwise.
         * Define S1[l] and S2[r] as 1/-1 if V[l+1] > / <= V[l] and V[r] > / <= V[r-1] respectively.
         * Let S3[i] be 1 if removing V[i] makes V[l...r] increasing/decreasing safe, 0 otherwise.
         * S3[0] = 1 iff S2[1] != 0.
         * S3[n-1] = 1 iff S1[n-2] != 0.
         * S3[1] = 1 iff S2[2] != 0 and if n==3, then isValid(V[0], V[2], (V[2] - V[0]) > 0) otherwise isValid(V[0], V[2], S2[2] > 0)
         * S3[n-2] = 1 iff S1[n-3] != 0 and isValid(V[n-3], V[n-1], S1[n-3] > 0)
         * For 2 <= i <= n-3,
         * We have S3[i] is 1 iff (S1[i-1] * S2[i+1] > 0) and isValid(S[i-1], S[i+1], S1[i-1] > 0).
         * V[l...r] is safe iff S3[i] is 1 for some l <= i <= r.
        */
        std::vector<int> S1(n, 0);
        std::vector<int> S2(n, 0);

        // Calculate S1
        bool increasing = (nums[1] > nums[0]);
        S1[0] = increasing ? 1 : -1;
        for (size_t i = 1; i < n; i++) {
            if (! isValid(nums[i-1], nums[i], increasing)) {
                // All of S1[i...(n-1)] are 0
                break;
            } else {
                S1[i] = increasing ? 1 : -1;
            }
        }
        if (S1[n-1] != 0) { // V is safe without the need to remove anything
            result += 1;
            continue;
        }

        // Calculate S2
        increasing = (nums[n-1] > nums[n-2]);
        S2[n-1] = increasing ? 1 : -1;
        for (size_t i = n-2; i >= 0; i--) {
            if (! isValid(nums[i], nums[i+1], increasing)) {
                // All of S2[0...i] are 0
                break;
            } else {
                S2[i] = increasing ? 1 : -1;
            }
        }

        // Find an i for which S3[i] is 1
        bool found = false;
        if (S2[1] != 0) found = true; // S3[0] = 1
        if (S1[n-2] != 0) found = true; // S3[n-1] = 1
        if ( (S2[2] != 0) && ( (n == 3) ? isValid(nums[0], nums[2], (nums[2] - nums[0]) > 0) : isValid(nums[0], nums[2], S2[2] > 0) ) )
            found = true; // S3[1] = 1
        if ( (S1[n-3] != 0) && isValid(nums[n-3], nums[n-1], S1[n-3] > 0) )
            found = true; // S3[n-2] = 1
        
        for (size_t i = 2; i < n-2; i++) {
            if ( (S1[i-1] * S2[i+1] > 0) && isValid(nums[i-1], nums[i+1], S1[i-1] > 0) ) {
                found = true;
                break;
            }
        }

        if (found) result += 1;
    }

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file '" << inputFile << "'\n";
        return 1;
    }

    uint64_t result = partA(file);
    std::cout << "Part A: " << result << "\n";

    // Reset file stream
    file.clear();
    file.seekg(0);
    result = partB(file);
    std::cout << "Part B: " << result << "\n";

    return 0;
}