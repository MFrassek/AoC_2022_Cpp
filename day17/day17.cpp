#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

class Boulder {
    public:
        int x = 2;
        int y;
        int height;
        char type;


        Boulder(long int highest_y, char type) {
            this->y = highest_y + 3;
            this->type = type;
            if (type == '-') {
                this->height = 1;
            } else if (type == '.') {
                this->height == 2;
            } else if (type == '+' | type == 'L') {
                this->height == 3;
            } else {
                this->height == 4;
            }
        }

        std::vector<std::tuple<int, int>> problematic_points_if_left() {
            std::vector<std::tuple<int, int>> points;
            if (type == '-') {
                points.push_back({x - 1, y});
            } else if (type == '+') {
                points.push_back({x, y});
                points.push_back({x - 1, y + 1});
                points.push_back({x, y + 2});
            } else if (type == 'L') {
                points.push_back({x - 1, y});
                points.push_back({x + 1, y +1});
                points.push_back({x + 1, y + 2});
            } else if (type == 'I') {
                points.push_back({x - 1, y});
                points.push_back({x - 1, y + 1});
                points.push_back({x - 1, y + 2});
                points.push_back({x - 1, y + 3});
            } else if (type == '.') {
                points.push_back({x - 1, y});
                points.push_back({x - 1, y + 1});
            }
            return points;
        }

        void move_left() {
            x --;
        }

        std::vector<std::tuple<int, int>> problematic_points_if_right() {
            std::vector<std::tuple<int, int>> points;
            if (type == '-') {
                points.push_back({x + 4, y});
            } else if (type == '+') {
                points.push_back({x + 2, y});
                points.push_back({x + 3, y + 1});
                points.push_back({x + 2, y + 2});
            } else if (type == 'L') {
                points.push_back({x + 3, y});
                points.push_back({x + 3, y +1});
                points.push_back({x + 3, y + 2});
            } else if (type == 'I') {
                points.push_back({x + 1, y});
                points.push_back({x + 1, y + 1});
                points.push_back({x + 1, y + 2});
                points.push_back({x + 1, y + 3});
            } else if (type == '.') {
                points.push_back({x + 2, y});
                points.push_back({x + 2, y + 1});
            }
            return points;
        }

        void move_right() {
            x ++;
        }

        std::vector<std::tuple<int, int>> problematic_points_if_down() {
            std::vector<std::tuple<int, int>> points;
            if (type == '-') {
                points.push_back({x, y - 1});
                points.push_back({x + 1, y - 1});
                points.push_back({x + 2, y - 1});
                points.push_back({x + 3, y - 1});
            } else if (type == '+') {
                points.push_back({x, y});
                points.push_back({x + 1, y - 1});
                points.push_back({x + 2, y});
            } else if (type == 'L') {
                points.push_back({x, y - 1});
                points.push_back({x + 1, y - 1});
                points.push_back({x + 2, y - 1});
            } else if (type == 'I') {
                points.push_back({x, y - 1});
            } else if (type == '.') {
                points.push_back({x, y - 1});
                points.push_back({x + 1, y - 1});
            }
            return points;
        }

        void move_down() {
            y --;
        }

        std::vector<std::tuple<int, int>> points() {
            std::vector<std::tuple<int, int>> points;
            if (type == '-') {
                points.push_back({x, y});
                points.push_back({x + 1, y});
                points.push_back({x + 2, y});
                points.push_back({x + 3, y});
            } else if (type == '+') {
                points.push_back({x, y + 1});
                points.push_back({x + 1, y});
                points.push_back({x + 1, y + 1});
                points.push_back({x + 1, y + 2});
                points.push_back({x + 2, y + 1});
            } else if (type == 'L') {
                points.push_back({x, y});
                points.push_back({x + 1, y});
                points.push_back({x + 2, y});
                points.push_back({x + 2, y + 1});
                points.push_back({x + 2, y + 2});
            } else if (type == 'I') {
                points.push_back({x, y});
                points.push_back({x, y + 1});
                points.push_back({x, y + 2});
                points.push_back({x, y + 3});
            } else if (type == '.') {
                points.push_back({x, y});
                points.push_back({x + 1, y});
                points.push_back({x, y + 1});
                points.push_back({x + 1, y + 1});
            }
            return points;
        }
};

class Tower {
    public:
        std::vector<std::vector<bool>> rows;
        std::string wind_sequence;
        int next_wind_index = 0;
        std::string boulder_sequence = "-+LI.";
        int next_boulder_index = 0;
        long long int height = 0;
        long int boulder_count = 0;
        int jet_completion_count = 0;
        char cycle_start_type;
        int cycle_count = 0;
        long int height_at_cycle_start;
        long int height_at_cycle_end;
        long int boulder_count_at_cycle_start;
        long int boulder_count_at_cycle_end;
        long int boulders_gained_per_cycle = 0;
        long int height_gained_per_cycle = 0;

        Tower(std::string wind_sequence) {
            this->wind_sequence = wind_sequence;
        }
        char current_boulder_type() {
            return boulder_sequence.at(next_boulder_index);
        }

        char next_boulder_type() {
            char type = boulder_sequence.at(next_boulder_index);
            next_boulder_index ++;
            next_boulder_index %= 5;
            return type;
        }

        char next_jet() {
            char jet = wind_sequence.at(next_wind_index);
            next_wind_index ++;
            next_wind_index %= wind_sequence.length();
            if (next_wind_index == 0 && boulders_gained_per_cycle == 0) {
                if (this->jet_completion_count == 0) {
                    this->cycle_start_type = this->current_boulder_type();
                } else if (this->cycle_start_type == this->current_boulder_type() ) {
                    if (this->cycle_count == 1) {
                        this->height_at_cycle_start = this->height;
                        this->boulder_count_at_cycle_start = this->boulder_count;
                    } else if (this->cycle_count == 2) {
                        this->height_at_cycle_end = this->height;
                        this->boulder_count_at_cycle_end = this->boulder_count;
                        this->boulders_gained_per_cycle = boulder_count_at_cycle_end - boulder_count_at_cycle_start;
                        this->height_gained_per_cycle = height_at_cycle_end - height_at_cycle_start;
                    }
                    this->cycle_count ++;
                }
                this->jet_completion_count ++;
            }
            return jet;
        }

        void print() {
            for (long int i = height; i >= 0; i--) {
                for (int j = 0; j < 7; j ++) {
                    if (rows[i][j]) {
                        std::cout << "#";
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        bool can_boulder_move_left(Boulder* boulder) {
            for (std::tuple<int, int> point : boulder->problematic_points_if_left()) {
                int x = std::get<0>(point);
                int y = std::get<1>(point);
                if (x < 0 || rows[y][x]) {
                    return false;
                }
            }
            return true;
        }

        bool can_boulder_move_right(Boulder* boulder) {
            for (std::tuple<int, int> point : boulder->problematic_points_if_right()) {
                int x = std::get<0>(point);
                int y = std::get<1>(point);
                if (x >= 7 || rows[y][x]) {
                    return false;
                }
            }
            return true;
        }


        bool can_boulder_move_down(Boulder* boulder) {
            for (std::tuple<int, int> point : boulder->problematic_points_if_down()) {
                int x = std::get<0>(point);
                int y = std::get<1>(point);
                if (y < 0 || rows[y][x]) {
                    return false;
                }
            }
            return true;
        }

        void drop_boulder() {
            Boulder boulder = Boulder(height, this->next_boulder_type());
            std::vector<bool> row(7, false);
            for (int i = rows.size(); i <= boulder.y + boulder.height + 4; i++) {
                rows.push_back(row);
            }
            while (true) {
                if (this->next_jet() == '<') {
                    if (can_boulder_move_left(&boulder)) {
                        boulder.move_left();
                    }
                } else {
                    if (can_boulder_move_right(&boulder)) {
                        boulder.move_right();
                    }
                }
                if (can_boulder_move_down(&boulder)) {
                    boulder.move_down();
                } else {
                    for (std::tuple<int, int> point : boulder.points()) {
                        int x = std::get<0>(point);
                        int y = std::get<1>(point);
                        rows[y][x] = true;
                        if (y + 1 > this->height) {
                            this->height = y + 1;
                        }
                    }
                    break;
                }
            }
            this->boulder_count ++;
        }
};


long long int tower_height(std::string file_name, long int iterations) {
    std::ifstream input(file_name);
    std::string line;
    std::getline(input, line);
    Tower tower = Tower(line);
    long long int height_bonus = 0;
    long long int remainder = 0;
    for (long long int i = 0; i < iterations; i++) {
        tower.drop_boulder();
        remainder --;
        if (remainder == 0) {
            break;
        }
        if (tower.height_gained_per_cycle != 0 && height_bonus == 0) {
            long int boulders_missing = iterations - i;
            long int full_cycles_needed = boulders_missing / tower.boulders_gained_per_cycle;
            height_bonus = full_cycles_needed * tower.height_gained_per_cycle;
            remainder = boulders_missing - full_cycles_needed * tower.boulders_gained_per_cycle;
        }
    }
    return tower.height + height_bonus - 1;
}

int main() {
    std::cout << tower_height("test1.txt", 1000000000000) << std::endl;
    std::cout << tower_height("input.txt", 1000000000000) << std::endl;

}