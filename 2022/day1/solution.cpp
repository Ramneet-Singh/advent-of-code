#include <iostream>
#include <chrono>
#include <string>
#include <queue>

// Read a list of (blank line separated) lists of (newline separated) integers
// froms stdin. Output the sum of top 3 sums of lists.
int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    std::priority_queue<long long> sums;
    long long curSum = 0;
    std::string line;
    // Read till EOF
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            // End of one list
            sums.push(curSum);
            curSum = 0;
        }else curSum += std::stoi(line);
    }
    std::cout << "Answer to part 1: " << sums.top() << "\n";
    long long ans2 = 0;
    for (int i=0; i<3; i++)
    {
        ans2 += sums.top();
        sums.pop();
    }
    std::cout << "Answer to part 2: " << ans2 << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Total Time - " << (end-start).count() << "ns\n";
    return 0;
}