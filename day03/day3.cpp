#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>

int overlapping_item_priority_sum(std::string file_name, bool per_rucksack) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> lines;
    int total = 0;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    for (int i = 0; i < lines.size(); i++) {
        std::vector<char> overlap;
        if (per_rucksack) {
            std::string first_inventory = lines[i].substr(0, lines[i].length()/2);
            std::string second_inventory = lines[i].substr(lines[i].length()/2, lines[i].length());
            std::set<char> first_inventory_unique(begin(first_inventory), end(first_inventory));
            std::set<char> second_inventory_unique(begin(second_inventory), end(second_inventory));
            std::set_intersection(
                first_inventory_unique.begin(), first_inventory_unique.end(),
                second_inventory_unique.begin(), second_inventory_unique.end(),
                std::inserter(overlap, overlap.begin()));
        } else {
            if (i % 3 == 0) {
                std::string first_inventory = lines[i];
                std::string second_inventory = lines[i+1];
                std::string third_inventory = lines[i+2];
                std::set<char> first_inventory_unique(begin(first_inventory), end(first_inventory));
                std::set<char> second_inventory_unique(begin(second_inventory), end(second_inventory));
                std::set<char> third_inventory_unique(begin(third_inventory), end(third_inventory));
                std::set<char> first_and_second_overlap;
                std::set_intersection(
                    first_inventory_unique.begin(), first_inventory_unique.end(),
                    second_inventory_unique.begin(), second_inventory_unique.end(),
                    std::inserter(first_and_second_overlap,
                    first_and_second_overlap.begin()));
                std::set_intersection(
                    first_and_second_overlap.begin(), first_and_second_overlap.end(),
                    third_inventory_unique.begin(), third_inventory_unique.end(),
                    std::inserter(overlap, overlap.begin()));
            }
        }
        for (char const& character : overlap) {
            if (isupper(character)) {
                total += int(character) - int('A') + 27;
            } else {
                total += int(character) - int('a') + 1;
            }
        }
    }
    return total;
}

int main()
{   
    std::cout << overlapping_item_priority_sum("test1.txt", true) << std::endl;
    std::cout << overlapping_item_priority_sum("input.txt", true) << std::endl;
    std::cout << overlapping_item_priority_sum("test1.txt", false) << std::endl;
    std::cout << overlapping_item_priority_sum("input.txt", false) << std::endl;
}

