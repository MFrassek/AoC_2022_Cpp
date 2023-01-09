#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <stdexcept>
#include <unordered_set>

class Elf {
    public:
        int row;
        int col;
        std::tuple<int, int> last_suggested_destination;

        Elf(int row, int col) {
            this->row = row;
            this->col = col;
            this->last_suggested_destination = {row, col};
        }

        bool wants_to_move(std::set<std::tuple<int, int>>& other_elf_positions) {
            if (other_elf_positions.find({this->row - 1, this->col - 1}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row - 1, this->col}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row - 1, this->col + 1}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row, this->col - 1}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row, this->col + 1}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row + 1, this->col - 1}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row + 1, this->col}) == other_elf_positions.end()
                    && other_elf_positions.find({this->row + 1, this->col + 1}) == other_elf_positions.end()) {
                this->last_suggested_destination = {this->row, this->col};
                return false;
            } else  if ((other_elf_positions.find({this->row - 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row - 1, this->col}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row - 1, this->col + 1}) == other_elf_positions.end())
                        || (other_elf_positions.find({this->row + 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col + 1}) == other_elf_positions.end())
                        || (other_elf_positions.find({this->row - 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col - 1}) == other_elf_positions.end())
                        || (other_elf_positions.find({this->row - 1, this->col + 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row, this->col + 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col + 1}) == other_elf_positions.end())) {
                return true;
            } else {
                this->last_suggested_destination = {this->row, this->col};
                return false;
            }
        }
        std::tuple<int, int> suggested_destination(std::set<std::tuple<int, int>>& other_elf_positions, int initial_direction) {
            for (int i = 0; i < 4; i++) {
                if ((initial_direction + i) % 4 == 0) {
                    if (other_elf_positions.find({this->row - 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row - 1, this->col}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row - 1, this->col + 1}) == other_elf_positions.end()) {
                        this->last_suggested_destination = {this->row - 1, this->col};
                        return this->last_suggested_destination;
                    }
                } else if ((initial_direction + i) % 4 == 1) {
                    if (other_elf_positions.find({this->row + 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col + 1}) == other_elf_positions.end()) {
                        this->last_suggested_destination = {this->row + 1, this->col};
                        return this->last_suggested_destination;
                    }
                } else if ((initial_direction + i) % 4 == 2) {
                    if (other_elf_positions.find({this->row - 1, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row, this->col - 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col - 1}) == other_elf_positions.end()) {
                        this->last_suggested_destination = {this->row , this->col - 1};
                        return this->last_suggested_destination;
                    }
                } else if ((initial_direction + i) % 4 == 3) {
                    if (other_elf_positions.find({this->row - 1, this->col + 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row, this->col + 1}) == other_elf_positions.end()
                            && other_elf_positions.find({this->row + 1, this->col + 1}) == other_elf_positions.end()) {
                        this->last_suggested_destination = {this->row, this->col + 1};
                        return this->last_suggested_destination;
                    }
                }
            }
            this->last_suggested_destination = {this->row, this->col};
            throw;
        }
        
        void move(std::set<std::tuple<int, int>>& invalid_suggestions) {
            if (invalid_suggestions.find(this->last_suggested_destination) == invalid_suggestions.end()) {
                this->row = std::get<0>(this->last_suggested_destination);
                this->col = std::get<1>(this->last_suggested_destination);
            }
        }
};

class ElfColony {
    public:
        int round_count = 0;
        std::vector<Elf*> elves;
    
        ElfColony(std::vector<std::string> grid) {
            for (int r = 0; r < grid.size(); r++) {
                for (int c = 0; c < grid[r].length(); c++) {
                    if (grid[r].at(c) == '#') {
                        elves.push_back(new Elf(r, c));
                    }
                }
            }
        }

        std::set<std::tuple<int, int>> elf_positions() {
            std::set<std::tuple<int, int>> positions;
            for (Elf* elf : this->elves) {
                positions.insert({elf->row, elf->col});
            }
            return positions;
        }
        std::set<std::tuple<int, int>> invalid_suggestions() {
            std::set<std::tuple<int, int>> elf_positions = this->elf_positions();
            std::set<std::tuple<int, int>> suggested_destinations;
            std::set<std::tuple<int, int>> invalid_destinations;
            for (Elf* elf : this->elves) {
                if (elf->wants_to_move(elf_positions)) {
                    try {
                        std::tuple<int, int> suggested_destination = elf->suggested_destination(elf_positions, round_count % 4);
                        if (suggested_destinations.find(suggested_destination) == suggested_destinations.end()) {
                            suggested_destinations.insert(suggested_destination);
                        } else {
                            invalid_destinations.insert(suggested_destination);
                        }
                    } catch ( ... ) {

                    }
                }
            }
            return invalid_destinations;
        }

        void step() {
            auto invalid_suggestions = this->invalid_suggestions();
            for (Elf* elf : this->elves) {
                elf->move(invalid_suggestions);
            }
            this->round_count++;

        }

        int empty_spaces() {
            int lowest_r = this->elves[0]->row;
            int lowest_c = this->elves[0]->col;
            int highest_r = this->elves[0]->row;
            int highest_c = this->elves[0]->col;
            for (Elf* elf : this->elves) {
                if (elf->row < lowest_r) {
                    lowest_r = elf->row;
                }
                if (elf->row > highest_r) {
                    highest_r = elf->row;
                }
                if (elf->col < lowest_c) {
                    lowest_c = elf->col;
                }
                if (elf->col > highest_c) {
                    highest_c = elf->col;
                }                
            }
            int count = 0;
            int grid_size = (highest_r - lowest_r + 1) * (highest_c - lowest_c + 1);
            return grid_size - this->elves.size();
        }

        bool any_elf_wants_to_move() {
            std::set<std::tuple<int, int>> elf_positions = this->elf_positions();
            for (Elf* elf : this->elves) {
                if (elf->wants_to_move(elf_positions)) {
                    return true;
                }
            }
            return false;
        }
};

int empty_ground_tiles(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> grid;
    while (std::getline(input, line)) {
        grid.push_back(line);
    }
    ElfColony colony = ElfColony(grid);
    for (int i = 0; i < 10; i++) {
        colony.step();
    }
    return colony.empty_spaces();
}

int rounds_til_standstill(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> grid;
    while (std::getline(input, line)) {
        grid.push_back(line);
    }
    ElfColony colony = ElfColony(grid);
    while (colony.any_elf_wants_to_move()) {
        colony.step();
    }
    return colony.round_count + 1;
}

int main() {
    std::cout << empty_ground_tiles("test1.txt") << std::endl;
    std::cout << empty_ground_tiles("test2.txt") << std::endl;
    std::cout << empty_ground_tiles("input.txt") << std::endl;
    std::cout << rounds_til_standstill("test1.txt") << std::endl;
    std::cout << rounds_til_standstill("test2.txt") << std::endl;
    std::cout << rounds_til_standstill("input.txt") << std::endl;
}