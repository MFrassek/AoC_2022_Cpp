#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <array>

class SignalCycle {
    public:
        int x = 1;
        int cycle = 1;
        std::map<int, int> signal_strengths;
        std::array<std::array<char, 40>, 6> drawing;

        void draw() {
            int row = cycle / 40;
            int col = (cycle - 1) % 40;
            if (col >= x - 1 & col <= x + 1) {
                drawing[row][col] = '#';
            } else {
                drawing[row][col] = '.';
            }
        }
        void process_instruction(std::string instruction) {
            if (instruction == "noop") {
                draw();
                cycle ++;
                signal_strengths[cycle] = x * cycle;
            } else {
                draw();
                cycle ++;
                signal_strengths[cycle] = x * cycle;
                draw();
                cycle ++;
                int space_index = instruction.find(" ");
                x += std::stoi(instruction.substr(space_index + 1, instruction.length() - space_index - 1));
                signal_strengths[cycle] = x * cycle;
            }
        }
};

int insteresting_signal_strength_sums(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    SignalCycle signal_cycle = SignalCycle();
    while (std::getline(input, line)) {
        signal_cycle.process_instruction(line);
    }
    int total = 0;
    for (int i = 1; i <= signal_cycle.cycle; i ++) {
        if (i % 40 == 20) {
            total += signal_cycle.signal_strengths[i];
        }
    }
    return total;
}
void render(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    SignalCycle signal_cycle = SignalCycle();
    while (std::getline(input, line)) {
        signal_cycle.process_instruction(line);
    }
    for (auto &row : signal_cycle.drawing) {
        for (char &character : row) {
            std::cout << character;
        }
        std::cout << std::endl;
    }
}


int main() {
    std::cout << insteresting_signal_strength_sums("test1.txt") << std::endl;
    render("test1.txt");
    std::cout << insteresting_signal_strength_sums("input.txt") << std::endl;
    render("input.txt");
}