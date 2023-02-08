#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::string snafu_from_number(long long int number) {
    std::vector<int> digits;
    std::string snafu = "";
    while (number > 0) {
        int digit = number % 5;
        number /= 5;
        if (digit == 4) {
            number ++;
            snafu = "-" + snafu;
        }  else if (digit == 3) {
            number ++;
            snafu = "=" + snafu;
        } else {
            snafu = std::to_string(digit) + snafu;
        }
    }
    return snafu;
}

long long int number_from_snafu(std::string snafu) {
    long long int total = 0;
    for (char digit : snafu) {
        total *= 5;
        if (digit == '2') {
            total += 2;
        } else if (digit == '1') {
            total += 1;
        } else if (digit == '-') {
            total -= 1;
        } else if (digit == '=') {
            total -= 2;
        }
    }
    return total;
}

std::string snafu_sum(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    long long int total = 0;
    while (std::getline(input, line)) {
        total += number_from_snafu(line);
    }
    return snafu_from_number(total);
}

int main() {
    std::cout << snafu_sum("test1.txt") << "\n";
    std::cout << snafu_sum("input.txt") << "\n";
}