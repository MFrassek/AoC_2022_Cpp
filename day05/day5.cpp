#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<char>> initial_stacks(std::vector<std::string> lines) {
    int stack_count =  (lines[0].length() + 1) / 4;
    std::vector<std::vector<char>> stacks;
    for (int i = 0; i < stack_count; i++) {
        stacks.push_back(std::vector<char>());
    } 
    for (int i = lines.size() - 2; i >= 0; i --) {
        for (int j = 0; j < stack_count; j++) {
            char crate = lines[i].at(j * 4 + 1);
            if (crate != ' ') {
                stacks[j].push_back(crate);
            }
        } 
    }
    return stacks;
}

std::vector<std::vector<int>> moving_instructions(std::vector<std::string> instruction_lines) {
    std::vector<std::vector<int>> instructions = {};
    for (auto &line : instruction_lines) {
        instructions.push_back(std::vector<int>());
        int first_space_index = line.find(" ");
        int second_space_index = line.find(" ", first_space_index + 1);
        int third_space_index = line.find(" ", second_space_index + 1);
        int fourth_space_index = line.find(" ", third_space_index + 1);
        int fifth_space_index = line.find(" ", fourth_space_index + 1);
        int sixth_space_index = line.find(" ", fifth_space_index + 1);
        int n_to_move = std::stoi(line.substr(first_space_index + 1, second_space_index - first_space_index));
        int from_stack = std::stoi(line.substr(third_space_index + 1, fourth_space_index - third_space_index)) - 1;
        int to_stack = std::stoi(line.substr(fifth_space_index + 1, sixth_space_index - fifth_space_index)) - 1;
        instructions[instructions.size() - 1].push_back(n_to_move);
        instructions[instructions.size() - 1].push_back(from_stack);
        instructions[instructions.size() - 1].push_back(to_stack);
    }
    return instructions;
}

std::vector<std::vector<char>> rearrange_stacks(
        std::vector<std::vector<char>> stacks, std::vector<std::vector<int>> instructions, bool retain_order) {
    for (int i = 0; i < instructions.size(); i++) {
        int n_to_move = instructions[i][0];
        int from_stack = instructions[i][1];
        int to_stack = instructions[i][2];
        if (!retain_order) {
            for (int j = 0; j < n_to_move; j++) {
                stacks[to_stack].push_back(stacks[from_stack][stacks[from_stack].size() - 1]);
                stacks[from_stack].pop_back();
            }
        } else {
            for (int j = n_to_move - 1; j >= 0; j--) {
                stacks[to_stack].push_back(stacks[from_stack][stacks[from_stack].size() - 1 - j]);
            }
            for (int j = 0; j < n_to_move; j++) {
                stacks[from_stack].pop_back();
            }
        }
    }
    return stacks;
}

std::string top_crate_names(std::string file_name, bool retain_order) {
    std::ifstream input(file_name);
    std::vector<std::string> stack_lines;
    std::vector<std::string> instruction_lines;
    std::string line;
    bool before_blank_line = true;
    while (std::getline(input, line)) {
        if (before_blank_line) {
            if (line == "") {
                before_blank_line = false;
            } else {
                stack_lines.push_back(line);
            }
        } else {
            instruction_lines.push_back(line);
        } 
    }
    auto stacks = initial_stacks(stack_lines);
    auto instructions = moving_instructions(instruction_lines);
    stacks = rearrange_stacks(stacks, instructions, retain_order);
    std::string output = "";
    for (int i = 0; i < stacks.size(); i++) {
        output += stacks[i][stacks[i].size() - 1];
    }
    return output;
}

int main() {   
    std::cout << top_crate_names("test1.txt", false) << std::endl;
    std::cout << top_crate_names("test1.txt", true) << std::endl;
    std::cout << top_crate_names("input.txt", false) << std::endl;
    std::cout << top_crate_names("input.txt", true) << std::endl;
}
