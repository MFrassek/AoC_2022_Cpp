#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <tuple>

class Point {
    public:
        int x;
        int y;

        Point(int x, int y) {
            this->x = x;
            this->y = y;
        }

        int manhatten_distance(Point* point) {
            return std::abs(this->x - point->x) + std::abs(this->y - point->y);
        }
};

class SensorBeaconPair {
    public:
        Point* sensor;
        Point* beacon;
        int coverage_distance;

        SensorBeaconPair(std::string line) {
            int sensor_x_start = line.find('=') + 1;
            int sensor_x_end = line.find(',');
            int sensor_y_start = line.find('=', sensor_x_end) + 1;
            int sensor_y_end = line.find(':', sensor_y_start);
            int beacon_x_start = line.find('=', sensor_y_end) + 1;
            int beacon_x_end = line.find(',', beacon_x_start);
            int beacon_y_start = line.find('=', beacon_x_end) + 1;
            int beacon_y_end = line.find(':', beacon_y_start);
            this->sensor = new Point(
                std::stoi(line.substr(sensor_x_start, sensor_x_end - sensor_x_start)),
                std::stoi(line.substr(sensor_y_start, sensor_y_end - sensor_y_start)));
            this->beacon = new Point(
                std::stoi(line.substr(beacon_x_start, beacon_x_end - beacon_x_start)),
                std::stoi(line.substr(beacon_y_start, beacon_y_end - beacon_y_start)));
            this->coverage_distance = this->sensor->manhatten_distance(this->beacon);
        }

        std::set<std::tuple<int, int>> low_x_low_y_edge_points() {
            std::set<std::tuple<int, int>> points;
            for (int i = 0; i <= this->coverage_distance + 1; i++) {
                points.insert({this->sensor->x - coverage_distance - 1 + i, this->sensor->y - i});
            }
            return points;
        }

        std::set<std::tuple<int, int>> low_x_high_y_edge_points() {
            std::set<std::tuple<int, int>> points;
            for (int i = 0; i <= this->coverage_distance + 1; i++) {
                points.insert({this->sensor->x - coverage_distance - 1 + i, this->sensor->y + i});
            }
            return points;
        }

        std::set<std::tuple<int, int>> high_x_low_y_edge_points() {
            std::set<std::tuple<int, int>> points;
            for (int i = 0; i <= this->coverage_distance + 1; i++) {
                points.insert({this->sensor->x + coverage_distance + 1 - i, this->sensor->y - i});
            }
            return points;
        }
        std::set<std::tuple<int, int>> high_x_high_y_edge_points() {
            std::set<std::tuple<int, int>> points;
            for (int i = 0; i <= this->coverage_distance + 1; i++) {
                points.insert({this->sensor->x + coverage_distance + 1 - i, this->sensor->y + i});
            }
            return points;
        }

        bool is_in_coverage(Point* target) {
            return this->sensor->manhatten_distance(target) <= coverage_distance;
        }
};

int tuning_frequency(std::string file_name, int max_x_y) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<SensorBeaconPair*> sensor_beacon_pairs;
    std::set<std::tuple<int, int>> decreasing_x_y_crevice_points;
    std::set<std::tuple<int, int>> increasing_x_y_crevice_points;
    while(std::getline(input, line)) {
        sensor_beacon_pairs.push_back(new SensorBeaconPair(line));
    }
    std::set<std::tuple<int, int>> all_low_x_low_y_points;
    std::set<std::tuple<int, int>> all_low_x_high_y_points;
    std::set<std::tuple<int, int>> all_high_x_low_y_points;
    std::set<std::tuple<int, int>> all_high_x_high_y_points;
    for (SensorBeaconPair* sensor_beacon_pair_1 : sensor_beacon_pairs) {
        for (std::tuple<int, int> point : sensor_beacon_pair_1->low_x_low_y_edge_points()) {
            all_low_x_low_y_points.insert(point);
        }
        for (std::tuple<int, int> point : sensor_beacon_pair_1->low_x_high_y_edge_points()) {
            all_low_x_high_y_points.insert(point);
        }
        for (std::tuple<int, int> point : sensor_beacon_pair_1->high_x_low_y_edge_points()) {
            all_high_x_low_y_points.insert(point);
        }
        for (std::tuple<int, int> point : sensor_beacon_pair_1->high_x_high_y_edge_points()) {
            all_high_x_high_y_points.insert(point);
        }
    }
    std::set<std::tuple<int, int>> two_side_overlap;
    std::set_intersection(
        all_low_x_low_y_points.begin(), all_low_x_low_y_points.end(),
        all_low_x_high_y_points.begin(), all_low_x_high_y_points.end(),
        std::inserter(two_side_overlap, two_side_overlap.begin()));
    std::set<std::tuple<int, int>> three_side_overlap;
    std::set_intersection(
        two_side_overlap.begin(), two_side_overlap.end(),
        all_high_x_low_y_points.begin(), all_high_x_low_y_points.end(),
        std::inserter(three_side_overlap, three_side_overlap.begin()));
    std::set<std::tuple<int, int>> four_side_overlap;
    std::set_intersection(
        three_side_overlap.begin(), three_side_overlap.end(),
        all_high_x_high_y_points.begin(), all_high_x_high_y_points.end(),
        std::inserter(four_side_overlap, four_side_overlap.begin()));
    for (std::tuple<int, int> point : four_side_overlap) {
        bool not_covered = true;
        if (std::get<0>(point) < 0 | std::get<0>(point) > max_x_y | std::get<1>(point) < 0 | std::get<1>(point) > max_x_y) {
            not_covered = false;
            continue;
        }
        Point checked_point = Point(std::get<0>(point), std::get<1>(point));
        for (SensorBeaconPair* sensor_beacon_pair : sensor_beacon_pairs) {
            if (sensor_beacon_pair->is_in_coverage(&checked_point)) {
                not_covered = false;
                break;
            }
        }
        if (not_covered) {
            return std::get<0>(point) * 4000000 + std::get<1>(point);
        }  
    }
    return -1;
}


int not_beacon_count(std::string file_name, int row) {
    std::ifstream input(file_name);
    std::string line;
    std::set<std::tuple<int, int>> covered_in_row;
    std::set<std::tuple<int, int>> beacons_in_row;
    while(std::getline(input, line)) {
        int sensor_x_start = line.find('=') + 1;
        int sensor_x_end = line.find(',');
        int sensor_y_start = line.find('=', sensor_x_end) + 1;
        int sensor_y_end = line.find(':', sensor_y_start);
        int beacon_x_start = line.find('=', sensor_y_end) + 1;
        int beacon_x_end = line.find(',', beacon_x_start);
        int beacon_y_start = line.find('=', beacon_x_end) + 1;
        int beacon_y_end = line.find(':', beacon_y_start);
        Point sensor = Point(
            std::stoi(line.substr(sensor_x_start, sensor_x_end - sensor_x_start)),
            std::stoi(line.substr(sensor_y_start, sensor_y_end - sensor_y_start)));
        Point beacon = Point(
            std::stoi(line.substr(beacon_x_start, beacon_x_end - beacon_x_start)),
            std::stoi(line.substr(beacon_y_start, beacon_y_end - beacon_y_start)));
        int distance = sensor.manhatten_distance(&beacon);
        if (sensor.y <= row & sensor.y + distance >= row) {
            int x_component = sensor.y + distance - row;
            for (int x = sensor.x - x_component; x <= sensor.x + x_component; x ++) {
                covered_in_row.insert({x, row});
            }
        } else if (sensor.y >= row & sensor.y - distance <= row) {
            int x_component = distance - (sensor.y - row);
            for (int x = sensor.x - x_component; x <= sensor.x + x_component; x ++) {
                covered_in_row.insert({x, row});
            }
        }
        if (beacon.y == row) {
            beacons_in_row.insert({beacon.x, beacon.y});
        }
    }
    return covered_in_row.size() - beacons_in_row.size();
}


int main() {
    std::cout << not_beacon_count("test1.txt", 10) << std::endl;
    std::cout << not_beacon_count("input.txt", 2000000) << std::endl;
    std::cout << tuning_frequency("test1.txt", 20) << std::endl;
    std::cout << tuning_frequency("input.txt", 4000000) << std::endl;
}