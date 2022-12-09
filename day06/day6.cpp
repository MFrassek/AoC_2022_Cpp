#include <iostream>
#include <fstream>
#include <string>
#include <set>

int message_start(std::string file_name, bool start_of_package) {
    std::ifstream input(file_name);
    std::string line;
    std::getline(input, line);
    int offset;
    if (start_of_package) {
        offset = 4;
    } else {
        offset = 14;
    }
    for (int i = 0; i < line.size() - offset; i++) {
        std::set<char> chars(line.begin() + i, line.begin() + i + offset);
        if (chars.size() == offset) {
            return i + offset;
        }
    }
    return -1;
}

int main() {
    std::cout << message_start("test1.txt", true) << std::endl;
    std::cout << message_start("test2.txt", true) << std::endl;
    std::cout << message_start("test3.txt", true) << std::endl;
    std::cout << message_start("test4.txt", true) << std::endl;
    std::cout << message_start("test5.txt", true) << std::endl;
    std::cout << message_start("input.txt", true) << std::endl;
    std::cout << message_start("test1.txt", false) << std::endl;
    std::cout << message_start("test2.txt", false) << std::endl;
    std::cout << message_start("test3.txt", false) << std::endl;
    std::cout << message_start("test4.txt", false) << std::endl;
    std::cout << message_start("test5.txt", false) << std::endl;
    std::cout << message_start("input.txt", false) << std::endl;
}