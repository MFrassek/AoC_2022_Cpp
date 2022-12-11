#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>

class Monkey {
    public:
        int inspection_count = 0;
        std::queue<long> items;
        char inspection_operator;
        int inspection_modifier;
        int divisibility_test_number;
        Monkey* pass_target;
        Monkey* fail_target;

        Monkey(std::queue<long> items, char inspection_operator, int inspection_modifier, int divisibility_test_number) {
            this->items = items;
            this->inspection_operator = inspection_operator;
            this->inspection_modifier = inspection_modifier;
            this->divisibility_test_number = divisibility_test_number;
        }
        void pass_front_item(long stress_management_number, bool reduce_stress_normally) {
            this->inspection_count ++;
            long item = this->items.front();
            this->items.pop();
            if (inspection_operator == '+') {
                item += inspection_modifier;
            } else if (inspection_operator == '*') {
                item *= inspection_modifier;
            } else if (inspection_operator == '^') {
                item *= item;
            }
            if (reduce_stress_normally) {
                item /= 3;
            } else {
                item %= stress_management_number;
            }
            if (item % this->divisibility_test_number == 0) {
                pass_target->items.push(item);
            } else {
                fail_target->items.push(item);
            }

        }
        void add_pass_target(Monkey* pass_target) {
            this->pass_target = pass_target;
        }
        void add_fail_target(Monkey* fail_target) {
            this->fail_target = fail_target;
        }
};


class MonkeyGang {
    public:
        std::vector<Monkey*> members;

        MonkeyGang(std::string file_name) {
            std::ifstream input(file_name);
            std::string line;
            std::vector<std::string> lines;
            while(std::getline(input, line)) {
                lines.push_back(line);
            }
            for (int i = 0; i < (lines.size() + 1) / 7; i++) {
                std::queue<long> items;
                std::stringstream item_line(lines[i*7+1].substr(18, lines[i*7+1].length()-18));
                std::string element;
                while (std::getline(item_line, element, ',')) {
                    items.push(stoi(element));
                }
                char inspection_operator;
                int inspection_modifier;
                if (lines[i*7+2].substr(25, 3) == "old") {
                    inspection_operator = '^';
                    inspection_modifier = 0;
                } else {
                    inspection_operator = lines[i*7+2].at(23);
                    inspection_modifier = std::stoi(lines[i*7+2].substr(25, 2));
                }
                int divisibility_test_number = std::stoi(lines[i*7+3].substr(21, lines[i*7+3].length()-21));
                this->members.push_back(new Monkey(items, inspection_operator, inspection_modifier, divisibility_test_number));
            }
            for (int i = 0; i < (lines.size() + 1) / 7; i++) {
                int pass_number = std::stoi(lines[i*7+4].substr(29, 1));
                this->members[i]->add_pass_target(this->members[pass_number]);
                int fail_number = std::stoi(lines[i*7+5].substr(30, 1));
                this->members[i]->add_fail_target(this->members[fail_number]);
            }
        }
        long divisibility_test_number_product() {
            long product = 1;
            for (Monkey* monkey : this->members) {
                product *= monkey->divisibility_test_number;
            }
            return product;
        }
        void execute_round(bool reduce_stress_normally) {
            for (int i = 0; i < this->members.size(); i++) {
                while (!this->members[i]->items.empty()) {
                this->members[i]->pass_front_item(this->divisibility_test_number_product(), reduce_stress_normally);
                }
            }
        }
        long monkey_business() {
            std::vector<long> inspection_counts;
            for (Monkey* monkey : this->members) {
                inspection_counts.push_back(monkey->inspection_count);
            }
            std::sort(inspection_counts.begin(), inspection_counts.end());
            return inspection_counts[inspection_counts.size() - 1] * inspection_counts[inspection_counts.size() - 2];
        }
};

long monkey_business(std::string file_name, int rounds, bool reduce_stress_normally) {
    MonkeyGang gang = MonkeyGang(file_name);
    for (int i = 0; i < rounds; i++) {
        gang.execute_round(reduce_stress_normally);
    }
    return gang.monkey_business();
}

int main() {
    std::cout << monkey_business("test1.txt", 20, true) << std::endl;
    std::cout << monkey_business("test1.txt", 10000, false) << std::endl;
    std::cout << monkey_business("input.txt", 20, true) << std::endl;
    std::cout << monkey_business("input.txt", 10000, false) << std::endl;
}