#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class Monkey {
    public:
        long long int number = 0;
        std::string pre_monkey_1_name = "";
        std::string pre_monkey_2_name = "";
        char operation = ' ';
        std::map<std::string, Monkey*>* monkey_names;
        std::map<std::string, std::string>* parent_names; 

        Monkey(std::string line, std::map<std::string, Monkey*>* monkey_names, std::map<std::string, std::string>* parent_names) {
            this->monkey_names = monkey_names;
            this->parent_names = parent_names;
            std::string monkey_name = line.substr(0, 4);
            std::string job = line.substr(6, line.length() - 6);
            int first_space_index = job.find(' ');
            if (first_space_index == std::string::npos) {
                this->number = std::stoll(job);
            } else {
                int second_space_index = job.find(' ', first_space_index + 1);
                this->pre_monkey_1_name = job.substr(0, first_space_index);
                this->pre_monkey_2_name = job.substr(second_space_index + 1, job.length() - second_space_index);
                this->operation = job.at(first_space_index + 1);
                (*parent_names)[pre_monkey_1_name] = monkey_name;
                (*parent_names)[pre_monkey_2_name] = monkey_name;
            }
            (*monkey_names)[monkey_name] = this;
        }

        long long int job_result() {
            if (this->operation == '+') {
                this->number = (*this->monkey_names)[this->pre_monkey_1_name]->job_result() + (*this->monkey_names)[this->pre_monkey_2_name]->job_result();
            } else if (this->operation == '-') {
                this->number = (*this->monkey_names)[this->pre_monkey_1_name]->job_result() - (*this->monkey_names)[this->pre_monkey_2_name]->job_result();
            } else if (this->operation == '*') {
                this->number = (*this->monkey_names)[this->pre_monkey_1_name]->job_result() * (*this->monkey_names)[this->pre_monkey_2_name]->job_result();
            } else if (this->operation == '/') {
                this->number = (*this->monkey_names)[this->pre_monkey_1_name]->job_result() / (*this->monkey_names)[this->pre_monkey_2_name]->job_result();
            }
            return this->number;
        }
};

long long int root_number(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::map<std::string, Monkey*> monkey_names;
    std::map<std::string, std::string> parent_names;
    while (std::getline(input, line)) {   
        Monkey* monkey = new Monkey(line, &monkey_names, &parent_names);
    }
    return monkey_names["root"]->job_result();
}

long long int human_number(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::map<std::string, Monkey*> monkey_names;
    std::map<std::string, std::string> parent_names;
    while (std::getline(input, line)) {   
        Monkey* monkey = new Monkey(line, &monkey_names, &parent_names);
    }
    std::string current_name = "humn";
    std::vector<std::string> name_stack;
    while (current_name != "root") {
        name_stack.push_back(current_name);
        current_name = parent_names[current_name];
    }
    monkey_names["root"]->job_result();
    long long int target_number = monkey_names[(monkey_names["root"]->pre_monkey_1_name == name_stack.back() ? monkey_names["root"]->pre_monkey_2_name : monkey_names["root"]->pre_monkey_1_name)]->job_result();
    std::string current_monkey_name;
    std::string next_monkey_name;
    while (next_monkey_name != "humn") {
        current_monkey_name = name_stack.back();
        name_stack.pop_back();
        next_monkey_name = name_stack.back();
        bool is_first = monkey_names[current_monkey_name]->pre_monkey_1_name == next_monkey_name;
        long long int other_number = monkey_names[(is_first ? monkey_names[current_monkey_name]->pre_monkey_2_name : monkey_names[current_monkey_name]->pre_monkey_1_name)]->job_result();
        char current_operation = monkey_names[current_monkey_name]->operation;
        if (current_operation == '+') {
            target_number -= other_number; 
        } else if (current_operation == '-') {
            if (is_first) {
                target_number += other_number;
            } else {
                target_number = other_number - target_number;
            }
        } else if (current_operation == '*') {
            target_number /= other_number;
        } else if (current_operation == '/') {
            if (is_first) {
                target_number *= other_number;
            } else {
                target_number = other_number / target_number;
            }
        }
    }
    return target_number;
}

int main() {
    std::cout << root_number("test1.txt") << std::endl;
    std::cout << root_number("input.txt") << std::endl;
    std::cout << human_number("test1.txt") << std::endl;
    std::cout << human_number("input.txt") << std::endl;
}
