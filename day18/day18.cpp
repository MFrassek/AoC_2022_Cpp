#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <tuple>
#include <queue>

int any_surface_area(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::set<std::tuple<int, int, int>> positions;
    int surface_area = 0;
    while (std::getline(input, line)) {
        int first_comma_index = line.find(',');
        int second_comma_index = line.find(',', first_comma_index + 1);
        int x = std::stoi(line.substr(0, first_comma_index));
        int y = std::stoi(line.substr(first_comma_index + 1, second_comma_index - first_comma_index - 1));
        int z = std::stoi(line.substr(second_comma_index + 1, line.length() - second_comma_index - 1));
        positions.insert({x, y, z});
    }
    for (std::tuple<int, int, int> position : positions) {
        if (!positions.count({std::get<0>(position) + 1, std::get<1>(position), std::get<2>(position)})) {
            surface_area ++;
        }
        if (!positions.count({std::get<0>(position) - 1, std::get<1>(position), std::get<2>(position)})) {
            surface_area ++;
        }
        if (!positions.count({std::get<0>(position), std::get<1>(position) + 1, std::get<2>(position)})) {
            surface_area ++;
        }
        if (!positions.count({std::get<0>(position), std::get<1>(position) - 1, std::get<2>(position)})) {
            surface_area ++;
        }
        if (!positions.count({std::get<0>(position), std::get<1>(position), std::get<2>(position) + 1})) {
            surface_area ++;
        }
        if (!positions.count({std::get<0>(position), std::get<1>(position), std::get<2>(position) - 1})) {
            surface_area ++;
        }
    }
    return surface_area;
}

bool is_along_surface(std::tuple<int, int, int> point, std::set<std::tuple<int, int, int>>* lava) {
    if (lava->count({std::get<0>(point), std::get<1>(point), std::get<2>(point)})) {
        return false;
    }
    if (lava->count({std::get<0>(point) - 1, std::get<1>(point) - 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) - 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) - 1, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 0, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 0, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 0, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) - 1, std::get<1>(point) + 1, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) - 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) - 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) - 1, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) + 0, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) + 0, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) + 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) + 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) + 0, std::get<1>(point) + 1, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) - 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) - 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) - 1, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 0, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 0, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 0, std::get<2>(point) + 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 1, std::get<2>(point) - 1})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 1, std::get<2>(point) + 0})
            || lava->count({std::get<0>(point) + 1, std::get<1>(point) + 1, std::get<2>(point) + 1})) {
        return true;
    }
    return false;
}

int external_surface_area(std::string file_name) {
    std::ifstream input(file_name);
    std::string line;
    std::set<std::tuple<int, int, int>> positions;
    int surface_area = 0;
    std::tuple<int, int, int> left_most_point = {100, 100, 100};
    while (std::getline(input, line)) {
        int first_comma_index = line.find(',');
        int second_comma_index = line.find(',', first_comma_index + 1);
        int x = std::stoi(line.substr(0, first_comma_index));
        int y = std::stoi(line.substr(first_comma_index + 1, second_comma_index - first_comma_index - 1));
        int z = std::stoi(line.substr(second_comma_index + 1, line.length() - second_comma_index - 1));
        positions.insert({x, y, z});
        if (x < std::get<0>(left_most_point)) {
            left_most_point = {x, y, z};
        }
    }
    std::queue<std::tuple<int, int, int>> outside_point_queue;
    std::set<std::tuple<int, int, int>> outside_points;
    outside_point_queue.push({std::get<0>(left_most_point) - 1, std::get<1>(left_most_point), std::get<2>(left_most_point)});
    outside_points.insert({std::get<0>(left_most_point) - 1, std::get<1>(left_most_point), std::get<2>(left_most_point)});
    while (!outside_point_queue.empty()) {
        int x = std::get<0>(outside_point_queue.front());
        int y = std::get<1>(outside_point_queue.front());
        int z = std::get<2>(outside_point_queue.front());
        if (is_along_surface({x - 1, y, z}, &positions) && !outside_points.count({x - 1, y, z})) {
            outside_point_queue.push({x - 1, y, z});
            outside_points.insert({x - 1, y, z});
        }
        if (is_along_surface({x + 1, y, z}, &positions) && !outside_points.count({x + 1, y, z})) {
            outside_point_queue.push({x + 1, y, z});
            outside_points.insert({x + 1, y, z});
        }
        if (is_along_surface({x, y - 1, z}, &positions) && !outside_points.count({x, y - 1, z})) {
            outside_point_queue.push({x, y - 1, z});
            outside_points.insert({x, y - 1, z});
        }
        if (is_along_surface({x, y + 1, z}, &positions) && !outside_points.count({x, y + 1, z})) {
            outside_point_queue.push({x, y + 1, z});
            outside_points.insert({x, y + 1, z});
        }
        if (is_along_surface({x, y, z - 1}, &positions) && !outside_points.count({x, y, z - 1})) {
            outside_point_queue.push({x, y, z - 1});
            outside_points.insert({x, y, z - 1});
        }
        if (is_along_surface({x, y, z + 1}, &positions) && !outside_points.count({x, y, z + 1})) {
            outside_point_queue.push({x, y, z + 1});
            outside_points.insert({x, y, z + 1});
        }
        outside_point_queue.pop();
    }
    for (std::tuple<int, int, int> position : outside_points) {
        if (positions.count({std::get<0>(position) + 1, std::get<1>(position), std::get<2>(position)})) {
            surface_area ++;
        }
        if (positions.count({std::get<0>(position) - 1, std::get<1>(position), std::get<2>(position)})) {
            surface_area ++;
        }
        if (positions.count({std::get<0>(position), std::get<1>(position) + 1, std::get<2>(position)})) {
            surface_area ++;
        }
        if (positions.count({std::get<0>(position), std::get<1>(position) - 1, std::get<2>(position)})) {
            surface_area ++;
        }
        if (positions.count({std::get<0>(position), std::get<1>(position), std::get<2>(position) + 1})) {
            surface_area ++;
        }
        if (positions.count({std::get<0>(position), std::get<1>(position), std::get<2>(position) - 1})) {
            surface_area ++;
        }
    }
    return surface_area;
}


int main() {
    std::cout << any_surface_area("test1.txt") << std::endl;
    std::cout << any_surface_area("input.txt") << std::endl;
    std::cout << external_surface_area("test1.txt") << std::endl;
    std::cout << external_surface_area("input.txt") << std::endl;
}