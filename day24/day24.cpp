#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <queue>

class Blizzard {
    public:
        int row;
        int col;
        int orientation;
        int map_height;
        int map_width;

        Blizzard(int row, int col, char orientation, int map_height, int map_width) {
            this->row = row;
            this->col = col;
            this->orientation = orientation;
            this->map_height = map_height;
            this->map_width = map_width;
        }

        void step() {
            if (this->orientation == '>') {
                this->col = (this->col + 1) % this->map_width;
            } else if (this->orientation == '<') {
                this->col = (this->col - 1 + this->map_width) % this->map_width;
            } else if (this->orientation == 'v') {
                this->row = (this->row + 1) % this->map_height;
            } else if (this->orientation == '^') {
                this->row = (this->row - 1 + this->map_height) % this->map_height;
            }
        }
};

class BlizzardMap {
    public:
        int height;
        int width;
        std::vector<Blizzard*> blizzards;
        int rounds = 0;

        BlizzardMap(std::vector<std::string> rows) {
            this->height = rows.size() - 2;
            this->width = rows[0].length() - 2;
            for (int r = 0; r < this->height; r++) {
                for (int c = 0; c < this->width; c++) {
                    if (rows[r + 1].at(c + 1) != '.') {
                        this->blizzards.push_back(new Blizzard(r, c, rows[r + 1].at(c + 1), this->height, this->width));
                    }
                }
            }

        }
        void step() {
            for (Blizzard* blizzard : this->blizzards) {
                blizzard->step();
            }
            this->rounds++;
        }

        std::vector<std::vector<bool>> free_spaces() {
            std::vector<std::vector<bool>> free_spaces;
            for (int r = 0; r < this->height; r++) {
                std::vector<bool> row;
                for (int c = 0; c < this->width; c++) {
                    row.push_back(true);
                }
                free_spaces.push_back(row);
            }
            for (Blizzard* blizzard : this->blizzards) {
                free_spaces[blizzard->row][blizzard->col] = false;
            }
            return free_spaces;
        }
};

void traverse(BlizzardMap &map, std::tuple<int, int> start_point, std::tuple<int, int> end_point) {
    std::queue<std::tuple<int, int, int>> queue;
    queue.push({std::get<0>(start_point), std::get<1>(start_point), map.rounds});
    while (true) {
        map.step();
        std::vector<std::vector<bool>> free_spaces = map.free_spaces();
        while (std::get<2>(queue.front()) == map.rounds - 1) {
            int row = std::get<0>(queue.front());
            int col = std::get<1>(queue.front());
            if ((row  == std::get<0>(start_point) && col == std::get<1>(start_point)) 
                    || (row == std::get<0>(end_point) && col == std::get<0>(end_point)) 
                    || free_spaces[row][col]) {
                queue.push({row, col, map.rounds});
                if (!((row  == std::get<0>(start_point) && col == std::get<1>(start_point)) 
                        || (row == std::get<0>(end_point) && col == std::get<0>(end_point)))) {
                    free_spaces[row][col] = false;
                }
            }
            if (row - 1 >= 0 && free_spaces[row - 1][col]) {
                queue.push({row - 1, col, map.rounds});
                free_spaces[row - 1][col] = false;
            }
            if (row + 1 < map.height && free_spaces[row + 1][col]) {
                queue.push({row + 1, col, map.rounds});
                free_spaces[row + 1][col] = false;
            }
            if (row != -1 && row != map.height && col - 1 >= 0 && free_spaces[row][col - 1]) {
                queue.push({row, col - 1, map.rounds});
                free_spaces[row][col - 1] = false;
            }
            if (row != -1 && col + 1 < map.width && free_spaces[row][col + 1]) {
                queue.push({row, col + 1, map.rounds});
                free_spaces[row][col + 1] = false;
            }
            if (row - 1 == -1 && col == 0) {
                queue.push({row - 1, col , map.rounds});
            }
            if ((row + 1 == std::get<0>(end_point) && col == std::get<1>(end_point)) ||
                    (row - 1 == std::get<0>(end_point) && col == std::get<1>(end_point))) {
                return;
            }
            queue.pop();
        }
    }
}

int minimal_traversal_time(std::string file_name, bool triple_trip) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> grid;
    while (std::getline(input, line)) {
        grid.push_back(line);
    }
    BlizzardMap map = BlizzardMap(grid);
    traverse(map, {-1, 0}, {map.height, map.width - 1});
    if (triple_trip) {
        traverse(map, {map.height, map.width - 1}, {-1, 0});
        traverse(map, {-1, 0}, {map.height, map.width - 1});
    }
    return map.rounds;
}



int main() {
    std::cout << minimal_traversal_time("test1.txt", false) << std::endl;
    std::cout << minimal_traversal_time("test2.txt", false) << std::endl;
    std::cout << minimal_traversal_time("input.txt", false) << std::endl;
    std::cout << minimal_traversal_time("test1.txt", true) << std::endl;
    std::cout << minimal_traversal_time("test2.txt", true) << std::endl;
    std::cout << minimal_traversal_time("input.txt", true) << std::endl;
}