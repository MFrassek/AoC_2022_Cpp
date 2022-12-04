#include <iostream>
#include <fstream>
#include <string>

int range_overlap_count(std::string file_name, bool full_overlap) {
    std::ifstream input(file_name);
    std::string line;
    int count = 0;
    while (std::getline(input, line)) {
        int comma_index = line.find(',');
        int first_dash_index = line.find('-');
        int second_dash_index = line.find('-', comma_index);
        int start_1 = std::stoi(line.substr(0, first_dash_index));
        int end_1 = std::stoi(line.substr(first_dash_index + 1, comma_index));
        int start_2 = std::stoi(line.substr(comma_index + 1, second_dash_index));
        int end_2 = std::stoi(line.substr(second_dash_index + 1, line.length()));
        if (full_overlap) {
            if ((start_1 <= start_2 & end_1 >= end_2) 
                | (start_2 <= start_1 & end_2 >= end_1)) {
                count ++;
            }
        } else {
            if (!(end_1 < start_2 | end_2 < start_1)) {
                count ++;
            }
        }
    }
    return count;
}

int main()
{   
    std::cout << range_overlap_count("test1.txt", true) << std::endl;
    std::cout << range_overlap_count("test1.txt", false) << std::endl;
    std::cout << range_overlap_count("input.txt", true) << std::endl;
    std::cout << range_overlap_count("input.txt", false) << std::endl;
}

