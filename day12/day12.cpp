#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>

std::vector<std::vector<int>> grid(std::string file_name) {
    std::vector<std::vector<int>> heights;
    std::ifstream input(file_name);
    std::string line;
    int row_count = 0;
    std::tuple<int, int> start; 
    std::tuple<int, int> end; 
    while (std::getline(input, line)) {
        std::vector<int> row;
        int col_count = 0;
        for (char &character : line) {
            if (character == 'S') {
                row.push_back(int('a'));
                start = {row_count, col_count};
            } else if (character == 'E') {
                row.push_back(int('z'));
                end = {row_count, col_count};
            } else {
                row.push_back(int(character));
            }
            col_count ++;
        }
        heights.push_back(row);
        row_count ++;
    }
    return heights;
}

int steps_up_start_to_destination(
        std::vector<std::vector<int>> heights, std::vector<std::vector<bool>> scheduled, 
        std::vector<std::vector<int>> distance_from_start,std::tuple<int, int> start, std::tuple<int, int> end) {
    int row_count = heights.size();
    int col_count = heights[0].size();
    std::queue<std::tuple<int, int>> upcoming_visits;
    upcoming_visits.push(start);
    while (!upcoming_visits.empty()) {
        std::tuple<int, int> node = upcoming_visits.front();
        upcoming_visits.pop();
        int r = std::get<0>(node);
        int c = std::get<1>(node);
        if (r > 0) {
            if (!scheduled[r-1][c] & heights[r][c] + 1 >= heights[r-1][c]) {
                scheduled[r-1][c] = true;
                distance_from_start[r-1][c] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r-1, c});
                if (end == std::tuple<int, int> {r-1, c}) {
                    break;
                }
            }
        }
        if (r < row_count - 1) {
            if (!scheduled[r+1][c] & heights[r][c] + 1 >= heights[r+1][c]) {
                scheduled[r+1][c] = true;
                distance_from_start[r+1][c] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r+1, c});
                if (end == std::tuple<int, int> {r+1, c}) {
                    break;
                }
            }
        }
        if (c > 0) {
            if (!scheduled[r][c-1] & heights[r][c] + 1 >= heights[r][c-1]) {
                scheduled[r][c-1] = true;
                distance_from_start[r][c-1] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r, c-1});
                if (end == std::tuple<int, int> {r, c-1}) {
                    break;
                }
            }
        }
        if (c < col_count - 1) {
            if (!scheduled[r][c+1] & heights[r][c] + 1 >= heights[r][c+1]) {
                scheduled[r][c+1] = true;
                distance_from_start[r][c+1] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r, c+1});
                if (end == std::tuple<int, int> {r, c+1}) {
                    break;
                }
            }
        }
    }
    return distance_from_start[std::get<0>(end)][std::get<1>(end)];
}
int steps_down_start_to_height(
        std::vector<std::vector<int>> heights, std::vector<std::vector<bool>> scheduled, 
        std::vector<std::vector<int>> distance_from_start, std::tuple<int, int> start, int end_height) {
    int row_count = heights.size();
    int col_count = heights[0].size();
    std::queue<std::tuple<int, int>> upcoming_visits;
    upcoming_visits.push(start);
    while (!upcoming_visits.empty()) {
        std::tuple<int, int> node = upcoming_visits.front();
        upcoming_visits.pop();
        int r = std::get<0>(node);
        int c = std::get<1>(node);
        if (r > 0) {
            if (!scheduled[r-1][c] & heights[r][c] - 1 <= heights[r-1][c]) {
                scheduled[r-1][c] = true;
                distance_from_start[r-1][c] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r-1, c});
                if (heights[r-1][c] == end_height) {
                    return distance_from_start[r-1][c];
                }
            }
        }
        if (r < row_count - 1) {
            if (!scheduled[r+1][c] & heights[r][c] - 1 <= heights[r+1][c]) {
                scheduled[r+1][c] = true;
                distance_from_start[r+1][c] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r+1, c});
                if (heights[r+1][c] == end_height) {
                    return distance_from_start[r+1][c];
                }
            }
        }
        if (c > 0) {
            if (!scheduled[r][c-1] & heights[r][c] - 1 <= heights[r][c-1]) {
                scheduled[r][c-1] = true;
                distance_from_start[r][c-1] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r, c-1});
                if (heights[r][c-1] == end_height) {
                    return distance_from_start[r][c-1];
                }
            }
        }
        if (c < col_count - 1) {
            if (!scheduled[r][c+1] & heights[r][c] - 1 <= heights[r][c+1]) {
                scheduled[r][c+1] = true;
                distance_from_start[r][c+1] = distance_from_start[r][c] + 1;
                upcoming_visits.push({r, c+1});
                if (heights[r][c+1] == end_height) {
                    return distance_from_start[r][c+1];
                }
            }
        }
    }
    return -1;
}

int steps_to_destination(std::string file_name, bool walk_up) {
    std::vector<std::vector<int>> heights;
    std::ifstream input(file_name);
    std::string line;
    int row_count = 0;
    int col_count = 0;
    std::tuple<int, int> start; 
    std::tuple<int, int> end; 
    while (std::getline(input, line)) {
        std::vector<int> row;
        col_count = 0;
        for (char &character : line) {
            if (character == 'S') {
                row.push_back(int('a'));
                start = {row_count, col_count};
            } else if (character == 'E') {
                row.push_back(int('z'));
                end = {row_count, col_count};
            } else {
                row.push_back(int(character));
            }
            col_count ++;
        }
        heights.push_back(row);
        row_count ++;
    }
    std::vector<std::vector<bool>> scheduled;
    std::vector<std::vector<int>> distance_from_start;
    for (int r = 0; r < row_count; r ++) {
        std::vector<bool> row_scheduled;
        std::vector<int> row_distance;
        for (int c = 0; c < col_count; c ++) {
            if (r == std::get<0>(start) & c == std::get<1>(start)) {
                row_scheduled.push_back(true);
                row_distance.push_back(0);
            } else {
                row_scheduled.push_back(false);
                row_distance.push_back(-1);
            }
        }
        scheduled.push_back(row_scheduled);
        distance_from_start.push_back(row_distance);
    }
    if (walk_up) {
        return steps_up_start_to_destination(heights, scheduled, distance_from_start, start, end);
    } else {
        return steps_down_start_to_height(heights, scheduled, distance_from_start, end, int('a'));
    }
}

int main() {
    std::cout << steps_to_destination("test1.txt", true) << std::endl;
    std::cout << steps_to_destination("test1.txt", false) << std::endl;
    std::cout << steps_to_destination("input.txt", true) << std::endl;
    std::cout << steps_to_destination("input.txt", false) << std::endl;
}