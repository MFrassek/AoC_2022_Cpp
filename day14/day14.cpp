#include <iostream>
#include <fstream>
#include <set>
#include <tuple>
#include <vector>


class Cave {
    public:
        std::set<std::tuple<int, int>> occupied;
        int lowest_y = 0;

        void add_boulder_line(std::tuple<int, int> start, std::tuple<int, int> end) {
            int start_x = std::get<0>(start);
            int start_y = std::get<1>(start);
            int end_x = std::get<0>(end);
            int end_y = std::get<1>(end);
            if (start_x == end_x) {
                if (end_y > start_y) {
                    if (end_y > this->lowest_y) {
                        this->lowest_y = end_y;
                    }
                    for (int y = start_y; y <= end_y; y ++) {
                        occupied.insert({start_x, y});
                    }
                } else {
                    if (start_y > this->lowest_y) {
                        this->lowest_y = start_y;
                    }
                    for (int y = start_y; y >= end_y; y --) {
                       occupied.insert({start_x, y});
                   }

                }
            } else if (start_y == end_y) {
                if (start_y > this->lowest_y) {
                    this->lowest_y = start_y;
                }
                if (end_x > start_x) {
                    for (int x = start_x; x <= end_x; x ++) {
                        occupied.insert({x, start_y});
                    }
                } else {
                    for (int x = start_x; x >= end_x; x --) {
                       occupied.insert({x, start_y});
                   }

                }
            }
        }
        bool sand_dropable(bool is_waiting_for_overflow) {
            int x = 500;
            int y = 0;
            while (is_waiting_for_overflow ? y <= lowest_y : true) {
                if (y + 1 == lowest_y + 2) {
                    this->occupied.insert({x, y});
                    return true;
                } else if (this->occupied.find({x, y + 1}) == this->occupied.end()) {
                    y ++;
                } else if (this->occupied.find({x - 1, y + 1}) == this->occupied.end()) {
                    x --;
                    y ++;
                } else if (this->occupied.find({x + 1, y + 1}) == this->occupied.end()) {
                    x ++;
                    y ++;
                } else if (x != 500 | y != 0) {
                    this->occupied.insert({x, y});
                    return true;
                } else {
                    return false;
                }
            }
            return false;
        }

};

int rounds_till_sand_falls_off(std::string file_name, bool is_waiting_for_overflow) {
    Cave cave = Cave();
    std::ifstream input(file_name);
    std::string line;
    while (std::getline(input, line)) {
        std::vector<std::tuple<int, int>> points;
        int previous_offset = 0;
        int offset = 0;
        while (offset != std::string::npos) {
            offset = line.find(" -> ", previous_offset);
            std::string position = line.substr(previous_offset, offset - previous_offset);
            int pos_x = stoi(position.substr(0, position.find(',')));
            int pos_y = stoi(position.substr(position.find(',') + 1));
            points.push_back({pos_x, pos_y});
            previous_offset = offset + 4;
            }
        for (int i = 0; i < points.size() - 1; i ++) {
            cave.add_boulder_line(
                {std::get<0>(points[i]), std::get<1>(points[i])},
                {std::get<0>(points[i+1]), std::get<1>(points[i+1])});
        }
    }
    int count = 0;
    while (true) {
        if (cave.sand_dropable(is_waiting_for_overflow)) {
            count ++;
        } else {
            return count;
        }
    }
    return -1;
}

int main() {
    std::cout << rounds_till_sand_falls_off("test1.txt", true) << std::endl;
    std::cout << rounds_till_sand_falls_off("input.txt", true) << std::endl;
    std::cout << rounds_till_sand_falls_off("test1.txt", false) + 1 << std::endl;
    std::cout << rounds_till_sand_falls_off("input.txt", false) + 1 << std::endl;
}
