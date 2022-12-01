#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int calories_of_top_n_elves(std::string file_name, int n) {
    std::vector<int> calorie_totals;
    std::ifstream input(file_name);
    std::string line;
    int temp = 0; 
    while (std::getline(input, line)) {
        if (line.size() > 0) {
            temp += std::stoi(line);

        } else {
            calorie_totals.push_back(temp);
            temp = 0;
        }        
    }
    calorie_totals.push_back(temp);
    std::sort(calorie_totals.begin(), calorie_totals.end());
    int top_elf_calories = 0;
    for (int i = 1; i <= n && i <= calorie_totals.size(); i++) {
        top_elf_calories += calorie_totals[calorie_totals.size() - i];
    }
    return top_elf_calories;
}

int main()
{   
    std::cout << calories_of_top_n_elves("test1.txt", 1) << std::endl;
    std::cout << calories_of_top_n_elves("test1.txt", 3) << std::endl;
    std::cout << calories_of_top_n_elves("input.txt", 1) << std::endl;
    std::cout << calories_of_top_n_elves("input.txt", 3) << std::endl;
}

