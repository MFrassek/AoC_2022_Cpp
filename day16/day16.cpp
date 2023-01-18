#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <utility>

class Valve {
    public:
        std::string name;
        int flow_rate;
        std::vector<Valve*> neighbors;

        Valve(std::string name, int flow_rate) {
            this->name = name;
            this->flow_rate = flow_rate;
        }

        void make_mutual_neighbors(Valve* other) {
            this->neighbors.push_back(other);
            other->neighbors.push_back(this);
        }
};

class Network {
    public:
        std::unordered_map<std::string, Valve*> valves;

        Network(std::vector<std::string> lines) {
            for (std::string& line : lines) {
                std::string name = line.substr(6, 2);
                int flow_rate_start_index = line.find('=') + 1;
                int flow_rate_end_index = line.find(';');
                int flow_rate = std::stoi(line.substr(flow_rate_start_index, flow_rate_end_index - flow_rate_start_index));
                int neighbor_start_index = line.find(" ", line.find("valve")) + 1;
                Valve* valve = new Valve(name, flow_rate);
                valves[name] = valve;
                std::string neighbor_name = line.substr(neighbor_start_index, 2);
                if (valves.find(neighbor_name) != valves.end()) {
                    valve->make_mutual_neighbors(valves[neighbor_name]);
                }
                while (line.find(',', neighbor_start_index) != std::string::npos) {
                    neighbor_start_index = line.find(',', neighbor_start_index) + 2;
                    neighbor_name = line.substr(neighbor_start_index, 2);
                    if (valves.find(neighbor_name) != valves.end()) {
                        valve->make_mutual_neighbors(valves[neighbor_name]);
                    }
                }
            }
        }
        int shortest_distance(Valve* start, Valve* destination) {
            std::queue<std::tuple<Valve*, int>> queue;
            std::unordered_set<std::string> visited_valve_names;
            queue.push({start, 0});
            visited_valve_names.insert(start->name);
            while (!queue.empty()) {
                Valve* current_valve = std::get<0>(queue.front());
                int current_distance = std::get<1>(queue.front());
                queue.pop();
                for (Valve* neighbor : current_valve->neighbors) {
                    if (neighbor == destination) {
                        return current_distance + 1;
                    }
                    if (visited_valve_names.find(neighbor->name) == visited_valve_names.end()) {
                        visited_valve_names.insert(neighbor->name);
                        queue.push({neighbor, current_distance + 1});

                    }
                }
            }
            return -1;
        }

        std::unordered_map<std::string, std::unordered_map<std::string, int>> distance_matrix() {
            std::unordered_map<std::string, std::unordered_map<std::string, int>> distance_matrix;
            for (auto &first_pair : this->valves) {
                std::string first_name = first_pair.first;
                Valve* first_valve = first_pair.second;
                if (first_valve->flow_rate != 0 || first_name == "AA") {
                    if (distance_matrix.find(first_name) == distance_matrix.end()) {
                        distance_matrix[first_name] = {};
                    }
                    for (auto &second_pair : this->valves) {
                        std::string second_name = second_pair.first;
                        Valve* second_valve = second_pair.second;
                        if (second_valve->flow_rate != 0) {
                            if (distance_matrix.find(second_name) == distance_matrix.end()) {
                                distance_matrix[second_name] = {};
                            }
                            if (first_name != second_name 
                                    && distance_matrix[first_name].find(second_name) == distance_matrix[first_name].end()) {
                                int distance = this->shortest_distance(first_valve, second_valve);
                                distance_matrix[first_name][second_name] = distance;
                                distance_matrix[second_name][first_name] = distance;
                            }
                        }
                    }
                }
            }
            return distance_matrix;
        }

        std::pair<int, std::unordered_set<std::string>> maximal_flow(
                std::unordered_set<std::string> visited_valve_names, Valve* current_valve, int remaining_time,
                std::unordered_map<std::string, std::unordered_map<std::string, int>> &distance_matrix) {
            int total_flow;
            if (current_valve->name == "AA") {
                total_flow = 0;
            } else {
                remaining_time--;
                total_flow = current_valve->flow_rate * remaining_time;
            }
            visited_valve_names.insert(current_valve->name);
            int max_additional_flow = 0;
            std::unordered_set<std::string> visited_for_solution_of_max;
            for (auto &pair : distance_matrix[current_valve->name]) {
                std::string target_valve_name = pair.first;
                int distance = pair.second;
                if (visited_valve_names.find(target_valve_name) == visited_valve_names.end() && distance < remaining_time) {
                    auto result = maximal_flow(
                        visited_valve_names, this->valves[target_valve_name], remaining_time - distance, distance_matrix);
                    int additional_flow = result.first;
                    std::unordered_set<std::string> visited_for_solution = result.second;
                    if (additional_flow > max_additional_flow) {
                        max_additional_flow = additional_flow;
                        visited_for_solution_of_max = visited_for_solution;
                    }
                }
            }
            total_flow += max_additional_flow;
            visited_for_solution_of_max.insert(current_valve->name);
            return std::make_pair(total_flow, visited_for_solution_of_max);
        }
};

int maximal_flow(std::string file_name, bool with_help) {
    std::ifstream input(file_name);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    Network network = Network(lines);
    std::unordered_map<std::string, std::unordered_map<std::string, int>> distance_matrix = network.distance_matrix();
    std::unordered_set<std::string> visited_valve_names;
    if (!with_help) {
        return network.maximal_flow(visited_valve_names, network.valves["AA"], 30, distance_matrix).first;
    } else {
        std::pair<int, std::unordered_set<std::string>> result = network.maximal_flow(visited_valve_names, network.valves["AA"], 26, distance_matrix);
        int flow_of_human = result.first;
        std::unordered_set<std::string> visited_by_human = result.second;
        int flow_of_elephant = network.maximal_flow(visited_by_human, network.valves["AA"], 26, distance_matrix).first;
        return flow_of_human + flow_of_elephant;
    }
}

int main() {
    std::cout << maximal_flow("test1.txt", false) << "\n";
    std::cout << maximal_flow("input.txt", false) << "\n";
    std::cout << maximal_flow("test1.txt", true) << "\n";
    std::cout << maximal_flow("input.txt", true) << "\n";
}