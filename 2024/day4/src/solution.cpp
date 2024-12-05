#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::vector<char>> parseInput(std::ifstream &file) {
    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row(line.size(), 0);
        for (size_t i = 0; i < line.size(); i++) {
            row[i] = line[i];
        }
        grid.push_back(std::move(row));
    }
    return grid;
}

unsigned searchInstancesPartA(const std::vector<std::vector<char>> &grid, size_t i, size_t j) {
    // Assuming grid[i][j] == 'X', search for "MAS" in all directions, forwards and backwards
    unsigned result = 0;
    size_t M = grid.size();
    size_t N = grid[0].size();

    // Search horizontally
    if (j + 3 < N && grid[i][j + 1] == 'M' && grid[i][j + 2] == 'A' && grid[i][j + 3] == 'S') {
        result++;
    }
    if (j >= 3 && grid[i][j - 1] == 'M' && grid[i][j - 2] == 'A' && grid[i][j - 3] == 'S') {
        result++;
    }

    // Search vertically
    if (i + 3 < M && grid[i + 1][j] == 'M' && grid[i + 2][j] == 'A' && grid[i + 3][j] == 'S') {
        result++;
    }
    if (i >= 3 && grid[i - 1][j] == 'M' && grid[i - 2][j] == 'A' && grid[i - 3][j] == 'S') {
        result++;
    }

    // Search diagonally
    if (i + 3 < M && j + 3 < N && grid[i + 1][j + 1] == 'M' && grid[i + 2][j + 2] == 'A' && grid[i + 3][j + 3] == 'S') {
        result++;
    }
    if (i >= 3 && j >= 3 && grid[i - 1][j - 1] == 'M' && grid[i - 2][j - 2] == 'A' && grid[i - 3][j - 3] == 'S') {
        result++;
    }
    if (i + 3 < M && j >= 3 && grid[i + 1][j - 1] == 'M' && grid[i + 2][j - 2] == 'A' && grid[i + 3][j - 3] == 'S') {
        result++;
    }
    if (i >= 3 && j + 3 < N && grid[i - 1][j + 1] == 'M' && grid[i - 2][j + 2] == 'A' && grid[i - 3][j + 3] == 'S') {
        result++;
    }

    return result;
}

unsigned partA(const std::vector<std::vector<char>> &grid) {
    // Pre: grid[0 ... (M-1)][0 ... (N-1)] is defined
    unsigned result = 0;
    size_t M = grid.size();
    size_t N = grid[0].size();

    // INV: 0 <= i <= M, result = all instances of "XMAS" with "X" appearing in grid[0...(i-1)][0...N]
    for (size_t i = 0; i < M; i++) {
        // INV: 0 <= j <= N, result = all instances of "XMAS" with "X" appearing in grid[0 ... (i-1)][0...N] + grid[i][0...(j-1)]
        for (size_t j = 0; j < N; j++) {
            if (grid[i][j] == 'X') {
                result += searchInstancesPartA(grid, i, j);
            }
        }
    }
    // Post: result = all instances of "XMAS" in grid[0 ... (M-1)][0 ... (N-1)], counted horizontally, vertically, diagonally and backwards
    return result;
}

unsigned searchInstancesPartB(const std::vector<std::vector<char>> &grid, size_t i, size_t j) {
    // Assuming grid[i][j] == 'A', search for 'M' and 'S' diagonally so that the diagonal forms "MAS" either forwards or backwards on both sides
    unsigned result = 0;
    size_t M = grid.size();
    size_t N = grid[0].size();

    bool leftFound = false, rightFound = false;
    // Search left-to-right
    if (i >= 1 && j >= 1 && i + 1 < M && j + 1 < N && grid[i - 1][j - 1] == 'M' && grid[i + 1][j + 1] == 'S') {
        leftFound = true;
    }
    if (i >= 1 && j >= 1 && i + 1 < M && j + 1 < N && grid[i - 1][j - 1] == 'S' && grid[i + 1][j + 1] == 'M') {
        leftFound = true;
    }
    // Search right-to-left
    if (i >= 1 && j + 1 < N && i + 1 < M && j >= 1 && grid[i - 1][j + 1] == 'M' && grid[i + 1][j - 1] == 'S') {
        rightFound = true;
    }
    if (i >= 1 && j + 1 < N && i + 1 < M && j >= 1 && grid[i - 1][j + 1] == 'S' && grid[i + 1][j - 1] == 'M') {
        rightFound = true;
    }

    if (leftFound && rightFound) {
        result++;
    }
    return result;
}

unsigned partB(const std::vector<std::vector<char>> &grid) {
    // Pre: grid[0 ... (M-1)][0 ... (N-1)] is defined
    unsigned result = 0;
    size_t M = grid.size();
    size_t N = grid[0].size();

    // INV: 0 <= i <= M, result = all instances of "X-MAS" with "A" appearing in grid[0...(i-1)][0...N]
    for (size_t i = 0; i < M; i++) {
        // INV: 0 <= j <= N, result = all instances of "X-MAS" with "A" appearing in grid[0 ... (i-1)][0...N] + grid[i][0...(j-1)]
        for (size_t j = 0; j < N; j++) {
            if (grid[i][j] == 'A') {
                result += searchInstancesPartB(grid, i, j);
            }
        }
    }
    // Post: result = all instances of "MAS" in the form of an X in grid[0 ... (M-1)][0 ... (N-1)] with "MAS" appearing forwards or backwards
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << "\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::vector<char>> grid = parseInput(file);

    unsigned result = partA(grid);
    std::cout << "Part A: " << result << "\n";

    result = partB(grid);
    std::cout << "Part B: " << result << "\n";

    return 0;
}